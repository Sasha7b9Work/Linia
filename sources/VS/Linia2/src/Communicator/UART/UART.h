#pragma once


#define UART_DEVICE   "/dev/ttyS6"
#define UART_BAUDRATE 115200


namespace UART
{
    typedef void (*ReceivedCallback)(uint8 byte);

    void Init();
    void DeInit();

    bool Open();
    void Close();
    bool SendByte(uint8 byte);
    bool SendBuffer(const void *buffer, int size);
    void SetCallback(ReceivedCallback callback);
    void Flush();

    bool IsReady();
    const char *GetMode();
}
