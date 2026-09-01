// 2025/09/10 21:36:34 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "IPPP/Emulator/EmulatorIPPP.h"
#include "Application.h"


void EmulatorIPPP::PeriodicTask()
{
    if (in_process_measuring)
    {
        int data_dac[NUMBER_ADC][POINTS_IN_SAMPLE_ADC];
        int data_code[POINTS_IN_SAMPLE_ADC];

        if (ReadData(data_dac, data_code))
        {
        }
    }
}


bool EmulatorIPPP::ReadData(int [NUMBER_ADC][POINTS_IN_SAMPLE_ADC], int [POINTS_IN_SAMPLE_ADC])
{
    return false;
}
