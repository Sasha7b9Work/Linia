// 2026/09/22 12:19:39 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once


namespace Upgrader
{
    void BeginUpgrade();

    void Start(int size, uint crc32);

    void ReceiveBlock(int num_block, int size_block, uint crc32_block);

    void End(int size, uint crc32);
}
