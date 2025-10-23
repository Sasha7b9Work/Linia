// 2025/10/23 11:56:23 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Utils/RingBuffer.h"


void RingBuffer::Push(uint8 byte)
{
    buffer[in++] = byte;

    if (in == SIZE)
    {
        in = 0;
    }
}


uint8 RingBuffer::Pop()
{
    uint8 result = buffer[out++];

    if (out == SIZE)
    {
        out = 0;
    }

    return result;
}


bool RingBuffer::IsEmpty() const
{
    return in == out;
}


void RingBuffer::Get(Buffer1024 &result)
{
    while (!IsEmpty())
    {
        result.Append(Pop());
    }
}
