// 2026/09/01 13:40:55 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once
#include <thread>
#include <atomic>
#include <functional>
#include <chrono>

// Запуск функции в потоке


class BackgroundWorker
{
public:
    // Конструктор принимает функцию, которая будет выполняться в потоке
    explicit BackgroundWorker(std::function<void()> _task, int _intervalMs = 1) :
        task(_task),
        running(false),
        intervalMs(_intervalMs)
    {
    }

    ~BackgroundWorker()
    {
        Stop();
    }

    void Start()
    {
        LOG_WRITE("1");

        if (running.exchange(true))
        {
            return; // Уже запущен
        }

        LOG_WRITE("2");

        thread = std::thread([this]()
            {
                while (running.load(std::memory_order_acquire))
                {
                    LOG_WRITE("3");
                    if (task)
                    {
                        task();
                    }
                    std::this_thread::sleep_for(std::chrono::milliseconds(intervalMs));
                }
            });
    }

    void Stop()
    {
        if (!running.exchange(false))
        {
            return; // Уже остановлен
        }

        if (thread.joinable())
        {
            thread.join();
        }
    }

    // Проверка, запущен ли поток
    bool IsRunning() const
    {
        return running.load(std::memory_order_acquire);
    }

    // Установить интервал между вызовами
    void SetInterval(int ms)
    {
        intervalMs = ms;
    }

private:
    std::function<void()> task;
    std::thread thread;
    std::atomic<bool> running;
    int intervalMs;
};
