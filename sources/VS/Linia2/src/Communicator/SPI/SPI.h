#pragma once


namespace SPI
{
    void Init();
    void DeInit();

    bool WriteDynamicDAC(int number_DAC, uint16_t value);

    bool SetSpeed(uint32_t speedHz);
    bool SetMode(uint8_t mode);

    bool IsReady();
    uint32_t GetSpeed();
    uint8_t GetMode();
}
