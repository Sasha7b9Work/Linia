// 2025/10/23 11:49:23 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Utils/Buffer.h"


class RingBuffer
{
public:
    void Push(char);
    char Pop();
    void Get(Buffer1024 &);
    bool IsEmpty() const;
private:
    static const int SIZE = 1024;
    char buffer[SIZE];
    int in;                 // Сюда ложим очередной байт
    int out;                // А отсюда считываем
};
