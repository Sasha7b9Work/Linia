// 2026/08/24 13:54:26 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "Communicator/ClientHTTP/ClientHTTP.h"
#include <rapidjson/rapidjson.h>
#include <iostream>
#include <sstream>
#include <chrono>
#include <cstring>
#include <map>

#ifdef _WIN32
    #include <windows.h>
    #define close closesocket
#else
    #include <sys/time.h>
#endif

// Инициализация статических членов
bool ClientHTTP::winsock_initialized = false;
std::mutex ClientHTTP::winsock_mutex;

ClientHTTP::ClientHTTP(const std::string &server_host, int server_port) :
    server_host(server_host), server_port(server_port)
{
    InitializeSockets();

    // Запуск рабочего потока
    worker = std::thread(&ClientHTTP::WorkerThread, this);
}

ClientHTTP::~ClientHTTP()
{
    running = false;
    queue_cv.notify_all();
    if (worker.joinable())
    {
        worker.join();
    }
}

void ClientHTTP::InitializeSockets()
{
#ifdef _WIN32
    std::lock_guard<std::mutex> lock(winsock_mutex);
    if (!winsock_initialized)
    {
        WSADATA wsa_data;
        if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0)
        {
            std::cerr << "Failed to initialize Winsock" << std::endl;
        }
        winsock_initialized = true;
    }
#endif
}

void ClientHTTP::CleanupSockets()
{
#ifdef _WIN32
    std::lock_guard<std::mutex> lock(winsock_mutex);
    if (winsock_initialized)
    {
        WSACleanup();
        winsock_initialized = false;
    }
#endif
}

void ClientHTTP::SendMessage(const std::string &message)
{
    SendLogMessage("INFO", message);
}

void ClientHTTP::SendLogMessage(const std::string &level, const std::string &message)
{
    std::map<std::string, std::string> empty_data;
    SendLogWithData(level, message, empty_data);
}

void ClientHTTP::SendLogWithData(const std::string &level, const std::string &message, const std::map<std::string, std::string> &additional_data)
{
    if (!enabled) return;

    // Создание JSON-сообщения
    std::string json_message = CreateJsonMessage(level, message, additional_data);

    // Добавление в очередь
    std::lock_guard<std::mutex> lock(queue_mutex);

    // Проверка размера очереди
    if (max_queue_size > 0 && message_queue.size() >= max_queue_size)
    {
        // Удаляем самое старое сообщение
        message_queue.pop();
        failed_count++;
    }

    message_queue.push(json_message);
    queue_cv.notify_one();
}

std::string ClientHTTP::CreateJsonMessage(const std::string &level, const std::string &message, const std::map<std::string, std::string> &additional_data)
{
    std::stringstream json_stream;
    json_stream << "{";
    json_stream << "\"level\":\"" << EscapeJson(level) << "\",";
    json_stream << "\"message\":\"" << EscapeJson(message) << "\",";
    json_stream << "\"timestamp\":" << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    // Добавление дополнительных данных
    if (!additional_data.empty())
    {
        json_stream << ",\"data\":{";
        bool first = true;
        for (const auto &pair : additional_data)
        {
            if (!first) json_stream << ",";
            json_stream << "\"" << EscapeJson(pair.first) << "\":\"" << EscapeJson(pair.second) << "\"";
            first = false;
        }
        json_stream << "}";
    }

    json_stream << "}";
    return json_stream.str();
}

std::string ClientHTTP::EscapeJson(const std::string &input)
{
    std::string output;
    output.reserve(input.length());

    for (char c : input)
    {
        switch (c)
        {
        case '"':  output += "\\\""; break;
        case '\\': output += "\\\\"; break;
        case '\b': output += "\\b"; break;
        case '\f': output += "\\f"; break;
        case '\n': output += "\\n"; break;
        case '\r': output += "\\r"; break;
        case '\t': output += "\\t"; break;
        default:
            if (c < 32)
            {
                char buffer[7];
                snprintf(buffer, sizeof(buffer), "\\u%04x", c);
                output += buffer;
            }
            else
            {
                output += c;
            }
        }
    }

    return output;
}

void ClientHTTP::SetEnabled(bool _enabled)
{
    enabled = _enabled;

    if (enabled)
    {
        queue_cv.notify_one();
    }
}

bool ClientHTTP::TestConnection()
{
    return SendHttpRequest("{\"test\":\"connection\"}");
}

size_t ClientHTTP::GetPendingMessagesCount()
{
    std::lock_guard<std::mutex> lock(queue_mutex);
    return message_queue.size();
}

size_t ClientHTTP::GetSentMessagesCount()
{
    return sent_count.load();
}

size_t ClientHTTP::GetFailedMessagesCount()
{
    return failed_count.load();
}

void ClientHTTP::SetMaxQueueSize(size_t max_size)
{
    std::lock_guard<std::mutex> lock(queue_mutex);
    max_queue_size = max_size;

    // Если очередь больше максимального размера, удаляем лишние
    while (max_queue_size > 0 && message_queue.size() > max_queue_size)
    {
        message_queue.pop();
        failed_count++;
    }
}

void ClientHTTP::WorkerThread()
{
    while (running)
    {
        std::unique_lock<std::mutex> lock(queue_mutex);

        // Ожидание сообщения или остановки
        queue_cv.wait(lock, [this]()
            {
                return !message_queue.empty() || !running;
            });

        if (!running && message_queue.empty())
        {
            break;
        }

        // Извлечение сообщения из очереди
        std::string message = message_queue.front();
        message_queue.pop();

        lock.unlock();

        // Отправка сообщения
        if (enabled)
        {
            if (SendHttpRequest(message))
            {
                sent_count++;
            }
            else
            {
                failed_count++;

                // В случае ошибки, возвращаем сообщение обратно в очередь
                std::lock_guard<std::mutex> requeue_lock(queue_mutex);
                if (max_queue_size == 0 || message_queue.size() < max_queue_size)
                {
                    message_queue.push(message);
                }

                // Небольшая задержка при ошибке
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
        }
    }
}

bool ClientHTTP::SendHttpRequest(const std::string &body)
{
#ifdef _WIN32
    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET)
    {
        return false;
    }
#else
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        return false;
    }
#endif

    // Разрешение имени хоста
    struct addrinfo hints, *result = nullptr;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    std::string port_str = std::to_string(server_port);

    if (getaddrinfo(server_host.c_str(), port_str.c_str(), &hints, &result) != 0)
    {
#ifdef _WIN32
        closesocket(sock);
#else
        close(sock);
#endif
        return false;
    }

    // Установка таймаута
#ifdef _WIN32
    DWORD timeout = 3000; // 3 секунды
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout));
    setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(timeout));
#else
    struct timeval timeout;
    timeout.tv_sec = 3;
    timeout.tv_usec = 0;
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
    setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));
#endif

    if (connect(sock, result->ai_addr, (int)result->ai_addrlen) < 0)
    {
        freeaddrinfo(result);
#ifdef _WIN32
        closesocket(sock);
#else
        close(sock);
#endif
        return false;
    }

    freeaddrinfo(result);

    // Формирование HTTP POST запроса
    std::stringstream request;
    request << "POST /log HTTP/1.1\r\n";
    request << "Host: " << server_host << ":" << server_port << "\r\n";
    request << "Content-Type: application/json\r\n";
    request << "Content-Length: " << body.length() << "\r\n";
    request << "Connection: close\r\n";
    request << "\r\n";
    request << body;

    std::string request_str = request.str();

    // Отправка запроса
#ifdef _WIN32
    int send_result = send(sock, request_str.c_str(), (int)request_str.length(), 0);
#else
    ssize_t send_result = send(sock, request_str.c_str(), request_str.length(), 0);
#endif

    if (send_result <= 0)
    {
#ifdef _WIN32
        closesocket(sock);
#else
        close(sock);
#endif
        return false;
    }

    // Получение ответа
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));

#ifdef _WIN32
    int recv_result = recv(sock, buffer, sizeof(buffer) - 1, 0);
#else
    ssize_t recv_result = recv(sock, buffer, sizeof(buffer) - 1, 0);
#endif

#ifdef _WIN32
    closesocket(sock);
#else
    close(sock);
#endif

    return recv_result > 0;
}
