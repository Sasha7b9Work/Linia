// 2026/08/24 15:12:20 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once
#include <string>
#include <vector>
#include <map>


struct NetworkInterface
{
    std::string name;
    std::string ip_address;
    std::string netmask;
    std::string broadcast;
    bool is_up;
    bool is_loopback;
};


class SystemInfoExtended
{
public:
    static std::string getComputerName();
    static std::string getLocalIPAddress();
    static std::vector<NetworkInterface> getNetworkInterfaces();
    static std::string getOSVersion();
    static std::string getCurrentUser();
    static std::map<std::string, std::string> getAllSystemInfo();
};
