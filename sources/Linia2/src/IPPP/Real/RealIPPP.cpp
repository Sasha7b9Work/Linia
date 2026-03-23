// 2025/09/10 21:28:18 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "IPPP/Real/RealIPPP.h"
#include "IPPP/IDevice.h"
#include "IPPP/Emulator/EmulatorDevice.h"
#include "Communicator/GPIO/GPIO.h"
#include "IPPP/Keyboard/Keyboard.h"
#include "IPPP/Real/RealDevice.h"


bool RealIPPP::IsChanBS(const Chan &ch) const
{
    if (ch.IsBS())
    {
        return true;
    }

    LOG_ERROR("Channel must be B or S, not %s", ch.Name());

    return false;
}



void RealIPPP::Update()
{
    Keyboard::Update();

    IDevice::impl->Update();
}


void RealIPPP::PressButtonStart()
{

}


void RealIPPP::PressButtonStop()
{

}


bool RealIPPP::ReadData(
    std::array<int, MAX_NUMBER_POINTS> &data1,
    std::array<int, MAX_NUMBER_POINTS> &data2,
    std::array<int, MAX_NUMBER_POINTS> &data3,
    std::array<int, MAX_NUMBER_POINTS> &data4
)
{
    std::reference_wrapper<std::array<int, MAX_NUMBER_POINTS>> data[4] =
    {
        data1,
        data2,
        data3,
        data4
    };

    if (pinFIFO_FULL.Get())
    {
        return false;                       // Читать нечего - выходим
    }

    static PinIn *pins[4] =
    {
        &pinDAT_F0,
        &pinSPI_MOSI,
        &pinDAT_F2
//        &pinDAT_F3
    };

    uint num_point = 0;

    while (!pinFIFO_FULL.Get())             // Продолжаем, пока не опустеет буфер передатчика
    {
        int val[4] = { 0, 0, 0, 0 };

        for (int i = 0; i < 18; i++)        // Читаем 18 бит каждого из четырёх значений АЦП
        {
            pinREQ_RD.Set(true);

            Pause();

            for (int bit = 0; bit < 4; bit++)
            {
                val[bit] <<= 1;

                if (pins[bit]->Get())
                {
                    val[bit] |= 1;
                }
            }

            pinREQ_RD.Set(false);
        }

        for (int i = 0; i < 4; i++)
        {
            if (num_point < MAX_NUMBER_POINTS)
            {
                data[i].get()[num_point] = val[i];
            }
            else
            {
                LOG_ERROR("Количество точек превышает размер массива");
            }
        }

        ++num_point;
    }

    return true;
}


void RealIPPP::Pause()
{
    volatile int i = 0;

    for (i = 0; i < 1000; i += 1)
    {
    }
}
