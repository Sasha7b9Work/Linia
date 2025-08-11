// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "defines.h"
#include "Communication/Communicator.h"
#include "Utils/CallbackManager.h"

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

class Device
{
public:
    Device();
    ~Device();
    
    bool Initialize();
    bool SetCommunicator(Communicator* comm);
    
    // Методы управления режимами
    bool SetMode(DeviceMode mode);
    DeviceMode GetCurrentMode() const { return current_mode; }
    
    // Специализированные методы SetMode
    bool SetModeIdle();
    bool SetModeGenerator1V12();
    bool SetModeGenerator1V24();
    bool SetModeGenerator2A();
    bool SetModeGenerator3A();
    bool SetModePause();
    
    // Управление
    bool Start();
    bool Stop();
    bool Pause();
    bool Resume();
    
    // Статус
    bool IsRunning() const { return is_running; }
    bool IsError() const { return current_mode == DeviceMode::ERROR; }
    
    // Обработка данных
    void ProcessIncomingData();
    bool SendResponse(const uint8_t* data, uint16_t size);
    
private:
    DeviceMode current_mode;
    DeviceMode previous_mode;
    bool is_running;
    bool is_initialized;
    
    Communicator* communicator;
    
    // Внутренние методы
    bool ChangeMode(DeviceMode new_mode);
    void OnModeChanged(DeviceMode old_mode, DeviceMode new_mode);
    void HandleError(const char* error_message);
    
    // Низкоуровневые методы для каждого режима
    bool InitializeGeneratorMode();
    bool ConfigureGenerator1V12();
    bool ConfigureGenerator1V24();
    bool ConfigureGenerator2A();
    bool ConfigureGenerator3A();
};
