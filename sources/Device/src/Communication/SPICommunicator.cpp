// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "SPICommunicator.h"

// Forward declarations для HAL_SPI1
namespace HAL_SPI1
{
    void Init();
    void Receive(void *buffer, int size);
    void Transmit(void *buffer, int size);
    void WaitCS();
}

// Подключение Log из common/Utils
namespace Log
{
    void AddString(char *format, ...);
}

SPICommunicator::SPICommunicator() : spi_ready(false)
{
}

SPICommunicator::~SPICommunicator()
{
    Disconnect();
}

bool SPICommunicator::Initialize()
{
    if (is_initialized)
        return true;
        
    HAL_SPI1::Init();
    spi_ready = true;
    is_initialized = true;
    
    Log::AddString("SPI Communicator initialized");
    return true;
}

bool SPICommunicator::SendData(const uint8_t* data, uint16_t size)
{
    if (!is_initialized || !spi_ready)
        return false;
        
    HAL_SPI1::Transmit(const_cast<void*>(static_cast<const void*>(data)), size);
    return true;
}

bool SPICommunicator::ReceiveData(uint8_t* buffer, uint16_t size, uint16_t timeout_ms)
{
    if (!is_initialized || !spi_ready)
        return false;
        
    HAL_SPI1::Receive(buffer, size);
    return true;
}

bool SPICommunicator::IsConnected()
{
    return is_initialized && spi_ready;
}

void SPICommunicator::Disconnect()
{
    spi_ready = false;
    is_initialized = false;
}
