// 2026/03/10 16:47:57 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "IPPP/I_IPPP.h"
#include "IPPP/IDevice.h"
#include "IPPP/Real/RealDevice.h"
#include "IPPP/Real/PinsDevice.h"
#include "IPPP/Keyboard/Keyboard.h"


I_IPPP *I_IPPP::impl = nullptr;


void I_IPPP::Init()
{
#ifdef EMULATOR_ENABLED



#else

    IDevice::impl = new RealDevice();

    IDevice::impl->Init();

    pinREQ_RD.Set(false);    // Это состояние означает, что чтение не нужно

    Keyboard::Init();

#endif
}
