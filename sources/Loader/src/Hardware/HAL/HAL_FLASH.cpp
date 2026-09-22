// 2026/09/22 11:34:29 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include <stm32f4xx_hal.h>
#include <cstring>


#ifdef WIN32
    #pragma warning(push)
    #pragma warning(disable:4312)
#endif


namespace HAL_FLASH
{
    struct FlashSector {
        uint startAddress;
        uint size;
        uint sectorNumber;
    };

    static const FlashSector flashSectors[] = {
        {0x08000000, 16 * 1024, FLASH_SECTOR_0},
        {0x08004000, 16 * 1024, FLASH_SECTOR_1},
        {0x08008000, 16 * 1024, FLASH_SECTOR_2},
        {0x0800C000, 16 * 1024, FLASH_SECTOR_3},
        {0x08010000, 64 * 1024, FLASH_SECTOR_4},
        {0x08020000, 128 * 1024, FLASH_SECTOR_5},
        {0x08040000, 128 * 1024, FLASH_SECTOR_6},
        {0x08060000, 128 * 1024, FLASH_SECTOR_7},
        {0x08080000, 128 * 1024, FLASH_SECTOR_8},
        {0x080A0000, 128 * 1024, FLASH_SECTOR_9},
        {0x080C0000, 128 * 1024, FLASH_SECTOR_10},
        {0x080E0000, 128 * 1024, FLASH_SECTOR_11}
    };

    static uint GetSector(uint address)
    {
        for (const auto &sector : flashSectors)
        {
            if (address >= sector.startAddress &&
                address < (sector.startAddress + sector.size))
            {
                return sector.sectorNumber;
            }
        }
        return 0xFFFFFFFF; // Ошибка: адрес вне Flash
    }
}


void HAL_FLASH::EraseSector(uint address)
{
    FLASH_EraseInitTypeDef EraseInitStruct;
    uint32_t SectorError = 0;

    // Определяем номер сектора по адресу
    uint32_t sector = GetSector(address);
    if (sector == 0xFFFFFFFF)
    {
        return; // Неверный адрес
    }

    HAL_FLASH_Unlock();

    EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
    EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3; // 2.7V - 3.6V
    EraseInitStruct.Sector = sector;
    EraseInitStruct.NbSectors = 1;

    if (HAL_FLASHEx_Erase(&EraseInitStruct, &SectorError) != HAL_OK)
    {
        // Обработка ошибки: SectorError содержит номер проблемного сектора
    }

    HAL_FLASH_Lock();
}


void HAL_FLASH::WriteBuffer(uint address, const void *buffer, int size)
{
    const uint8_t *data = static_cast<const uint8_t *>(buffer);

    HAL_FLASH_Unlock();

    // Стираем флаги ошибок перед программированием
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_OPERR |
        FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR |
        FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);

    // Программируем побайтово
    for (int i = 0; i < size; i++)
    {
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE,
            address + i,
            data[i]) != HAL_OK)
        {
            // Ошибка программирования
            break;
        }
    }

    HAL_FLASH_Lock();
}


void HAL_FLASH::ReadBuffer(uint address, void *buffer, int size)
{
    // Прямое чтение из памяти (Flash отображён на адресное пространство)
    std::memcpy(buffer, (const void *)address, (uint)size);
}


#ifdef WIN32
    #pragma warning(pop)
#endif
