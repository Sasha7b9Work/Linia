// 2025/09/22 13:39:17 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Hardware/HAL/HAL_PINS.h"


// œÀ»—


namespace FPGA
{
    void Init();

    namespace Reg
    {
        void SetLength(int num, uint length);
        void Write(int num, uint value);
    }
}
