// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/HAL/HAL_PINS.h"
#include "Hardware/Timer.h"
#include "Generator/Generator.h"
#include "Generator/MAX532.h"
#include "Connector/Device/Interface_d.h"
#include "Connector/Device/Value_.h"
#include "Generator/FPGA.h"
#include "Device/Device.h"
#include "Communication/SPICommunicator.h"
#include "Utils/CallbackManager.h"


#ifndef WIN32
    #if __ARMCC_VERSION != 6210000
        // �� ������ ������� ������������ �� �����������
        // �� �� 6.23 �� Keil 5.42a �� ����������� ��-�� new, malloc
        #error "Requires ARM Compiler V6.21 from uVision 5.39"
    #endif
#endif

// Глобальные объекты
Device device;
SPICommunicator spi_communicator;

// Callback функции
void OnModeChanged(DeviceEvent event, void* data)
{
    DeviceMode* mode = static_cast<DeviceMode*>(data);
    // Обработка смены режима
}

void OnErrorOccurred(DeviceEvent event, void* data)
{
    char* error_msg = static_cast<char*>(data);
    // Обработка ошибки
}

void OnDataReceived(DeviceEvent event, void* data)
{
    uint8_t* buffer = static_cast<uint8_t*>(data);
    // Обработка полученных данных
}


int main()
{
    HAL::Init();
    HAL_TIM::DelayMS(500);             // �������� ����� ��� ����, ����� AD9952 ����� ������ ���������� �������������

    Generator::Stop();

    FPGA::Init();
    
    // Инициализация новой архитектуры
    device.Initialize();
    device.SetCommunicator(&spi_communicator);
    
    // Регистрация callbacks
    CallbackManager::RegisterCallback(DeviceEvent::MODE_CHANGED, OnModeChanged);
    CallbackManager::RegisterCallback(DeviceEvent::ERROR_OCCURRED, OnErrorOccurred);
    CallbackManager::RegisterCallback(DeviceEvent::DATA_RECEIVED, OnDataReceived);
    
    // Установка начального режима
    device.SetModeIdle();

    while (1)
    {
        // Обработка входящих данных через новую архитектуру
        device.ProcessIncomingData();
        
        // Сохраняем совместимость с существующим кодом
        DInterface::Update();

//        FPGA::WritePeriod(TypeSignal::_1_12V, Value(1000));
//        FPGA::WritePeriod(TypeSignal::_2a, Value(1000));
//
//        FPGA::Start();
//
//        TimeMeterMS().Delay(5000);
//
//        FPGA::Stop();
//
//        TimeMeterMS().Delay(5000);
    }
}
