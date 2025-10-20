// 2025/09/10 21:36:34 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "IPPP/Emulator/EmulatorIPPP.h"
#include "Application.h"


void EmulatorIPPP::Init()
{

}


void EmulatorIPPP::Update()
{
    if (in_process_measuring)
    {
        std::vector<int> data[4];

        if (ReadData(data))
        {
            Application::self->OnReadData(data);
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


bool EmulatorIPPP::ReadData(std::vector<int>(& /*data*/)[4])
{
    return false;
}
