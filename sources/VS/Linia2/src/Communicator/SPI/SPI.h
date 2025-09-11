#pragma once


#define SPI_DEVICE "/dev/spidev0.0"
#define SPI_SPEED  100000
#define SPI_CHIP   "gpiochip3"


namespace SPI
{
    void Init();
    void DeInit();

    bool WriteDynamicDAC(int number_DAC, uint16 value);

    bool SetSpeed(uint speedHz);
    bool SetMode(uint8_t mode);

    bool IsReady();
    uint GetSpeed();
    uint8_t GetMode();
}
