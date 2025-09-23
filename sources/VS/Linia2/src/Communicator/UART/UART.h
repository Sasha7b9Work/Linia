#pragma once


#define UART_DEVICE   "/dev/ttyS6"
#define UART_BAUDRATE 115200
#define UART_MODE     "8N1"


namespace UART
{
    bool Init(void (*recv_callback)(uint8));

    void DeInit();

    bool SendByte(uint8);

    // Отправка буфера данных через UART порциями по 32 байта
    bool SendBuffer(const void *buffer, int size);

    void Flush();
}
