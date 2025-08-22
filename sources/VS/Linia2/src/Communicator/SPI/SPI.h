#pragma once


namespace SPI
{
    void Init();
    void DeInit();
    
    // Основные функции для динамических DAC
    bool WriteDynamicDAC1(uint16_t value);
    bool WriteDynamicDAC2(uint16_t value);
    
    // Настройки
    bool SetSpeed(uint32_t speedHz);
    bool SetMode(uint8_t mode);
    
    // Состояние
    bool IsReady();
    uint32_t GetSpeed();
    uint8_t GetMode();
    
    // Диагностические функции для Orange Pi
    void TestGPIO();
    void DiagnoseSPI();
    bool CheckSPIDevice(const char* device_path = "/dev/spidev1.0");
    void PrintSystemInfo();
}
