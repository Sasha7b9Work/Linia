// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "defines.h"

class Communicator
{
public:
    virtual ~Communicator() = default;
    
    virtual bool Initialize() = 0;
    virtual bool SendData(const uint8_t* data, uint16_t size) = 0;
    virtual bool ReceiveData(uint8_t* buffer, uint16_t size, uint16_t timeout_ms = 1000) = 0;
    virtual bool IsConnected() = 0;
    virtual void Disconnect() = 0;
    
protected:
    bool is_initialized = false;
};
