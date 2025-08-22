#pragma once


namespace SPI
{
    void Init();
    void DeInit();

    // Основные функции для динамических DAC
    // \todo Переделать реализации. Функции не должны быть одинаковыми, а должны вызывать третью функцию с параметром.
    // Ещё лучше переделать к виду WriteDynamicDAC1(number_DAC, uint16_t value) - так уменьшается перегрузка интерфейса *.h-файла
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
    bool CheckSPIDevice(const char *device_path = "/dev/spidev1.0");
    void PrintSystemInfo();
}
