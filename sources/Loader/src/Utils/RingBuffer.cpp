// 2025/10/15 11:19:21 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "Utils/RingBuffer.h"


void RingBuffer::Append(uint8 byte)
{
    buffer[in_index] = byte;
    in_index = (in_index + 1) % CAPACITY;
    count++;
}


bool RingBuffer::IsEmpty() const
{
    return count == 0;
}


uint8 RingBuffer::Pop()
{
    if (IsEmpty())
    {
        return 0;
    }

    uint8 result = buffer[out_index];
    out_index = (out_index + 1) % CAPACITY;
    count--;
    return result;
}


int RingBuffer::Size() const
{
    return count;
}
