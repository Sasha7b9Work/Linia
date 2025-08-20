// 2025/6/11 18:48:07 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include <cstdint>


namespace Comm
{
    void Init();
    
    // Функции для работы с динамическими DAC
    bool WriteDynamicDAC1(uint16_t value);
    bool WriteDynamicDAC2(uint16_t value);
    
    // Настройки SPI
    bool SetSPISpeed(uint32_t speedHz);
    bool SetSPIMode(uint8_t mode);
    
    // Проверка состояния
    bool IsSPIReady();
}
