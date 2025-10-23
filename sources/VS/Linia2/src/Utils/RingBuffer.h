// 2025/10/23 11:49:23 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Utils/Buffer.h"


class RingBuffer
{
public:
    void Push(uint8);
    void Get(Buffer1024 &);
private:
    static const int SIZE = 1024;
    uint8 buffer[SIZE];
    int in;                 // Сюда ложим очередной байт
    int out;                
};
