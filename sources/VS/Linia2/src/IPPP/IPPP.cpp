// 2025/09/07 16:36:30 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "IPPP/IPPP.h"
#include "Device/Device.h"
#include "DeviceTest/DeviceEmulator.h"


void IPPP::Init()
{
    g_device = new DeviceEmulator();

    g_device->Init();
}


void IPPP::Start()
{

}


void IPPP::Stop()
{

}
