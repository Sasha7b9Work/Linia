#pragma once


namespace SPI
{
    void Init();
    void DeInit();

    // Основные функции для динамических DAC
    bool WriteDynamicDAC(int number_DAC, uint16_t value);

    // Настройки
    bool SetSpeed(uint32_t speedHz);
    bool SetMode(uint8_t mode);

    // Состояние
    bool IsReady();
    uint32_t GetSpeed();
    uint8_t GetMode();
}
