#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <thread>
#include <mutex>
#include <chrono>
#include <atomic>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <cstring>
#include <csignal>
#include <fcntl.h>
#include <map>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>  // Добавлено для GetAsyncKeyState
#pragma comment(lib, "ws2_32.lib")
// НЕ ИСПОЛЬЗУЕМ #define close closesocket
#define SHUT_RDWR SD_BOTH
typedef int socklen_t;
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/time.h>
#endif

class HttpLogServer {
private:
#ifdef _WIN32
    SOCKET server_socket;
#else
    int server_socket;
#endif
    int port;
    std::string log_file;
    std::mutex log_mutex;
    std::atomic<bool> running;
    std::thread accept_thread;
    std::vector<std::thread> client_threads;
    std::mutex threads_mutex;
    std::atomic<size_t> total_requests;
    std::atomic<size_t> successful_requests;
    std::atomic<size_t> failed_requests;

public:
    HttpLogServer(int port, const std::string &log_file_path)
        : port(port), log_file(log_file_path), running(false),
        total_requests(0), successful_requests(0), failed_requests(0)
    {
#ifdef _WIN32
        server_socket = INVALID_SOCKET;
#else
        server_socket = -1;
#endif

#ifndef _WIN32
        // Установка обработчика сигналов (только для Linux)
        signal(SIGPIPE, SIG_IGN);
#endif
    }

    ~HttpLogServer()
    {
        stop();
    }

    bool start()
    {
#ifdef _WIN32
        // Инициализация Winsock
        WSADATA wsa_data;
        if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0)
        {
            std::cerr << "Failed to initialize Winsock" << std::endl;
            return false;
        }
#endif

        server_socket = socket(AF_INET, SOCK_STREAM, 0);
#ifdef _WIN32
        if (server_socket == INVALID_SOCKET)
        {
#else
        if (server_socket < 0)
        {
#endif
            std::cerr << "Failed to create socket" << std::endl;
            return false;
        }

        int opt = 1;
        if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR,
            (char *)&opt, sizeof(opt)) < 0)
        {
            std::cerr << "Failed to set socket options" << std::endl;
            closeSocket(server_socket);
            return false;
        }

        sockaddr_in server_addr;
        memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = INADDR_ANY;
        server_addr.sin_port = htons(port);

        if (bind(server_socket, (sockaddr *)&server_addr, sizeof(server_addr)) < 0)
        {
            std::cerr << "Failed to bind to port " << port << std::endl;
            closeSocket(server_socket);
            return false;
        }

        if (listen(server_socket, SOMAXCONN) < 0)
        {
            std::cerr << "Failed to listen" << std::endl;
            closeSocket(server_socket);
            return false;
        }

        running = true;
        accept_thread = std::thread(&HttpLogServer::acceptLoop, this);

        std::cout << "=== HTTP Log Server ===" << std::endl;
        std::cout << "Server started on port: " << port << std::endl;
        std::cout << "Log file: " << log_file << std::endl;
        std::cout << "Press Ctrl+C to stop the server" << std::endl;
        std::cout << "=========================" << std::endl;

        return true;
        }

    void stop()
    {
        if (running)
        {
            running = false;
#ifdef _WIN32
            if (server_socket != INVALID_SOCKET)
            {
#else
            if (server_socket >= 0)
            {
#endif
                shutdown(server_socket, SHUT_RDWR);
                closeSocket(server_socket);
#ifdef _WIN32
                server_socket = INVALID_SOCKET;
#else
                server_socket = -1;
#endif
            }

            if (accept_thread.joinable())
            {
                accept_thread.join();
            }

            // Ожидание завершения всех клиентских потоков
            std::lock_guard<std::mutex> lock(threads_mutex);
            for (auto &thread : client_threads)
            {
                if (thread.joinable())
                {
                    thread.join();
                }
            }
            client_threads.clear();

            std::cout << "Server stopped" << std::endl;

#ifdef _WIN32
            WSACleanup();
#endif
            }
        }

    // Получение статистики
    size_t getTotalRequests() const
    {
        return total_requests.load();
    }
    size_t getSuccessfulRequests() const
    {
        return successful_requests.load();
    }
    size_t getFailedRequests() const
    {
        return failed_requests.load();
    }

private:
    // Вспомогательная функция для закрытия сокета
    void closeSocket(
#ifdef _WIN32
        SOCKET sock
#else
        int sock
#endif
    )
    {
#ifdef _WIN32
        closesocket(sock);
#else
        close(sock);
#endif
    }

    void acceptLoop()
    {
        while (running)
        {
            sockaddr_in client_addr;
            socklen_t client_len = sizeof(client_addr);

#ifdef _WIN32
            SOCKET client_socket = accept(server_socket, (sockaddr *)&client_addr, &client_len);
            if (client_socket == INVALID_SOCKET)
            {
#else
            int client_socket = accept(server_socket, (sockaddr *)&client_addr, &client_len);
            if (client_socket < 0)
            {
#endif
                if (running)
                {
#ifdef _WIN32
                    std::cerr << "Failed to accept connection: " << WSAGetLastError() << std::endl;
#else
                    std::cerr << "Failed to accept connection" << std::endl;
#endif
                }
                continue;
            }

            // Получение IP клиента
            char client_ip[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &(client_addr.sin_addr), client_ip, INET_ADDRSTRLEN);

            // Обработка каждого клиента в отдельном потоке
            std::lock_guard<std::mutex> lock(threads_mutex);
            client_threads.emplace_back(&HttpLogServer::handleClient, this,
                client_socket, std::string(client_ip));
            }
        }

    void handleClient(
#ifdef _WIN32
        SOCKET client_socket
#else
        int client_socket
#endif
        , const std::string & client_ip)
    {
        total_requests++;

        // Установка таймаута на чтение
#ifdef _WIN32
        DWORD timeout = 10000; // 10 секунд
        setsockopt(client_socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout));
#else
        struct timeval timeout;
        timeout.tv_sec = 10;
        timeout.tv_usec = 0;
        setsockopt(client_socket, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
#endif

        char buffer[8192];
        memset(buffer, 0, sizeof(buffer));

        int bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received <= 0)
        {
            failed_requests++;
            closeSocket(client_socket);
            return;
        }

        std::string request(buffer);

        // Обработка различных методов
        if (request.find("POST /log") != std::string::npos ||
            request.find("POST") != std::string::npos)
        {
            if (handleLogRequest(client_socket, request, client_ip))
            {
                successful_requests++;
            }
            else
            {
                failed_requests++;
            }
        }
        else if (request.find("GET /stats") != std::string::npos)
        {
            handleStatsRequest(client_socket);
            successful_requests++;
        }
        else if (request.find("GET /health") != std::string::npos)
        {
            handleHealthRequest(client_socket);
            successful_requests++;
        }
        else if (request.find("GET") != std::string::npos)
        {
            handleGetRequest(client_socket);
            successful_requests++;
        }
        else
        {
            sendErrorResponse(client_socket, 405, "Method Not Allowed");
            failed_requests++;
        }

        closeSocket(client_socket);
    }

    bool handleLogRequest(
#ifdef _WIN32
        SOCKET client_socket
#else
        int client_socket
#endif
        , const std::string & request, const std::string & client_ip)
    {
        // Извлечение тела запроса
        size_t body_pos = request.find("\r\n\r\n");
        if (body_pos == std::string::npos)
        {
            sendErrorResponse(client_socket, 400, "Bad Request");
            return false;
        }

        std::string body = request.substr(body_pos + 4);

        // Проверка на пустое тело
        if (body.empty())
        {
            sendErrorResponse(client_socket, 400, "Empty body");
            return false;
        }

        // Логирование сообщения
        logMessage(body, client_ip);

        // Отправка ответа
        std::string response_json = "{\"status\":\"ok\",\"timestamp\":" +
            std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::system_clock::now().time_since_epoch()).count()) + "}";

        sendJsonResponse(client_socket, 200, response_json);
        return true;
    }

    void handleStatsRequest(
#ifdef _WIN32
        SOCKET client_socket
#else
        int client_socket
#endif
    )
    {
        std::stringstream stats;
        stats << "{";
        stats << "\"status\":\"running\",";
        stats << "\"log_file\":\"" << log_file << "\",";
        stats << "\"total_requests\":" << total_requests.load() << ",";
        stats << "\"successful_requests\":" << successful_requests.load() << ",";
        stats << "\"failed_requests\":" << failed_requests.load();
        stats << "}";

        sendJsonResponse(client_socket, 200, stats.str());
    }

    void handleHealthRequest(
#ifdef _WIN32
        SOCKET client_socket
#else
        int client_socket
#endif
    )
    {
        std::string health = "{\"status\":\"healthy\",\"timestamp\":" +
            std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::system_clock::now().time_since_epoch()).count()) + "}";

        sendJsonResponse(client_socket, 200, health);
    }

    void handleGetRequest(
#ifdef _WIN32
        SOCKET client_socket
#else
        int client_socket
#endif
    )
    {
        std::string html = R"(
<!DOCTYPE html>
<html>
<head>
    <title>Log Server</title>
    <style>
        body { font-family: Arial, sans-serif; margin: 40px; }
        h1 { color: #333; }
        .status { color: green; font-weight: bold; }
        .info { background-color: #f0f0f0; padding: 20px; border-radius: 5px; }
        .stats { margin-top: 20px; }
        .stat-item { margin: 5px 0; }
    </style>
</head>
<body>
    <h1>Log Server</h1>
    <p class="status">Server is running</p>
    <div class="info">
        <p>Send POST requests to /log with JSON body to log messages.</p>
        <p>Example: {"level":"INFO","message":"Test message"}</p>
    </div>
    <div class="stats">
        <h3>Statistics:</h3>
        <div class="stat-item">Total Requests: )" + std::to_string(total_requests.load()) + R"(</div>
        <div class="stat-item">Successful: )" + std::to_string(successful_requests.load()) + R"(</div>
        <div class="stat-item">Failed: )" + std::to_string(failed_requests.load()) + R"(</div>
    </div>
</body>
</html>
)";

        sendHtmlResponse(client_socket, html);
    }

    void sendJsonResponse(
#ifdef _WIN32
        SOCKET client_socket
#else
        int client_socket
#endif
        , int status_code, const std::string & json_body)
    {
        std::string response = "HTTP/1.1 " + std::to_string(status_code) + " " +
            getHttpStatusText(status_code) + "\r\n";
        response += "Content-Type: application/json\r\n";
        response += "Content-Length: " + std::to_string(json_body.length()) + "\r\n";
        response += "Connection: close\r\n";
        response += "Access-Control-Allow-Origin: *\r\n";
        response += "\r\n";
        response += json_body;

        send(client_socket, response.c_str(), (int)response.length(), 0);
    }

    void sendHtmlResponse(
#ifdef _WIN32
        SOCKET client_socket
#else
        int client_socket
#endif
        , const std::string & html_body)
    {
        std::string response = "HTTP/1.1 200 OK\r\n";
        response += "Content-Type: text/html\r\n";
        response += "Content-Length: " + std::to_string(html_body.length()) + "\r\n";
        response += "Connection: close\r\n";
        response += "\r\n";
        response += html_body;

        send(client_socket, response.c_str(), (int)response.length(), 0);
    }

    void sendErrorResponse(
#ifdef _WIN32
        SOCKET client_socket
#else
        int client_socket
#endif
        , int error_code, const std::string & error_message)
    {
        std::string response = "HTTP/1.1 " + std::to_string(error_code) + " " +
            getHttpStatusText(error_code) + "\r\n";
        response += "Content-Type: text/plain\r\n";
        response += "Content-Length: " + std::to_string(error_message.length()) + "\r\n";
        response += "Connection: close\r\n";
        response += "\r\n";
        response += error_message;

        send(client_socket, response.c_str(), (int)response.length(), 0);
    }

    std::string getHttpStatusText(int code)
    {
        switch (code)
        {
        case 200: return "OK";
        case 400: return "Bad Request";
        case 404: return "Not Found";
        case 405: return "Method Not Allowed";
        case 500: return "Internal Server Error";
        default: return "Unknown";
        }
    }

    void logMessage(const std::string & message, const std::string & client_ip)
    {
        std::lock_guard<std::mutex> lock(log_mutex);

        // Получение текущего времени
        auto now = std::chrono::system_clock::now();
        auto now_time_t = std::chrono::system_clock::to_time_t(now);
        auto now_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            now.time_since_epoch()) % 1000;

        std::tm *now_tm = std::localtime(&now_time_t);
        char time_buffer[30];
        strftime(time_buffer, sizeof(time_buffer), "%Y-%m-%d %H:%M:%S", now_tm);

        // Форматирование сообщения для лога
        std::stringstream log_stream;
        log_stream << "[" << time_buffer << "." << std::setfill('0') << std::setw(3)
            << now_ms.count() << "] ";
        log_stream << "[IP: " << client_ip << "] ";
        log_stream << message;

        std::string log_entry = log_stream.str();

        // Запись в файл
        std::ofstream file_stream(log_file, std::ios::app);
        if (file_stream.is_open())
        {
            file_stream << log_entry << std::endl;
            file_stream.close();  // Теперь это работает, так как нет макроса close
        }

        // Вывод в консоль
        std::cout << log_entry << std::endl;
    }
    };

// Глобальный указатель на сервер для обработки сигналов
HttpLogServer *global_server = nullptr;

#ifndef _WIN32
void signalHandler(int signal)
{
    if (global_server)
    {
        std::cout << "\nReceived signal " << signal << ", stopping server..." << std::endl;
        global_server->stop();
        exit(0);
    }
}
#endif

int main(int argc, char *argv[])
{
    int port = 8080;
    std::string log_file = "cpp_app.log";  // Для Windows

#ifdef __linux__
    log_file = "/var/log/cpp_app.log";  // Для Linux по умолчанию
#endif

    // Обработка аргументов командной строки
    for (int i = 1; i < argc; i++)
    {
        std::string arg = argv[i];
        if (arg == "-p" && i + 1 < argc)
        {
            port = std::stoi(argv[i + 1]);
            i++;
        }
        else if (arg == "-l" && i + 1 < argc)
        {
            log_file = argv[i + 1];
            i++;
        }
        else if (arg == "-h" || arg == "--help")
        {
            std::cout << "Usage: " << argv[0] << " [options]" << std::endl;
            std::cout << "Options:" << std::endl;
            std::cout << "  -p <port>     Port number (default: 8080)" << std::endl;
            std::cout << "  -l <file>     Log file path" << std::endl;
            std::cout << "  -h, --help    Show this help" << std::endl;
            return 0;
        }
    }

    // Создание сервера
    HttpLogServer server(port, log_file);
    global_server = &server;

#ifndef _WIN32
    // Установка обработчиков сигналов (только для Linux)
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);
#endif

    // Запуск сервера
    if (!server.start())
    {
        std::cerr << "Failed to start server" << std::endl;
        return 1;
    }

    // Бесконечный цикл ожидания
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));

#ifdef _WIN32
        // Проверка на Ctrl+C в Windows
        if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
        {
            if (GetAsyncKeyState('C') & 0x8000)
            {
                std::cout << "\nCtrl+C detected, stopping server..." << std::endl;
                server.stop();
                break;
            }
        }
#endif
    }

    return 0;
}
