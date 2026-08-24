// 2026/08/24 16:27:45 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once
#include <string>
#include <mutex>
#include <vector>
#include <atomic>
#include <thread>


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
    typedef int SOCKET;
#endif


class HttpLogServer
{
public:

    HttpLogServer(int port, const std::string &log_file_path);
    ~HttpLogServer();

    bool Start();
    void Stop();

private:

    SOCKET server_socket;

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

    void CloseSocket(SOCKET sock);
    void HandleClient(SOCKET client_socket, const std::string &client_ip);
    bool HandleLogRequest(SOCKET client_socket, const std::string &request, const std::string &client_ip);
    void HandleStatsRequest(SOCKET client_socket);
    void HandleHealthRequest(SOCKET client_socket);
    void HandleGetRequest(SOCKET client_socket);
    void SendErrorResponse(SOCKET client_socket, int error_code, const std::string &error_message);
    void LogMessage(const std::string &message, const std::string &client_ip);
    void SendJsonResponse(SOCKET client_socket, int status_code, const std::string &json_body);
    void SendHtmlResponse(SOCKET client_socket, const std::string &html_body);
    std::string GetHttpStatusText(int code);
    std::string SanitizeString(const std::string &input);

    void AcceptLoop();

    size_t GetTotalRequests() const;
    size_t GetSuccessfulRequests() const;
    size_t GetFailedRequests() const;
};
