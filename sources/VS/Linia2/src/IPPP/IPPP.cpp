// 2025/09/07 16:36:30 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "IPPP/IPPP.h"
#include "Device/Device.h"
#include "DeviceTest/DeviceEmulator.h"
#include "Communicator/GPIO/GPIO.h"


namespace IPPP
{
    static void Pause();
}


void IPPP::Init()
{
    g_device = new DeviceEmulator();

    g_device->Init();

    pinREQ_RD.Set(false);    // Это состояние означает, что чтение не нужно
}


void IPPP::Start()
{

}


void IPPP::Stop()
{

}


bool IPPP::ReadData(std::vector<int> (&data)[4])
{
    if (pinFIFO_FULL.Get())
    {
        return false;                       // Читать нечего - выходим
    }

    static PinIn *pins[4] =
    {
        &pinDAT_F0,
        &pinDAT_F1,
        &pinDAT_F2,
        &pinDAT_F3
    };

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
            data[i].push_back(val[i]);
        }
    }

    return true;
}


void IPPP::Pause()
{
    volatile int i = 0;

    for (i = 0; i < 1000; i += 1)
    {
    }
}
