// 2025/09/10 21:36:34 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "IPPP/Emulator/EmulatorIPPP.h"
#include "Application.h"


void EmulatorIPPP::Update()
{
    if (in_process_measuring)
    {
        std::array<int, 100> data1;
        std::array<int, 100> data2;
        std::array<int, 100> data3;
        std::array<int, 100> data4;

        if (ReadData(data1, data2, data3, data4))
        {
            TheApp->OnReadData(data1, data2, data3, data4);
        }
    }
}


void EmulatorIPPP::PressButtonStart()
{
    in_process_measuring = true;
}


void EmulatorIPPP::PressButtonStop()
{
    in_process_measuring = false;
}


bool EmulatorIPPP::ReadData(
    std::array<int, 100> &/*data1*/,
    std::array<int, 100> &/*data2*/,
    std::array<int, 100> &/*data3*/,
    std::array<int, 100> &/*data4*/
)
{
    return false;
}
