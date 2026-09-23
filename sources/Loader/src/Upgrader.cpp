// 2026/09/22 12:19:56 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "Upgrader.h"
#include "Hardware/HAL/HAL.h"
#include "Device/OPi5Plus/SCPI.h"
#include "Device/OPi5Plus/OPi5Plus.h"
#include "Utils/GlobalFunctions.h"
#include "Hardware/Timer.h"
#include <stm32f4xx_hal.h>


#ifdef WIN32
    #pragma warning(push)
    #pragma warning(disable : 4312)     // 'type cast': conversion from 'uint' to 'const void *' of greater size
#endif


namespace Upgrader
{
    static uint offset = 0;
    static int current_block = -1;

    // Действия, который нужно сделать при возникновении ошибки
    static void ErrorUpgrade();
}


void Upgrader::BeginUpgrade()
{
    LOG_WRITE("Upgrader::BeginUpgrade()");

    offset = 0;

    current_block = -1;

    // Команда начала обновления уже получена, стираем сектор, где будет храниться прошивка
    HAL_FLASH::Firmware::EraseSector();

    OPi5Plus::SCPI::Send(":UPGRADE:START");
}


void Upgrader::ReceiveBlock(int _num_block, int _size_block, uint _crc32_block)
{
    if (_num_block - 1 != current_block)
    {
        ErrorUpgrade();

        return;
    }

    current_block = _num_block;

    OPi5Plus::_text_mode = false;

    BufferOSDP buffer(_size_block);

    OPi5Plus::SCPI::Send(":UPGRADE:HEAD %d %d %X", current_block, _size_block, _crc32_block);

    TimeMeterMS meter_full;

    int prev_bytes = 0;

    while (HAL_USART1::BytesInBuffer() < _size_block)
    {
        Timer::DelayMS(1);
        static TimeMeterMS meter;

        if (meter.ElapsedMS() > 2)
        {
//            LOG_WRITE("HAL_USART1::BytesInBuffer() = %d, time %d ms", HAL_USART1::BytesInBuffer(), meter_full.ElapsedMS());
            meter.Reset();
        }

        if ((prev_bytes != 0) &&
            (prev_bytes == HAL_USART1::BytesInBuffer()))
        {
//            LOG_WRITE("HAL_USART1::BytesInBuffer() = %d, time %d ms", HAL_USART1::BytesInBuffer(), meter_full.ElapsedMS());

            HAL_USART1::GetData(buffer);

            OPi5Plus::_text_mode = true;

            ErrorUpgrade();

            return;
        }

        prev_bytes = HAL_USART1::BytesInBuffer();
    }

    LOG_WRITE("HAL_USART1::BytesInBuffer() = %d", HAL_USART1::BytesInBuffer());

    HAL_USART1::GetData(buffer);

    LOG_WRITE("                      %d, %d, %d, %d, %d, %d, %d, %d, %d, %d",
        *buffer.Data(0),
        *buffer.Data(1),
        *buffer.Data(2),
        *buffer.Data(3),
        *buffer.Data(4),
        *buffer.Data(5),
        *buffer.Data(6),
        *buffer.Data(7),
        *buffer.Data(8),
        *buffer.Data(9));

    LOG_WRITE("                      %c, %c, %c, %c, %c, %c, %c, %c, %c, %c",
        (char)*buffer.Data(0),
        (char)*buffer.Data(1),
        (char)*buffer.Data(2),
        (char)*buffer.Data(3),
        (char)*buffer.Data(4),
        (char)*buffer.Data(5),
        (char)*buffer.Data(6),
        (char)*buffer.Data(7),
        (char)*buffer.Data(8),
        (char)*buffer.Data(9));

    OPi5Plus::_text_mode = true;

    HAL_FLASH::Firmware::WriteBuffer(offset, buffer.Data(0), _size_block);

    uint crc32 = GF::CalculateCRC32(buffer.Data(0), _size_block);

    LOG_WRITE("Confirmation receive content : num_block=%d, size_block=%d, crc32=%X", current_block, _size_block, crc32);

    OPi5Plus::SCPI::Send(":UPGRADE:CONTENT %d %d %X", current_block, _size_block, crc32);

    offset += _size_block;
}


void Upgrader::Start(int /*size*/, uint /*crc32*/)
{
    BeginUpgrade();
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
    OPi5Plus::_text_mode = true;
    current_block = -1;

    LOG_ERROR("Error upgrader");

    OPi5Plus::SCPI::Send(":UPGRADE:ERROR");
}


#ifdef WIN32
    #pragma warning(pop)
#endif
