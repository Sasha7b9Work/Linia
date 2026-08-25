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
#include "GUI/PageDebug/PanelRight.h"


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

    int data_dac[NUMBER_ADC][POINTS_IN_SAMPLE_ADC];
    int data_code[POINTS_IN_SAMPLE_ADC];

    if (ReadData(data_dac, data_code))
    {
        LOG_WRITE("Data reading is ok");

        for (int i = 0; i < NUMBER_ADC; i++)
        {
            ThePanelRight->data[i]->SetData(data_dac[i]);
        }

        ThePanelRight->data[NUMBER_ADC]->SetData(data_code);
    }
}


void RealIPPP::PressButtonStart()
{

}


void RealIPPP::PressButtonStop()
{

}


bool RealIPPP::ReadData(int data_dac[NUMBER_ADC][POINTS_IN_SAMPLE_ADC], int data_code[POINTS_IN_SAMPLE_ADC])
{
    bool result = false;

    static bool prev = false;

    if (pinFIFO_FULL.GetState() && prev == false)
    {
        for (int i = 0; i < POINTS_IN_SAMPLE_ADC; i++)
        {
            uint16 data[5];

            SPI::ReadFPGA((uint8 *)data, NUMBER_ADC * 2 + 1);

            for (int num_dac = 0; num_dac < 4; num_dac++)
            {
                data_dac[num_dac][i] = data[num_dac];
            }

            data_code[i] = (uint8)data[4];
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
