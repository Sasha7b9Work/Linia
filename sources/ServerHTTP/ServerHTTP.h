// 2026/08/24 16:27:45 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once
#include <string>
#include <mutex>
#include <vector>


#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #include <windows.h>  // Добавлено для GetAsyncKeyState
    #pragma comment(lib, "ws2_32.lib")
    #define SHUT_RDWR SD_BOTH
    typedef int socklen_t;
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #include <sys/time.h>
#endif


class HttpLogServer
{
public:

    HttpLogServer(int port, const std::string &log_file_path);
    ~HttpLogServer();

    bool Start();
    void Stop();

private:

#ifdef _WIN32
    SOCKET server_socket;
#else
    int server_socket;
#endif
    int port;
    std::string log_file;
    std::mutex log_mutex;
    std::atomic<bool> running{ false };
    std::thread accept_thread;
    std::vector<std::thread> client_threads;
    std::mutex threads_mutex;
    std::atomic<size_t> total_requests{ 0 };
    std::atomic<size_t> successful_requests{ 0 };
    std::atomic<size_t> failed_requests{ 0 };

    void CloseSocket(
#ifdef _WIN32
        SOCKET sock
#else
        int sock
#endif
    );

    void AcceptLoop();

    size_t GetTotalRequests() const;
    size_t GetSuccessfulRequests() const;
    size_t GetFailedRequests() const;
};
