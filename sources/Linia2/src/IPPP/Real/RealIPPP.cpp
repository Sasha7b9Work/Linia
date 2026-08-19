// 2025/09/10 21:28:18 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "IPPP/Real/RealIPPP.h"
#include "IPPP/IDevice.h"
#include "IPPP/Emulator/EmulatorDevice.h"
#include "Communicator/GPIO/GPIO.h"
#include "IPPP/Keyboard/Keyboard.h"
#include "IPPP/Real/RealDevice.h"
#include "Utils/Timer.h"
#include "Communicator/SPI/SPI.h"


bool RealIPPP::IsChanBS(const Chan &ch) const
{
    if (ch.IsBS())
    {
        return true;
    }

    LOG_ERROR("Channel must be B or S, not %s", ch.Name());

    return false;
}



void RealIPPP::PeriodicTask()
{
    Keyboard::PeriodicTask();

    IDevice::impl->PeriodicTask();

    int data[NUMBER_ADC][POINTS_IN_SAMPLE_ADC];

    if (ReadData(data))
    {
        for (int i = 0; i < 5; i++)
        {
        }
    }
}


void RealIPPP::PressButtonStart()
{

}


void RealIPPP::PressButtonStop()
{

}


bool RealIPPP::ReadData(int data_out[NUMBER_ADC][POINTS_IN_SAMPLE_ADC])
{
    bool result = false;

    static bool prev = false;

    if (pinFIFO_FULL.GetState() && prev == false)
    {
        uint8 data[POINTS_IN_SAMPLE_ADC * NUMBER_ADC];

        SPI::ReadFPGA(data, sizeof(data));

        int *pointer = (int *)data_out;

        for (size_t i = 0; i < sizeof(data); i++)
        {
            *pointer = data[i];
            pointer++;
        }

        result = true;
    }

    prev = pinFIFO_FULL.GetState();

    return result;
}


void RealIPPP::Pause()
{
    volatile int i = 0;

    for (i = 0; i < 1000; i += 1)
    {
    }
}
