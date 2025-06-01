// 2024/08/30 14:42:00 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "DataBase/Server/TaskAnswer.h"


namespace Server
{
    template<typename T>
    class Queue
    {
    public:
        void Push(T *task)
        {
            mutex.lock();
            tasks.push(task);
            mutex.unlock();
        }
        uint Size()
        {
            mutex.lock();
            uint result = tasks.size();
            mutex.unlock();

            return result;
        }
        T *GetFirst()
        {
            if (Size())
            {
                mutex.lock();
                T *result = tasks.front();
                tasks.pop();
                mutex.unlock();
                return result;
            }

            return nullptr;
        }
    private:
        std::queue<T *> tasks;
        std::mutex mutex;
    };
}
