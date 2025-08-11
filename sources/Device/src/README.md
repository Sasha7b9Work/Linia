# Device Communication Architecture

Новая архитектура для управления устройством с поддержкой различных протоколов связи и системы обратных вызовов.

## Структура

### 1. Базовый класс Communicator
Абстрактный класс для различных протоколов связи:
```cpp
class Communicator
{
    virtual bool Initialize() = 0;
    virtual bool SendData(const uint8_t* data, uint16_t size) = 0;
    virtual bool ReceiveData(uint8_t* buffer, uint16_t size, uint16_t timeout_ms = 1000) = 0;
    virtual bool IsConnected() = 0;
    virtual void Disconnect() = 0;
};
```

### 2. Реализации протоколов
- **SPICommunicator** - коммуникация через SPI
- **I2CCommunicator** - коммуникация через I2C
- **ComPortCommunicator** - коммуникация через UART

### 3. Система событий
Класс `CallbackManager` предоставляет механизм регистрации функций обратного вызова:
```cpp
enum class DeviceEvent
{
    MODE_CHANGED,
    ERROR_OCCURRED,
    DATA_RECEIVED,
    CONNECTION_LOST,
    GENERATOR_STARTED,
    GENERATOR_STOPPED
};
```

### 4. Основной класс Device
Центральный класс для управления устройством:
```cpp
enum class DeviceMode
{
    IDLE,
    GENERATOR_1V12,
    GENERATOR_1V24,
    GENERATOR_2A,
    GENERATOR_3A,
    PAUSE,
    ERROR
};
```

## Использование

### Базовое использование
```cpp
#include "Device/Device.h"
#include "Communication/SPICommunicator.h"

Device device;
SPICommunicator spi_comm;

int main()
{
    // Инициализация
    device.Initialize();
    device.SetCommunicator(&spi_comm);
    
    // Установка режима
    device.SetModeGenerator1V12();
    device.Start();
    
    while(1)
    {
        device.ProcessIncomingData();
    }
}
```

### Использование с callbacks
```cpp
#include "Utils/CallbackManager.h"

void OnModeChanged(DeviceEvent event, void* data)
{
    DeviceMode* mode = static_cast<DeviceMode*>(data);
    // Обработка смены режима
}

int main()
{
    // Регистрация callback
    CallbackManager::RegisterCallback(DeviceEvent::MODE_CHANGED, OnModeChanged);
    
    // ... остальная инициализация
}
```

### Методы управления режимами

#### Универсальный метод
```cpp
device.SetMode(DeviceMode::GENERATOR_1V12);
```

#### Специализированные методы
```cpp
device.SetModeIdle();
device.SetModeGenerator1V12();
device.SetModeGenerator1V24();
device.SetModeGenerator2A();
device.SetModeGenerator3A();
device.SetModePause();
```

### Управление выполнением
```cpp
device.Start();    // Запуск генератора
device.Pause();    // Пауза
device.Resume();   // Возобновление
device.Stop();     // Остановка
```

### Отправка данных
```cpp
uint8_t response[] = {0x01, 0x02, 0x03};
device.SendResponse(response, sizeof(response));
```

## Интеграция с существующим кодом

Новая архитектура полностью совместима с существующим кодом. В `main_d.cpp` сохранена поддержка старого интерфейса:

```cpp
int main()
{
    HAL::Init();
    
    // Новая архитектура
    device.Initialize();
    device.SetCommunicator(&spi_communicator);
    device.SetModeIdle();
    
    while (1)
    {
        // Новый интерфейс
        device.ProcessIncomingData();
        
        // Старый интерфейс (сохранена совместимость)
        DInterface::Update();
    }
}
```

## Файлы

### Communication/
- `Communicator.h` - базовый интерфейс
- `SPICommunicator.h/.cpp` - SPI реализация
- `I2CCommunicator.h/.cpp` - I2C реализация  
- `ComPortCommunicator.h/.cpp` - UART реализация

### Device/
- `Device.h/.cpp` - основной класс управления
- `DeviceExamples.cpp` - примеры использования

### Utils/
- `CallbackManager.h/.cpp` - система событий

## Преимущества

1. **Модульность** - легко добавлять новые протоколы связи
2. **Расширяемость** - простое добавление новых режимов работы
3. **Событийная модель** - гибкая система обратных вызовов
4. **Совместимость** - работает с существующим кодом
5. **Инкапсуляция** - четкое разделение ответственности между компонентами
