// 2025/08/11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "defines.h"
#include "Device/Protocol.h"
#include <memory>
#include <functional>
#include <queue>
#include <chrono>


// Менеджер связи с STM32 через RS-232
class DeviceCommunicator
{
public:
    enum State
    {
        DISCONNECTED = 0,
        CONNECTING,
        CONNECTED,
        ERROR_STATE
    };

    enum DeviceMode
    {
        IDLE = 0,
        MEASURING_1_12V,
        MEASURING_1_24V,
        MEASURING_2A,
        MEASURING_3A,
        MEASURING_3B,
        PAUSED,
        ERROR_MODE
    };

    // Колбэки для событий
    using OnStateChangedCallback = std::function<void(State old_state, State new_state)>;
    using OnResponseCallback = std::function<void(const Protocol::Response& response)>;
    using OnErrorCallback = std::function<void(const std::string& error_message)>;
    using OnMeasurementDataCallback = std::function<void(const std::vector<uint8_t>& data)>;

    DeviceCommunicator();
    ~DeviceCommunicator();

    // Основные методы управления
    bool Initialize();
    void Shutdown();
    void Update(); // Вызывать каждый кадр
    
    // Управление соединением
    bool Connect(int com_port = -1); // -1 = автопоиск
    void Disconnect();
    bool IsConnected() const { return state == CONNECTED; }
    
    // Отправка команд
    bool SendStartCommand1_12V(float voltage, int time_ms);
    bool SendStartCommand1_24V(float voltage, int time_ms);
    bool SendStartCommand2A(float voltage, int time_ms);
    bool SendStartCommand3A(float voltage);
    bool SendStartCommand3B(float voltage);
    bool SendStopCommand();
    bool SendPauseCommand();
    bool SendResumeCommand();

    // Статус и мониторинг
    State GetState() const { return state; }
    DeviceMode GetDeviceMode() const { return device_mode; }
    std::string GetStateString() const;
    std::string GetDeviceModeString() const;
    bool IsDeviceBusy() const;
    
    // Статистика
    int GetSentCommandsCount() const { return sent_commands_count; }
    int GetReceivedResponsesCount() const { return received_responses_count; }
    int GetErrorsCount() const { return errors_count; }
    std::chrono::milliseconds GetLastResponseTime() const { return last_response_time; }
    
    // Колбэки
    void SetOnStateChanged(OnStateChangedCallback callback) { on_state_changed = callback; }
    void SetOnResponse(OnResponseCallback callback) { on_response = callback; }
    void SetOnError(OnErrorCallback callback) { on_error = callback; }
    void SetOnMeasurementData(OnMeasurementDataCallback callback) { on_measurement_data = callback; }

    // Валидация параметров
    static bool ValidateParameters1_12V(float voltage, int time_ms);
    static bool ValidateParameters1_24V(float voltage, int time_ms);
    static bool ValidateParameters2A(float voltage, int time_ms);
    static bool ValidateParameters3A(float voltage);
    static bool ValidateParameters3B(float voltage);

private:
    State state;
    DeviceMode device_mode;
    int current_com_port;
    
    // Статистика
    int sent_commands_count;
    int received_responses_count;
    int errors_count;
    std::chrono::milliseconds last_response_time;
    
    // Тайм-ауты и таймеры
    std::chrono::steady_clock::time_point last_command_time;
    std::chrono::steady_clock::time_point connection_start_time;
    static const int RESPONSE_TIMEOUT_MS = 5000;
    static const int CONNECTION_TIMEOUT_MS = 10000;
    
    // Очереди команд
    std::queue<std::shared_ptr<Protocol::Message>> pending_commands;
    std::shared_ptr<Protocol::Message> current_command;
    
    // Буфер для получения данных
    std::vector<uint8_t> receive_buffer;
    
    // Колбэки
    OnStateChangedCallback on_state_changed;
    OnResponseCallback on_response;
    OnErrorCallback on_error;
    OnMeasurementDataCallback on_measurement_data;
    
    // Внутренние методы
    void SetState(State new_state);
    void SetDeviceMode(DeviceMode new_mode);
    void HandleError(const std::string& message);
    
    bool SendMessage(std::shared_ptr<Protocol::Message> message);
    void ProcessIncomingData();
    void ProcessPendingCommands();
    
    bool TryConnectToPort(int port);
    void UpdateConnection();
    void UpdateConnected();
    
    // Обработка ответов
    void HandleResponse(const Protocol::Response& response);
    void HandleTimeout();
    
    // Поиск портов
    std::vector<int> FindAvailablePorts();
    
    // Логирование
    void LogMessage(const std::string& message);
    void LogError(const std::string& error);
};
