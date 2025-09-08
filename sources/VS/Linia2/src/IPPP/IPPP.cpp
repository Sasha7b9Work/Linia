// 2025/09/07 16:36:30 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "IPPP/IPPP.h"
#include "Device/Device.h"
#include "DeviceTest/DeviceEmulator.h"
#include "Communicator/GPIO/GPIO.h"


namespace IPPP
{
    static void WaitForNS(uint64 &counter, uint timeNextNS);

    struct Timer
    {
        uint64 counter;

        void Start()
        {
            counter = 0;
        }

        // Ожидать, пока счётчик counter увеличится до timeNextNS
        void WatiFormNS(uint64 /*timeNextNS*/)
        {

        }
    };
}


void IPPP::Init()
{
    g_device = new DeviceEmulator();

    g_device->Init();

    pinREQ_RD.Set(true);    // Это состояние означает, что чтение не нужно
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

    Timer timer;

    uint64 time_next = 500;                 // В это время должен сработать следующий счётчик

    pinREQ_RD.Set(false);                   // Сообщаем ПЛИС-ке, что будем читать данные

    timer.Start();

    while (!pinFIFO_FULL.Get())             // Продолжаем, пока не опустеет буфер передатчика
    {
        int val[4] = { 0, 0, 0, 0 };

        for (int i = 0; i < 18; i++)        // Читаем 18 бит числа
        {
            timer.WatiFormNS(time_next);    // Ждём, пока передатчик выставит следующие четыре бита

            for (int num_bit = 0; num_bit < 4; num_bit++)
            {
                bool bit = pins[num_bit]->Get();

                val[num_bit] <<= 1;

                if (bit)
                {
                    val[num_bit] |= 1;
                }
            }
        }

        for (int i = 0; i < 4; i++)
        {
            data[i].push_back(val[i]);
        }

        time_next += 1000;                  // Следующие данные будем читать через 1 мкс.
    }

    pinREQ_RD.Set(true);

    return true;
}
