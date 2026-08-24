// 2026/08/24 16:25:48 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "ServerHTTP.h"
#include <string>
#include <iostream>


// Глобальный указатель на сервер для обработки сигналов
HttpLogServer *global_server = nullptr;

#ifndef _WIN32
void SignalHandler(int signal)
{
    if (global_server)
    {
        std::cout << "\nReceived signal " << signal << ", stopping server..." << std::endl;
        global_server->Stop();
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
    if (!server.Start())
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
                server.Stop();
                break;
            }
        }
#endif
    }

    return 0;
}
