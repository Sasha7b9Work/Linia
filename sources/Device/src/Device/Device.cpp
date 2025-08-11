// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Device.h"

// Forward declarations
namespace Generator
{
    void Init();
    void Stop();
    void Start();
    void Pause();
}

namespace FPGA
{
    void SetTypeSignal(int signal);
}

namespace Log
{
    void AddString(char *format, ...);
}

// Определение типов сигналов из FPGA.h
struct TypeSignal
{
    enum E
    {
        _1_12V,
        _1_24V,
        _2a,
        _3a,
        _3b,
        Off,
        Count
    };
};

Device::Device() 
    : current_mode(DeviceMode::IDLE)
    , previous_mode(DeviceMode::IDLE)
    , is_running(false)
    , is_initialized(false)
    , communicator(nullptr)
{
}

Device::~Device()
{
    Stop();
}

bool Device::Initialize()
{
    if (is_initialized)
        return true;
        
    Log::AddString("Initializing Device...");
    
    // Инициализация генератора
    Generator::Init();
    
    is_initialized = true;
    Log::AddString("Device initialized successfully");
    return true;
}

bool Device::SetCommunicator(Communicator* comm)
{
    if (!comm)
        return false;
        
    communicator = comm;
    return communicator->Initialize();
}

bool Device::SetMode(DeviceMode mode)
{
    if (!is_initialized)
    {
        HandleError("Device not initialized");
        return false;
    }
    
    if (current_mode == mode)
        return true; // Уже в нужном режиме
        
    Log::AddString("Changing mode from %d to %d", static_cast<int>(current_mode), static_cast<int>(mode));
    
    return ChangeMode(mode);
}

bool Device::ChangeMode(DeviceMode new_mode)
{
    DeviceMode old_mode = current_mode;
    
    // Остановка текущего режима
    if (is_running)
    {
        Generator::Pause();
    }
    
    // Настройка нового режима
    bool success = false;
    switch (new_mode)
    {
        case DeviceMode::IDLE:
            Generator::Stop();
            success = true;
            break;
            
        case DeviceMode::GENERATOR_1V12:
            success = ConfigureGenerator1V12();
            break;
            
        case DeviceMode::GENERATOR_1V24:
            success = ConfigureGenerator1V24();
            break;
            
        case DeviceMode::GENERATOR_2A:
            success = ConfigureGenerator2A();
            break;
            
        case DeviceMode::GENERATOR_3A:
            success = ConfigureGenerator3A();
            break;
            
        case DeviceMode::PAUSE:
            success = true; // Пауза не требует дополнительной настройки
            break;
            
        default:
            HandleError("Unknown mode");
            return false;
    }
    
    if (success)
    {
        previous_mode = current_mode;
        current_mode = new_mode;
        OnModeChanged(old_mode, new_mode);
        return true;
    }
    else
    {
        HandleError("Failed to change mode");
        return false;
    }
}

bool Device::ConfigureGenerator1V12()
{
    FPGA::SetTypeSignal(TypeSignal::_1_12V);
    return InitializeGeneratorMode();
}

bool Device::ConfigureGenerator1V24()
{
    FPGA::SetTypeSignal(TypeSignal::_1_24V);
    return InitializeGeneratorMode();
}

bool Device::ConfigureGenerator2A()
{
    FPGA::SetTypeSignal(TypeSignal::_2a);
    return InitializeGeneratorMode();
}

bool Device::ConfigureGenerator3A()
{
    FPGA::SetTypeSignal(TypeSignal::_3a);
    return InitializeGeneratorMode();
}

bool Device::InitializeGeneratorMode()
{
    // Общая инициализация для всех режимов генератора
    return true; // Здесь должна быть реальная логика инициализации
}

void Device::OnModeChanged(DeviceMode old_mode, DeviceMode new_mode)
{
    Log::AddString("Mode changed: %d -> %d", static_cast<int>(old_mode), static_cast<int>(new_mode));
    CallbackManager::TriggerEvent(DeviceEvent::MODE_CHANGED, &new_mode);
}

void Device::HandleError(const char* error_message)
{
    Log::AddString("Device Error: %s", error_message);
    current_mode = DeviceMode::ERROR;
    CallbackManager::TriggerEvent(DeviceEvent::ERROR_OCCURRED, const_cast<char*>(error_message));
}

// Удобные методы для установки конкретных режимов
bool Device::SetModeIdle() { return SetMode(DeviceMode::IDLE); }
bool Device::SetModeGenerator1V12() { return SetMode(DeviceMode::GENERATOR_1V12); }
bool Device::SetModeGenerator1V24() { return SetMode(DeviceMode::GENERATOR_1V24); }
bool Device::SetModeGenerator2A() { return SetMode(DeviceMode::GENERATOR_2A); }
bool Device::SetModeGenerator3A() { return SetMode(DeviceMode::GENERATOR_3A); }
bool Device::SetModePause() { return SetMode(DeviceMode::PAUSE); }

bool Device::Start()
{
    if (current_mode == DeviceMode::IDLE || current_mode == DeviceMode::ERROR)
        return false;
        
    Generator::Start();
    is_running = true;
    CallbackManager::TriggerEvent(DeviceEvent::GENERATOR_STARTED, nullptr);
    return true;
}

bool Device::Stop()
{
    Generator::Stop();
    is_running = false;
    SetMode(DeviceMode::IDLE);
    CallbackManager::TriggerEvent(DeviceEvent::GENERATOR_STOPPED, nullptr);
    return true;
}

bool Device::Pause()
{
    if (!is_running)
        return false;
        
    Generator::Pause();
    SetMode(DeviceMode::PAUSE);
    return true;
}

bool Device::Resume()
{
    if (current_mode != DeviceMode::PAUSE)
        return false;
        
    SetMode(previous_mode);
    Generator::Start();
    is_running = true;
    return true;
}

void Device::ProcessIncomingData()
{
    if (!communicator || !communicator->IsConnected())
        return;
        
    uint8_t buffer[256];
    if (communicator->ReceiveData(buffer, sizeof(buffer), 10))
    {
        CallbackManager::TriggerEvent(DeviceEvent::DATA_RECEIVED, buffer);
        // Здесь обработка полученных данных
    }
}

bool Device::SendResponse(const uint8_t* data, uint16_t size)
{
    if (!communicator || !communicator->IsConnected())
        return false;
        
    return communicator->SendData(data, size);
}
