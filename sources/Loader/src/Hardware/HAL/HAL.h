// (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once
#include "Utils/BufferOSDP.h"


struct CalibrationSettings;


#define ERROR_HANDLER() HAL::ErrorHandler(__FILE__, __LINE__)


namespace HAL
{
    void Init();

    void ErrorHandler(pchar, int);
};


namespace HAL_FLASH
{
    // Стереть сектор, в который будем сохранять прошивку перед обновлением
    void EraseSector(uint address);

    // Сюда передаётся смещение относительно начала сектора хранения прошивки
    void WriteBuffer(uint address, const void *buffer, int size);

    // Сюда передаётся смещение относительно начала сектора хранения прошивки
    void ReadBuffer(uint address, void *buffer, int size);

    namespace Firmware
    {
        // Стереть сектор, в который будем сохранять прошивку перед обновлением
        void EraseSector();

        // Сюда передаётся смещение относительно начала сектора хранения прошивки
        void WriteBuffer(uint offset, const void *buffer, int size);

        // Сюда передаётся смещение относительно начала сектора хранения прошивки
        void ReadBuffer(uint offset, void *buffer, int size);
    }
}


namespace HAL_TIM
{
    uint TimeMS();

    void Delay(uint timeMS);
};


namespace HAL_TIM2
{
    void Init();

    uint GetTicks();

    void StartMultiMeasurement();

    uint TimeUS();
};


namespace HAL_TIM3
{
    void Init();

    void StartIT(uint period);

    void StopIT();
};


// Взаимодействие с Orange Pi
namespace HAL_USART1
{
    void Init();

    void GetData(BufferOSDP &);

    void Transmit(const void *buffer, int size);

    void TransmitString(pchar);

    extern void *handle;       // UART_HandleTypeDef
}
