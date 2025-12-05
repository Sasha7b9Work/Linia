// 2025/11/27 15:47:35 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Device/Device.h"
#include "Device/Commutator.h"
#include "Device/FPGA.h"
#include "Device/Sources.h"
#include "Device/Channels.h"


void Device::Init()
{
    FPGA::Init();

    Commutator::Init();

    Source50V::Init();

    Source3kV::Init();

    ChanB::Init();

    ChanS::Init();

    ChanC::Init();
}


void Device::Update()
{
    Commutator::Update();
}
