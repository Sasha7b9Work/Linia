// 2025/09/10 21:36:34 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "IPPP/Emulator/EmulatorIPPP.h"
#include "Application.h"


void EmulatorIPPP::PeriodicTask()
{
    if (in_process_measuring)
    {
        int data[NUMBER_ADC][POINTS_IN_SAMPLE_ADC];

        if (ReadData(data))
        {
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


bool EmulatorIPPP::ReadData(int [NUMBER_ADC][POINTS_IN_SAMPLE_ADC])
{
    return false;
}
