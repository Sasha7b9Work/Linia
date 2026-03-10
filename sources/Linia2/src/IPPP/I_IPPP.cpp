// 2026/03/10 16:47:57 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "IPPP/I_IPPP.h"
#include "IPPP/IDevice.h"
#include "IPPP/Real/RealDevice.h"
#include "IPPP/Keyboard/Keyboard.h"
#include "Communicator/GPIO/GPIO.h"
#include "IPPP/Real/RealIPPP.h"


I_IPPP *I_IPPP::impl = nullptr;


void I_IPPP::Create()
{
#ifdef EMULATOR_ENABLED



#else

    impl = new RealIPPP();

    IDevice::impl = new RealDevice();

    IDevice::impl->Init();

    pinREQ_RD.Set(false);    // Это состояние означает, что чтение не нужно

    Keyboard::Init();

#endif
}
