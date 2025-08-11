// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "ComPortCommunicator.h"

namespace Log
{
    void AddString(char *format, ...);
}

ComPortCommunicator::ComPortCommunicator(uint16_t baudrate) 
    : baud_rate(baudrate), uart_ready(false)
{
}

ComPortCommunicator::~ComPortCommunicator()
{
    Disconnect();
}

bool ComPortCommunicator::Initialize()
{
    if (is_initialized)
        return true;
        
    // TODO: Инициализация UART
    // HAL_UART::Init(baud_rate);
    uart_ready = true;
    is_initialized = true;
    
    Log::AddString("UART Communicator initialized, baudrate: %d", baud_rate);
    return true;
}

bool ComPortCommunicator::SendData(const uint8_t* data, uint16_t size)
{
    if (!is_initialized || !uart_ready)
        return false;
        
    // TODO: Реализация передачи по UART
    // return HAL_UART::Transmit(const_cast<uint8_t*>(data), size);
    return true;
}

bool ComPortCommunicator::ReceiveData(uint8_t* buffer, uint16_t size, uint16_t timeout_ms)
{
    if (!is_initialized || !uart_ready)
        return false;
        
    // TODO: Реализация приема по UART
    // return HAL_UART::Receive(buffer, size, timeout_ms);
    return true;
}

bool ComPortCommunicator::IsConnected()
{
    return is_initialized && uart_ready;
}

void ComPortCommunicator::Disconnect()
{
    uart_ready = false;
    is_initialized = false;
}
