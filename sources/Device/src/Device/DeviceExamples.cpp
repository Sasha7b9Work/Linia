// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
// Примеры использования новой архитектуры Device

#include "defines.h"
#include "Device/Device.h"
#include "Communication/SPICommunicator.h"
#include "Communication/I2CCommunicator.h"
#include "Communication/ComPortCommunicator.h"
#include "Utils/CallbackManager.h"

// Пример 1: Базовое использование с SPI
void Example_BasicSPIUsage()
{
    Device device;
    SPICommunicator spi_comm;
    
    // Инициализация
    device.Initialize();
    device.SetCommunicator(&spi_comm);
    
    // Установка режимов
    device.SetModeGenerator1V12();
    device.Start();
    
    // Пауза
    device.Pause();
    device.Resume();
    
    // Остановка
    device.Stop();
}

// Пример 2: Использование с I2C
void Example_I2CUsage()
{
    Device device;
    I2CCommunicator i2c_comm(0x48); // Адрес устройства
    
    device.Initialize();
    device.SetCommunicator(&i2c_comm);
    
    device.SetModeGenerator2A();
    device.Start();
}

// Пример 3: Callback функции
void OnDeviceModeChanged(DeviceEvent event, void* data)
{
    DeviceMode* mode = static_cast<DeviceMode*>(data);
    
    switch (*mode)
    {
        case DeviceMode::GENERATOR_1V12:
            // Действия при переключении в режим 1.12V
            break;
            
        case DeviceMode::GENERATOR_2A:
            // Действия при переключении в режим 2A
            break;
            
        case DeviceMode::PAUSE:
            // Действия при паузе
            break;
            
        default:
            break;
    }
}

void OnDeviceError(DeviceEvent event, void* data)
{
    char* error_message = static_cast<char*>(data);
    // Обработка ошибки, например, запись в лог или отправка уведомления
}

// Пример 4: Регистрация callbacks
void Example_CallbackUsage()
{
    CallbackManager::RegisterCallback(DeviceEvent::MODE_CHANGED, OnDeviceModeChanged);
    CallbackManager::RegisterCallback(DeviceEvent::ERROR_OCCURRED, OnDeviceError);
    
    Device device;
    SPICommunicator spi_comm;
    
    device.Initialize();
    device.SetCommunicator(&spi_comm);
    
    // При смене режима автоматически вызовется OnDeviceModeChanged
    device.SetModeGenerator1V24();
}

// Пример 5: Отправка данных
void Example_DataTransmission()
{
    Device device;
    SPICommunicator spi_comm;
    
    device.Initialize();
    device.SetCommunicator(&spi_comm);
    
    // Отправка ответа
    uint8_t response_data[] = {0x01, 0x02, 0x03, 0x04};
    device.SendResponse(response_data, sizeof(response_data));
}

// Пример 6: Обработка входящих данных
void OnDataReceived(DeviceEvent event, void* data)
{
    uint8_t* buffer = static_cast<uint8_t*>(data);
    
    // Обработка команд
    switch (buffer[0])
    {
        case 0x01: // Команда старт
            {
                Device* device = nullptr; // Получить ссылку на device
                device->Start();
            }
            break;
            
        case 0x02: // Команда стоп
            {
                Device* device = nullptr; // Получить ссылку на device  
                device->Stop();
            }
            break;
            
        case 0x03: // Команда смены режима
            {
                Device* device = nullptr; // Получить ссылку на device
                if (buffer[1] == 0x01)
                    device->SetModeGenerator1V12();
                else if (buffer[1] == 0x02)
                    device->SetModeGenerator2A();
            }
            break;
    }
}

// Пример 7: Полная интеграция
void Example_FullIntegration()
{
    Device device;
    SPICommunicator spi_comm;
    
    // Инициализация
    device.Initialize();
    device.SetCommunicator(&spi_comm);
    
    // Регистрация всех необходимых callbacks
    CallbackManager::RegisterCallback(DeviceEvent::MODE_CHANGED, OnDeviceModeChanged);
    CallbackManager::RegisterCallback(DeviceEvent::ERROR_OCCURRED, OnDeviceError);
    CallbackManager::RegisterCallback(DeviceEvent::DATA_RECEIVED, OnDataReceived);
    CallbackManager::RegisterCallback(DeviceEvent::GENERATOR_STARTED, 
        [](DeviceEvent event, void* data) {
            // Действия при старте генератора
        });
    
    // Установка начального режима
    device.SetModeIdle();
    
    // Основной цикл
    while (true)
    {
        device.ProcessIncomingData();
        
        // Другая логика приложения
        // ...
    }
}
