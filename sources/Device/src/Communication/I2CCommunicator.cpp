// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "I2CCommunicator.h"

namespace Log
{
    void AddString(char *format, ...);
}

I2CCommunicator::I2CCommunicator(uint8_t device_address) 
    : address(device_address), i2c_ready(false)
{
}

I2CCommunicator::~I2CCommunicator()
{
    Disconnect();
}

bool I2CCommunicator::Initialize()
{
    if (is_initialized)
        return true;
        
    // TODO: Инициализация I2C
    // HAL_I2C::Init();
    i2c_ready = true;
    is_initialized = true;
    
    Log::AddString("I2C Communicator initialized, address: 0x%02X", address);
    return true;
}

bool I2CCommunicator::SendData(const uint8_t* data, uint16_t size)
{
    if (!is_initialized || !i2c_ready)
        return false;
        
    // TODO: Реализация передачи по I2C
    // return HAL_I2C::Transmit(address, const_cast<uint8_t*>(data), size);
    return true;
}

bool I2CCommunicator::ReceiveData(uint8_t* buffer, uint16_t size, uint16_t timeout_ms)
{
    if (!is_initialized || !i2c_ready)
        return false;
        
    // TODO: Реализация приема по I2C
    // return HAL_I2C::Receive(address, buffer, size);
    return true;
}

bool I2CCommunicator::IsConnected()
{
    return is_initialized && i2c_ready;
}

void I2CCommunicator::Disconnect()
{
    i2c_ready = false;
    is_initialized = false;
}
