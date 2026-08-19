// 2026/08/18 17:10:34 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once
#include "IPPP/Real/Chips.h"


#define SPI_DEVICE "/dev/spidev0.0"
#define SPI_SPEED  1000000
#define SPI_CHIP   "gpiochip3"


namespace SPI
{
    void Init();
    void DeInit();
    bool IsAvailability();
    bool IsReady();

    // Чтение из FPGA. Используется аппаратный SPI
    bool ReadFPGA(uint8 *data, size_t length);

    // Запись в динамические ЦАПы. Используется программный SPI.
    // Возвращает false, если недопустимый номер DAC (не управляется с Orange Pi)
    bool WriteDynamicDAC(DAC::E, uint16 value);
}
