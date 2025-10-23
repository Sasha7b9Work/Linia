// 2025/10/23 11:44:22 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "IPPP/SCPI/SCPI.h"
#include "Utils/RingBuffer.h"
#include "Utils/Buffer.h"


namespace SCPI
{
    static RingBuffer ring_buffer;
    static Buffer1024 buffer1024;
    class BufferSCPI
    {
    public:
        void Append(Buffer1024 &);
        bool Update();
    private:
        static const int SIZE = 1024;
        uint8 buffer[SIZE];
        int pointer = 0;
        bool ExistMessage();
        pchar GetMessage();
        void RemoveMessage();
        // Возвращает первую встреченную позицию символа
        int FindSymbol(char) const;
        void Clear();
        int Size() const;
    } buffer;

    extern StructSCPI head[];

    static bool Update(pchar, StructSCPI *);
}


void SCPI::OnEventCallback(uint8 byte)
{
    char symbol = (char)byte;

    ring_buffer.Push((char)std::toupper((int)symbol));
}


void SCPI::Update()
{
    ring_buffer.Get(buffer1024);

    if (buffer1024.Size())
    {
        buffer.Append(buffer1024);

        while (buffer.Update())
        {

        }
    }
}


void SCPI::BufferSCPI::Append(Buffer1024 &in)
{
    std::memcpy(buffer, in.DataConst(), (size_t)in.Size());
    pointer += in.Size();
    in.Clear();
}


bool SCPI::BufferSCPI::Update()
{
    if (ExistMessage())
    {
        pchar message = GetMessage();

        bool result = SCPI::Update(message, SCPI::head);

        RemoveMessage();

        return result;
    }

    return false;
}


bool SCPI::BufferSCPI::ExistMessage()
{
    if (Size() == 0)
    {
        return false;
    }

    int pos = FindSymbol(':');

    if (pos < 0)
    {
        Clear();

        return false;
    }

    if (pos > 0)
    {
        std::memmove(buffer, buffer + pos, (size_t)(Size() - pos));
        pointer -= pos;
    }

    return FindSymbol('\0') > 0;
}


pchar SCPI::BufferSCPI::GetMessage()
{
    return (pchar)buffer;
}


int SCPI::BufferSCPI::FindSymbol(char symbol) const
{
    for (int i = 0; i < pointer; i++)
    {
        if ((char)buffer[i] == symbol)
        {
            return i;
        }
    }

    return -1;
}


void SCPI::BufferSCPI::Clear()
{
    pointer = 0;
}


int SCPI::BufferSCPI::Size() const
{
    return pointer;
}


void SCPI::BufferSCPI::RemoveMessage()
{
    int pos = FindSymbol('\0');

    if (pos >= 0)
    {
        std::memmove(buffer, buffer + pos + 1, (size_t)(pointer - pos + 1));
        pointer = pointer - pos - 1;
    }
}


bool SCPI::Update(pchar message, StructSCPI *handler)
{
    while (handler->begin != nullptr)
    {
        size_t len_msg = std::strlen(message);

        if (std::strlen(handler->begin) >= len_msg)
        {
            if (std::memcmp(handler->begin, message, len_msg) == 0)
            {
                if (handler->func)
                {
                    return handler->func(message + len_msg);
                }
                else if (handler->handler)
                {
                    return Update(message + len_msg, handler->handler);
                }
            }
        }

        handler++;
    }

    return false;
}