// 2026/08/24 13:15:51 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once

#include <string>
#include <functional>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <memory>
#include <map>

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <netdb.h>
    #include <unistd.h>
#endif

class LogClient {
public:
    LogClient(const std::string &server_host, int server_port);
    ~LogClient();

    // Отправка сообщения (асинхронная)
    void sendLog(const std::string &message);

    // Отправка сообщения с уровнем логирования
    void sendLog(const std::string &level, const std::string &message);

    // Отправка сообщения с дополнительными данными
    void sendLogWithData(const std::string &level, const std::string &message, const std::map<std::string, std::string> &additional_data);

    // Включение/отключение логирования
    void setEnabled(bool enabled);

    // Проверка соединения
    bool testConnection();

    // Получение статистики
    size_t getPendingMessagesCount();
    size_t getSentMessagesCount();
    size_t getFailedMessagesCount();

    // Установка максимального размера очереди (0 - без ограничения)
    void setMaxQueueSize(size_t max_size);

private:
    void workerThread();
    bool sendHttpRequest(const std::string &body);
    std::string createJsonMessage(const std::string &level, const std::string &message, const std::map<std::string, std::string> &additional_data);
    void initializeSockets();
    void cleanupSockets();

    std::string server_host;
    int server_port;
    std::atomic<bool> enabled;
    std::atomic<bool> running;

    std::queue<std::string> message_queue;
    std::mutex queue_mutex;
    std::condition_variable queue_cv;
    std::thread worker;

    // Статистика
    std::atomic<size_t> sent_count;
    std::atomic<size_t> failed_count;
    size_t max_queue_size;

    // Для Windows - инициализация Winsock
    static bool winsock_initialized;
    static std::mutex winsock_mutex;
};
