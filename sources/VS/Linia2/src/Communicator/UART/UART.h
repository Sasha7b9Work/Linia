#pragma once


#define uart_device "/dev/ttyS6"


namespace UART {
    typedef void (*ReceivedCallback)(uint8 byte);

    void Init();
    void DeInit();

    bool Open(int baudrate, const char *mode);
    void Close();
    bool SendByte(uint8 byte);
    bool SendBuffer(const uint8 *buffer, int size);
    void SetCallback(ReceivedCallback callback);
    void Flush();

    bool IsReady();
    int GetBaudrate();
    const char *GetMode();
}
