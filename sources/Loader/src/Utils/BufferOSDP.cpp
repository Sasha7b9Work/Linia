// (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "Utils/BufferOSDP.h"
#include <cstdlib>
#include <cstring>


BufferOSDP::BufferOSDP(int new_capacity) : buffer(nullptr), size(0)
{
    Allocate(new_capacity);
}


BufferOSDP::~BufferOSDP()
{
    Free();
}


void BufferOSDP::Free()
{
    std::free(buffer);
    buffer = nullptr;
    size = 0;
    capacity = 0;
}


void BufferOSDP::Allocate(int new_capacity)
{
    if (new_capacity <= 0) return;

    // Сохраняем указатель на старые данные и их размер
    char *old_buffer = buffer;
    int old_size = size;

    // Выделяем новый буфер
    char *new_buffer = (char *)std::malloc((size_t)new_capacity);
    if (!new_buffer)
    {
        // Ошибка выделения — оставляем старый буфер
        return;
    }

    // Обнуляем новый буфер
    std::memset(new_buffer, 0, (size_t)new_capacity);

    // Копируем только реальные данные (size, а не capacity)
    if (old_buffer && old_size > 0)
    {
        std::memcpy(new_buffer, old_buffer, (size_t)old_size);
    }

    // Освобождаем старый буфер
    std::free(old_buffer);

    // Обновляем состояние
    buffer = new_buffer;
    capacity = new_capacity;
    // size остаётся прежним
}


void BufferOSDP::Append(char byte)
{
    if (!buffer)
    {
        Allocate(1024);  // защита от nullptr
        if (!buffer) return;
    }

    if (IsFull())
    {
        Allocate(capacity + 1024);
        if (IsFull()) return;  // если не удалось расширить
    }

    buffer[size++] = byte;
}


bool BufferOSDP::IsFull() const
{
    return (size >= capacity);
}


bool BufferOSDP::IsEmpty() const
{
    return size == 0;
}


const char &BufferOSDP::operator[](int num) const
{
    return buffer[num];
}


void BufferOSDP::RemoveFirst(int num_bytes)
{
    if (num_bytes <= 0) return;

    if (num_bytes >= size)
    {
        size = 0;
    }
    else
    {
        std::memmove(buffer, buffer + num_bytes, (size_t)(size - num_bytes));
        size -= num_bytes;
    }
}


int BufferOSDP::FirstPosition(char symbol)
{
    for (int i = 0; i < size; i++)
    {
        if (buffer[i] == symbol)
        {
            return i;
        }
    }

    return -1;
}


pchar BufferOSDP::Data(int pos) const
{
    if (pos < 0 || pos >= size) return nullptr;  // защита
    return buffer + pos;
}
