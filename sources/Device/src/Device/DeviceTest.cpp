// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
// Тестовый файл для проверки новой архитектуры

#include "defines.h"
#include "Device/Device.h"
#include "Communication/SPICommunicator.h"
#include "Utils/CallbackManager.h"

// Простой тест компиляции
void CompileTest()
{
    Device device;
    SPICommunicator spi_comm;
    
    device.Initialize();
    device.SetCommunicator(&spi_comm);
    
    device.SetModeGenerator1V12();
    device.Start();
    device.Stop();
    
    CallbackManager::RegisterCallback(DeviceEvent::MODE_CHANGED, nullptr);
}
