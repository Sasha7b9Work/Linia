// 2026/08/24 15:00:53 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "ServerHTTP.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <algorithm>
#include <iomanip>
#include <cstring>
#include <csignal>
#include <fcntl.h>
#include <map>


HttpLogServer::HttpLogServer(int port, const std::string &log_file_path) :
    port(port),
    log_file(log_file_path)
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

HttpLogServer::~HttpLogServer()
{
    Stop();
}

bool HttpLogServer::Start()
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
        CloseSocket(server_socket);
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
        CloseSocket(server_socket);
        return false;
    }

    if (listen(server_socket, SOMAXCONN) < 0)
    {
        std::cerr << "Failed to listen" << std::endl;
        CloseSocket(server_socket);
        return false;
    }

    running = true;
    accept_thread = std::thread(&HttpLogServer::AcceptLoop, this);

    std::cout << "=== HTTP Log Server ===" << std::endl;
    std::cout << "Server started on port: " << port << std::endl;
    std::cout << "Log file: " << log_file << std::endl;
    std::cout << "Press Ctrl+C to stop the server" << std::endl;
    std::cout << "=========================" << std::endl;

    return true;
    }

void HttpLogServer::Stop()
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
            CloseSocket(server_socket);
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
size_t HttpLogServer::GetTotalRequests() const
{
    return total_requests.load();
}


size_t HttpLogServer::GetSuccessfulRequests() const
{
    return successful_requests.load();
}


size_t  HttpLogServer::GetFailedRequests() const
{
    return failed_requests.load();
}

// Вспомогательная функция для закрытия сокета
void HttpLogServer::CloseSocket(
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


std::string HttpLogServer::GetHttpMethod(const std::string & request)
{
    size_t space_pos = request.find(' ');
    if (space_pos != std::string::npos)
    {
        return request.substr(0, space_pos);
    }
    return "";
}


std::string HttpLogServer::GetHttpPath(const std::string & request)
{
    size_t first_space = request.find(' ');
    if (first_space != std::string::npos)
    {
        size_t second_space = request.find(' ', first_space + 1);
        if (second_space != std::string::npos)
        {
            return request.substr(first_space + 1, second_space - first_space - 1);
        }
    }
    return "";
}


void HttpLogServer::AcceptLoop()
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
        client_threads.emplace_back(&HttpLogServer::HandleClient, this, client_socket, std::string(client_ip));
        }
}

void HttpLogServer::HandleClient(
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
        CloseSocket(client_socket);
        return;
    }

    std::string request(buffer);

    std::string method = GetHttpMethod(request);
    std::string path = GetHttpPath(request);

    if (method == "POST" && path == "/log")
    {
        if (HandleLogRequest(client_socket, request, client_ip))
        {
            successful_requests++;
        }
        else
        {
            failed_requests++;
        }
    }
    else if (method == "POST")
    {
        // Любой другой POST
        if (HandleLogRequest(client_socket, request, client_ip))
        {
            successful_requests++;
        }
        else
        {
            failed_requests++;
        }
    }
    else if (method == "GET" && path == "/stats")
    {
        HandleStatsRequest(client_socket);
        successful_requests++;
    }
    else if (method == "GET" && path == "/health")
    {
        HandleHealthRequest(client_socket);
        successful_requests++;
    }
    else if (method == "GET")
    {
        HandleGetRequest(client_socket);
        successful_requests++;
    }
    else
    {
        SendErrorResponse(client_socket, 405, "Method Not Allowed");
        failed_requests++;
    }

    CloseSocket(client_socket);
}

bool HttpLogServer::HandleLogRequest(SOCKET client_socket, const std::string & request, const std::string & client_ip)
{
    // Извлечение тела запроса
    size_t body_pos = request.find("\r\n\r\n");
    if (body_pos == std::string::npos)
    {
        SendErrorResponse(client_socket, 400, "Bad Request");
        return false;
    }

    std::string body = request.substr(body_pos + 4);

    // Проверка на пустое тело
    if (body.empty())
    {
        SendErrorResponse(client_socket, 400, "Empty body");
        return false;
    }

    // Логирование сообщения
    LogMessage(body, client_ip);

    // Отправка ответа
    std::string response_json = "{\"status\":\"ok\",\"timestamp\":" +
        std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count()) + "}";

    SendJsonResponse(client_socket, 200, response_json);
    return true;
}


void HttpLogServer::HandleStatsRequest(SOCKET client_socket)
{
    std::stringstream stats;
    stats << "{";
    stats << "\"status\":\"running\",";
    stats << "\"log_file\":\"" << log_file << "\",";
    stats << "\"total_requests\":" << total_requests.load() << ",";
    stats << "\"successful_requests\":" << successful_requests.load() << ",";
    stats << "\"failed_requests\":" << failed_requests.load();
    stats << "}";

    SendJsonResponse(client_socket, 200, stats.str());
}

void HttpLogServer::HandleHealthRequest(SOCKET client_socket)
{
    std::string health = "{\"status\":\"healthy\",\"timestamp\":" +
        std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count()) + "}";

    SendJsonResponse(client_socket, 200, health);
}

void HttpLogServer::HandleGetRequest(SOCKET client_socket)
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

    SendHtmlResponse(client_socket, html);
}

void HttpLogServer::SendJsonResponse(SOCKET client_socket, int status_code, const std::string & json_body)
{
    std::string response = "HTTP/1.1 " + std::to_string(status_code) + " " + GetHttpStatusText(status_code) + "\r\n";
    response += "Content-Type: application/json\r\n";
    response += "Content-Length: " + std::to_string(json_body.length()) + "\r\n";
    response += "Connection: close\r\n";
    response += "Access-Control-Allow-Origin: *\r\n";
    response += "\r\n";
    response += json_body;

    send(client_socket, response.c_str(), (int)response.length(), 0);
}

void HttpLogServer::SendHtmlResponse(SOCKET client_socket, const std::string & html_body)
{
    std::string response = "HTTP/1.1 200 OK\r\n";
    response += "Content-Type: text/html\r\n";
    response += "Content-Length: " + std::to_string(html_body.length()) + "\r\n";
    response += "Connection: close\r\n";
    response += "\r\n";
    response += html_body;

    send(client_socket, response.c_str(), (int)response.length(), 0);
}

void HttpLogServer::SendErrorResponse(SOCKET client_socket, int error_code, const std::string & error_message)
{
    std::string response = "HTTP/1.1 " + std::to_string(error_code) + " " + GetHttpStatusText(error_code) + "\r\n";
    response += "Content-Type: text/plain\r\n";
    response += "Content-Length: " + std::to_string(error_message.length()) + "\r\n";
    response += "Connection: close\r\n";
    response += "\r\n";
    response += error_message;

    send(client_socket, response.c_str(), (int)response.length(), 0);
}

std::string HttpLogServer::GetHttpStatusText(int code)
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

void HttpLogServer::LogMessage(const std::string &message, const std::string & client_ip)
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

    // Очистка сообщения от непечатаемых символов
    std::string clean_message = SanitizeString(message);
    log_stream << clean_message;

    std::string log_entry = log_stream.str();

    // Запись в файл
    std::ofstream file_stream(log_file, std::ios::app);
    if (file_stream.is_open())
    {
        file_stream << log_entry << std::endl;
        file_stream.close();
    }

    // Вывод в консоль
    std::cout << log_entry << std::endl;
}

// Метод для очистки строки от непечатаемых символов
std::string HttpLogServer::SanitizeString(const std::string & input)
{
    std::string output;
    output.reserve(input.length());

    for (char c : input)
    {
        // Проверяем, является ли символ печатаемым
        if (static_cast<unsigned char>(c) >= 32 && static_cast<unsigned char>(c) < 127)
        {
            output += c;
        }
        else if (c == '\n' || c == '\r' || c == '\t')
        {
            // Сохраняем пробельные символы
            output += ' ';
        }
        else
        {
            // Заменяем непечатаемые символы на точку
            output += '.';
        }
    }

    return output;
}
