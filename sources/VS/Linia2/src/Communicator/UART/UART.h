#pragma once

#include <cstdint>

namespace UART {
    typedef void (*ReceivedCallback)(uint8_t byte);
    
    void Init();
    void DeInit();
    
    bool Open(int baudrate, const char* mode);
    void Close();
    bool SendByte(uint8_t byte);
    bool SendBuffer(const uint8_t* buffer, int size);
    void SetCallback(ReceivedCallback callback);
    void Flush();
    
    bool IsReady();
    int GetBaudrate();
    const char* GetMode();
}