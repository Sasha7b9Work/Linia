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
        void Append(const Buffer1024 &);
        bool Update();
    private:
    } buffer;
}


void SCPI::OnEventCallback(uint8 byte)
{
    ring_buffer.Push(byte);
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


void SCPI::BufferSCPI::Append(const Buffer1024 &)
{

}


bool SCPI::BufferSCPI::Update()
{
    return false;
}
