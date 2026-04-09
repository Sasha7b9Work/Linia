#pragma once


#define SPI_DEVICE "/dev/spidev0.0"
#define SPI_SPEED  100000
#define SPI_CHIP   "gpiochip3"


namespace SPI
{
    bool IsAvailability();

    void Init();
    void DeInit();

    bool WriteDynamicDAC(int number_DAC, uint16 value);

    bool SetSpeed(uint speedHz);
    // Установка режима SPI (полярность и фаза тактового сигнала)
    // mode: режим SPI (0-3: 0=CPOL=0,CPHA=0; 1=CPOL=0,CPHA=1; 2=CPOL=1,CPHA=0; 3=CPOL=1,CPHA=1)
    bool SetMode(uint8 mode);

    bool IsReady();
    uint GetSpeed();
    uint8 GetMode();
}
