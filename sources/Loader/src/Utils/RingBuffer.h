// 2025/10/15 11:13:53 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once


class RingBuffer
{
public:

    void Append(uint8);

    bool IsEmpty() const;

    uint8 Pop();

    // Возвращает количество хранимых байт
    int Size() const;

private:

    static const int CAPACITY = 8 * 1024;

    uint8 buffer[CAPACITY];
    int in_index = 0;           // Сюда будет записан следующий символ
    int out_index = 0;          // Отсюда будет считан следующий символ
    int count = 0;
};
