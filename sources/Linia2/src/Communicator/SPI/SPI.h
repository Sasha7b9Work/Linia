#pragma once


#define SPI_DEVICE "/dev/spidev0.0"
#define SPI_SPEED  1000000
#define SPI_CHIP   "gpiochip3"

// Аппаратные чипселекты SPI0
// CS0 - контакт 24 (GPIO1_B4)
// CS1 - контакт 26 (GPIO1_B5)
#define SPI_CS0 0  // SPI0_CS0_M2
#define SPI_CS1 1  // SPI0_CS1_M2


// \todo Для SPI нужно использовать 24 SPI0_CS0_M2 и 26 SPI0_CS1_M2. Программные чипселекты слишком долго выполняются

namespace SPI
{
    bool IsAvailability();

    void Init();
    void DeInit();

    bool WriteDynamicDAC(int number_DAC, uint16 value);

    bool IsReady();
}
