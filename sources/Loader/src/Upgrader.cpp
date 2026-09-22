// 2026/09/22 12:19:56 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "Upgrader.h"
#include "Hardware/HAL/HAL.h"
#include "Device/OPi5Plus/SCPI.h"
#include "Device/OPi5Plus/OPi5Plus.h"


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

    OPi5Plus::SCPI::Send(":UPGRADE:START 1");
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


void Upgrader::ErrorUpgrade()
{
    OPi5Plus::text_mode = true;
    current_block = -1;

    LOG_ERROR("Error upgrader");

    OPi5Plus::SCPI::Send(":UPGRADE:ERROR");
}
