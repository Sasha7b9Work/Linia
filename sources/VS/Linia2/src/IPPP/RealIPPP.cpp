// 2025/09/10 21:28:18 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "IPPP/RealIPPP.h"
#include "IPPP/Device/IDevice.h"
#include "IPPP/Device/DeviceEmulator.h"
#include "Communicator/GPIO/GPIO.h"
#include "IPPP/Device/Keyboard.h"


void RealIPPP::Init()
{
    IDevice::impl = new DeviceEmulator();

    IDevice::impl->Init();

    pinREQ_RD.Set(false);    // Это состояние означает, что чтение не нужно

    Keyboard::Init();
}


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
}


void RealIPPP::PressButtonStart()
{

}


void RealIPPP::PressButtonStop()
{

}


bool RealIPPP::ReadData(std::vector<int>(&data)[4])
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


void RealIPPP::Pause()
{
    volatile int i = 0;

    for (i = 0; i < 1000; i += 1)
    {
    }
}


void RealIPPP::SetPointCount(int /*count*/)
{

}


void RealIPPP::SetCircuitConnection(const Chan & /*ch*/, StateJack::E /*state*/)
{
}


void RealIPPP::SetTypeScan(TypeScan::E /*type*/)
{
}


void RealIPPP::ChanC_SourceRange(RangeU::E /*range*/)
{
//    SendCommand(":C:SOURCE:RANGE %s", RangeU(range).Name(RowRange(Chan::_C), false));
}


void RealIPPP::ChanC_MeasRange(RangeU::E /*range*/)
{
//    SendCommand(":C:MEAS:RANGE %s", RangeU(range).Name(RowRange(Chan::_C), false));
}


void RealIPPP::ChanC_MeasRange(RangeI::E /*range*/)
{
//    SendCommand(":C:MEAS:RANGE %s", RangeI(range).Name(RowRange(Chan::_C), false));
}


void RealIPPP::ChanC_LimitSourceU(int /*min*/, int /*max*/)
{
//    SendCommand(":C:SOURCE:LIMIT %d %d", min, max);
}


void RealIPPP::ChanBS_SourceMode(const Chan & /*ch*/, ModeSource::E /*mode*/)
{
//    if (IsChanBS(ch))
//    {
//        SendCommand(":%s:SOURCE:MODE %s", ch.Name(), ModeSource::Name(mode));
//    }
}


void RealIPPP::ChanBS_StepRange(const Chan & /*ch*/, RangeU::E /*range*/)
{
//    if (IsChanBS(ch))
//    {
//        SendCommand(":%s:STEP:RANGE %s",
//            ch.Name(),
//            RangeU(range).Name(RowRange(ch.value), false));
//    }
}


void RealIPPP::ChanBS_StepRange(const Chan & /*ch*/, RangeI::E /*range*/)
{
//    if (IsChanBS(ch))
//    {
//        SendCommand(":%s:STEP:RANGE %s",
//            ch.Name(),
//            RangeI(range).Name(RowRange(ch.value), false));
//    }
}


void RealIPPP::ChanBS_StepCount(const Chan & /*ch*/, int /*count*/)
{
//    if (IsChanBS(ch))
//    {
//        SendCommand(":%s:STEP:COUNT %d", ch.Name(), count);
//    }
}


void RealIPPP::ChanBS_StepValue(const Chan & /*ch*/, double /*value*/)
{
//    if (IsChanBS(ch))
//    {
//        SendCommand(":%s:STEP:VALUE %10e", ch.Name(), value);
//    }
}


void RealIPPP::ChaBS_Offset(const Chan & /*ch*/, double /*offset*/)
{
//    if (IsChanBS(ch))
//    {
//        SendCommand(":%s:OFFSET %10e", ch.Name(), offset);
//    }
}


void RealIPPP::ChanBS_MeasMode(const Chan & /*ch*/, ModeMeas::E /*mode*/)
{
//    if (IsChanBS(ch))
//    {
//        SendCommand(":%s:MEAS:MODE %s", ch.Name(), ModeMeas::Name(mode));
//    }
}


void RealIPPP::ChanBS_MeasRange(const Chan & /*ch*/, RangeU::E /*range*/)
{
//    if (IsChanBS(ch))
//    {
//        SendCommand(":%s:MEAS:RANGE %s",
//            ch.Name(),
//            RangeU(range).Name(RowRange(ch.value), false));
//    }
}


void RealIPPP::ChanBS_MeasRange(const Chan & /*ch*/, RangeI::E /*range*/)
{
//    if (IsChanBS(ch))
//    {
//        SendCommand(":%s:MEAS:RANGE %s",
//            ch.Name(),
//            RangeI(range).Name(RowRange(ch.value), false));
//    }
}


void RealIPPP::ChanBS_LimitRange(const Chan & /*ch*/, RangeU::E /*range*/)
{
//    if (IsChanBS(ch))
//    {
//        SendCommand(":%s:LIMIT:RANGE %s",
//            ch.Name(),
//            RangeU(range).Name(RowRange(ch.value), false));
//    }
}


void RealIPPP::ChanBS_LimitRange(const Chan & /*ch*/, RangeI::E /*range*/)
{
//    if (IsChanBS(ch))
//    {
//        SendCommand(":%s:LIMIT:RANGE %s",
//            ch.Name(),
//            RangeI(range).Name(RowRange(ch.value), false));
//    }
}


void RealIPPP::ChanBS_LimitThreshold(const Chan & /*ch*/, int /*threshold*/)
{
//    if (IsChanBS(ch))
//    {
//        SendCommand(":%s:LIMIT:THRESHOLD %d", ch.Name(), threshold);
//    }
}


void RealIPPP::SetPulseDuration(uint /*durationUS*/, bool /*genertaion_stump*/)
{
//    SendCommand(":PULSE:DURATION %u", durationUS);
//    SendCommand(":GENERATIONSTUP %d", genertaion_stump ? 1 : 0);
}


void RealIPPP::SetFirstQueue(const Chan & /*ch*/)
{
//    SendCommand(":FIRSTQUEUE %s", ch.Name());
}
