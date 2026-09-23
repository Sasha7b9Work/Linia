// 2026/09/22 12:19:56 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "Upgrader.h"
#include "Hardware/HAL/HAL.h"
#include "Device/OPi5Plus/SCPI.h"
#include "Device/OPi5Plus/OPi5Plus.h"
#include "Utils/GlobalFunctions.h"
#include <stm32f4xx_hal.h>


#ifdef WIN32
    #pragma warning(push)
    #pragma warning(disable : 4312)     // 'type cast': conversion from 'uint' to 'const void *' of greater size
#endif


namespace Upgrader
{
    static uint offset = 0;
    static int current_block = -1;  // Текущий принимаемый блок
    static int size_block = 0;      // Размер принимаемого блока
    static uint crc32_block = 0;    // Контрольная сумма принимаемого блока

    // Действия, который нужно сделать при возникновении ошибки
    static void ErrorUpgrade();
}


void Upgrader::BeginUpgrade()
{
    offset = 0;

    current_block = -1;

    // Команда начала обновления уже получена, стираем сектор, где будет храниться прошивка
    HAL_FLASH::Firmware::EraseSector();

    OPi5Plus::SCPI::Send(":UPGRADE:START");
}


void Upgrader::PeriodicTask()
{
    if (current_block == -1)        // Признак того, что не идём приём очередного блока
    {
        return;
    }

    if (HAL_USART1::BytesInBuffer() < size_block)
    {
        return;
    }

    BufferOSDP buffer(size_block);

    HAL_USART1::GetData(buffer);



    HAL_FLASH::Firmware::WriteBuffer(offset, buffer.Data(0), size_block);

    offset += size_block;

    OPi5Plus::text_mode = true;
}


void Upgrader::Start(int /*size*/, uint /*crc32*/)
{

}


void Upgrader::ReceiveBlock(int _num_block, int _size_block, uint _crc32_block)
{
    OPi5Plus::text_mode = false;

    if (_num_block - 1 != current_block)
    {
        ErrorUpgrade();
    }
    else
    {
        current_block = _num_block;
        size_block = _size_block;
        crc32_block = _crc32_block;
    }
}


void Upgrader::End(int _size, uint _crc32)
{
    if (_size != (int)offset)
    {
        ErrorUpgrade();
    }
    else if (_crc32 != GF::CalculateCRC32((const void *)HAL_FLASH::Firmware::Address(), (int)offset))
    {
        ErrorUpgrade();
    }
    else
    {
        uint crc32 = 0;

        do
        {
            crc32 = GF::CalculateCRC32((const void *)HAL_FLASH::Firmware::Address(), (int)offset);

            HAL_FLASH::EraseSector(0x08000000);     // /
            HAL_FLASH::EraseSector(0x08004000);     // | Стираем первые пять секторов для записи основной прошивки
            HAL_FLASH::EraseSector(0x08008000);     // |
            HAL_FLASH::EraseSector(0x0800C000);     // |
            HAL_FLASH::EraseSector(0x08010000);     // /

            HAL_FLASH::WriteBuffer(0x08000000, (const void *)HAL_FLASH::Firmware::Address(), (int)offset);

        } while (crc32 != GF::CalculateCRC32((const void *)0x08000000, (int)offset));

        OPi5Plus::SCPI::Send(":UPGRADE:END %d, %X", offset, _crc32);

        HAL_NVIC_SystemReset();
    }
}


void Upgrader::ErrorUpgrade()
{
    OPi5Plus::text_mode = true;
    current_block = -1;

    LOG_ERROR("Error upgrader");

    OPi5Plus::SCPI::Send(":UPGRADE:ERROR");
}


#ifdef WIN32
    #pragma warning(pop)
#endif
