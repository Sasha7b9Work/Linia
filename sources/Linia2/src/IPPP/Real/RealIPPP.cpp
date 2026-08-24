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
        for (int i = 0; i < POINTS_IN_SAMPLE_ADC; i++)
        {
            uint16 data[5];

            SPI::ReadFPGA((uint8 *)data, sizeof(uint16) * 5);

            for (int num_dac = 0; num_dac < 4; num_dac++)
            {
                data_out[num_dac][i] = data[num_dac];
            }
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
