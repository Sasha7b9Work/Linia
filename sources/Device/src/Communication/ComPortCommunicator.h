// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Communicator.h"

class ComPortCommunicator : public Communicator
{
public:
    ComPortCommunicator(uint16_t baudrate = 115200);
    ~ComPortCommunicator();
    
    bool Initialize() override;
    bool SendData(const uint8_t* data, uint16_t size) override;
    bool ReceiveData(uint8_t* buffer, uint16_t size, uint16_t timeout_ms = 1000) override;
    bool IsConnected() override;
    void Disconnect() override;
    
private:
    uint16_t baud_rate;
    bool uart_ready;
};
