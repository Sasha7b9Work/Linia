// 2026/08/24 15:12:30 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "Utils/SystemInfo.h"
#include <sstream>
#include <cstring>
#include <algorithm>

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #include <windows.h>
    #include <iphlpapi.h>
    #include <lmcons.h>
    #pragma comment(lib, "ws2_32.lib")
    #pragma comment(lib, "iphlpapi.lib")
    #pragma comment(lib, "advapi32.lib")
#else
    #include <unistd.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <netdb.h>
    #include <ifaddrs.h>
    #include <net/if.h>
    #include <pwd.h>
    #include <sys/utsname.h>
#endif

#pragma warning(push, 0)
#pragma warning(disable:4996)


std::string SystemInfoExtended::getComputerName()
{
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));

#ifdef _WIN32
    DWORD size = sizeof(buffer);
    if (GetComputerNameA(buffer, &size))
    {
        return std::string(buffer);
    }
#else
    if (gethostname(buffer, sizeof(buffer)) == 0)
    {
        return std::string(buffer);
    }
#endif

    return "Unknown";
}

std::string SystemInfoExtended::getLocalIPAddress()
{
    auto interfaces = getNetworkInterfaces();

    for (const auto &iface : interfaces)
    {
        if (!iface.is_loopback && iface.is_up && !iface.ip_address.empty())
        {
            // Ищем частный IP
            if (iface.ip_address.find("192.168.") == 0 ||
                iface.ip_address.find("10.") == 0 ||
                iface.ip_address.find("172.") == 0)
            {
                return iface.ip_address;
            }
        }
    }

    // Если частный не найден, возвращаем первый доступный
    for (const auto &iface : interfaces)
    {
        if (!iface.is_loopback && iface.is_up && !iface.ip_address.empty())
        {
            return iface.ip_address;
        }
    }

    return "127.0.0.1";
}

std::vector<NetworkInterface> SystemInfoExtended::getNetworkInterfaces()
{
    std::vector<NetworkInterface> interfaces;

#ifdef _WIN32
    WSADATA wsa_data;
    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0)
    {
        return interfaces;
    }

    ULONG buffer_size = 0;
    GetAdaptersAddresses(AF_INET, 0, nullptr, nullptr, &buffer_size);

    std::vector<unsigned char> buffer(buffer_size);
    PIP_ADAPTER_ADDRESSES adapters = reinterpret_cast<PIP_ADAPTER_ADDRESSES>(buffer.data());

    if (GetAdaptersAddresses(AF_INET, 0, nullptr, adapters, &buffer_size) == NO_ERROR)
    {
        for (PIP_ADAPTER_ADDRESSES adapter = adapters; adapter != nullptr; adapter = adapter->Next)
        {
            for (PIP_ADAPTER_UNICAST_ADDRESS addr = adapter->FirstUnicastAddress;
                addr != nullptr; addr = addr->Next)
            {

                if (addr->Address.lpSockaddr->sa_family == AF_INET)
                {
                    NetworkInterface iface;
                    iface.name = adapter->AdapterName;

                    sockaddr_in *sockaddr = (sockaddr_in *)addr->Address.lpSockaddr;
                    char ip_buffer[INET_ADDRSTRLEN];
                    inet_ntop(AF_INET, &(sockaddr->sin_addr), ip_buffer, sizeof(ip_buffer));
                    iface.ip_address = ip_buffer;

                    iface.is_up = (adapter->OperStatus == IfOperStatusUp);
                    iface.is_loopback = (adapter->IfType == IF_TYPE_SOFTWARE_LOOPBACK);

                    interfaces.push_back(iface);
                }
            }
        }
    }

    WSACleanup();
#else
    struct ifaddrs *ifaddr = nullptr;
    if (getifaddrs(&ifaddr) == 0)
    {
        for (struct ifaddrs *ifa = ifaddr; ifa != nullptr; ifa = ifa->ifa_next)
        {
            if (ifa->ifa_addr == nullptr) continue;

            if (ifa->ifa_addr->sa_family == AF_INET)
            {
                NetworkInterface iface;
                iface.name = ifa->ifa_name;

                struct sockaddr_in *addr = (struct sockaddr_in *)ifa->ifa_addr;
                char ip_buffer[INET_ADDRSTRLEN];
                inet_ntop(AF_INET, &(addr->sin_addr), ip_buffer, sizeof(ip_buffer));
                iface.ip_address = ip_buffer;

                iface.is_up = (ifa->ifa_flags & IFF_UP) != 0;
                iface.is_loopback = (ifa->ifa_flags & IFF_LOOPBACK) != 0;

                // Получение netmask
                if (ifa->ifa_netmask)
                {
                    struct sockaddr_in *netmask = (struct sockaddr_in *)ifa->ifa_netmask;
                    char netmask_buffer[INET_ADDRSTRLEN];
                    inet_ntop(AF_INET, &(netmask->sin_addr), netmask_buffer, sizeof(netmask_buffer));
                    iface.netmask = netmask_buffer;
                }

                interfaces.push_back(iface);
            }
        }
        freeifaddrs(ifaddr);
    }
#endif

    return interfaces;
}

std::string SystemInfoExtended::getOSVersion()
{
#ifdef _WIN32
    OSVERSIONINFOA osvi;
    ZeroMemory(&osvi, sizeof(OSVERSIONINFOA));
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOA);

    if (GetVersionExA(&osvi))       // warning C4996
    {
        return "Windows " + std::to_string(osvi.dwMajorVersion) + "." +
            std::to_string(osvi.dwMinorVersion);
    }
    return "Windows";
#else
    struct utsname info;
    if (uname(&info) == 0)
    {
        return std::string(info.sysname) + " " + info.release;
    }
    return "Linux";
#endif
}

std::string SystemInfoExtended::getCurrentUser()
{
#ifdef _WIN32
    char username[256];
    DWORD size = sizeof(username);
    if (GetUserNameA(username, &size))
    {
        return std::string(username);
    }
#else
    struct passwd *pw = getpwuid(getuid());
    if (pw)
    {
        return std::string(pw->pw_name);
    }
#endif
    return "Unknown";
}

std::map<std::string, std::string> SystemInfoExtended::getAllSystemInfo()
{
    std::map<std::string, std::string> info;
    info["computer_name"] = getComputerName();
    info["local_ip"] = getLocalIPAddress();
    info["os_version"] = getOSVersion();
    info["user"] = getCurrentUser();
    return info;
}

#pragma warning(pop)
