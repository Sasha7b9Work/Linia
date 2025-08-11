// 2025/08/11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Device/DeviceCommunicator.h"
#include "Communicator/ComPort/ComPort.h"
#include "Utils/Log.h"
#include <algorithm>
#include <sstream>

using namespace std::chrono;

DeviceCommunicator::DeviceCommunicator()
    : state(DISCONNECTED)
    , device_mode(IDLE)
    , current_com_port(-1)
    , sent_commands_count(0)
    , received_responses_count(0)
    , errors_count(0)
    , last_response_time(0ms)
{
}

DeviceCommunicator::~DeviceCommunicator()
{
    Shutdown();
}

bool DeviceCommunicator::Initialize()
{
    LogMessage("DeviceCommunicator: Initializing...");
    
    // Настройка колбэка для получения данных от ComPort
    ComPort::CallbackOnReceive::Set([this](pchar data) {
        if (data && strlen(data) > 0)
        {
            // Добавляем полученные данные в буфер
            const char* str_data = reinterpret_cast<const char*>(data);
            size_t len = strlen(str_data);
            
            for (size_t i = 0; i < len; i++)
            {
                receive_buffer.push_back(static_cast<uint8_t>(str_data[i]));
            }
        }
    });
    
    SetState(DISCONNECTED);
    return true;
}

void DeviceCommunicator::Shutdown()
{
    if (IsConnected())
    {
        Disconnect();
    }
    
    ComPort::CallbackOnReceive::Reset();
    LogMessage("DeviceCommunicator: Shutdown complete");
}

void DeviceCommunicator::Update()
{
    switch (state)
    {
    case CONNECTING:
        UpdateConnection();
        break;
        
    case CONNECTED:
        UpdateConnected();
        break;
        
    case DISCONNECTED:
    case ERROR_STATE:
        // Ничего не делаем
        break;
    }
}

bool DeviceCommunicator::Connect(int com_port)
{
    if (state == CONNECTED || state == CONNECTING)
    {
        LogError("Already connected or connecting");
        return false;
    }
    
    SetState(CONNECTING);
    connection_start_time = steady_clock::now();
    
    if (com_port >= 0)
    {
        // Подключение к указанному порту
        current_com_port = com_port;
        return TryConnectToPort(com_port);
    }
    else
    {
        // Автопоиск портов
        std::vector<int> ports = FindAvailablePorts();
        
        for (int port : ports)
        {
            if (TryConnectToPort(port))
            {
                current_com_port = port;
                return true;
            }
        }
        
        SetState(ERROR_STATE);
        HandleError("No suitable COM ports found");
        return false;
    }
}

void DeviceCommunicator::Disconnect()
{
    if (state == CONNECTED || state == CONNECTING)
    {
        ComPort::Close();
        current_com_port = -1;
        
        // Очищаем очереди
        while (!pending_commands.empty())
        {
            pending_commands.pop();
        }
        current_command.reset();
        receive_buffer.clear();
        
        SetState(DISCONNECTED);
        SetDeviceMode(IDLE);
        
        LogMessage("Disconnected from STM32");
    }
}

bool DeviceCommunicator::SendStartCommand1_12V(float voltage, int time_ms)
{
    if (!ValidateParameters1_12V(voltage, time_ms))
    {
        HandleError("Invalid parameters for 1.12V mode");
        return false;
    }
    
    Protocol::Value v_val(voltage);
    Protocol::Value t_val(time_ms);
    
    auto message = std::make_shared<Protocol::StartMessage1_12V>(v_val, t_val);
    
    if (SendMessage(message))
    {
        SetDeviceMode(MEASURING_1_12V);
        return true;
    }
    
    return false;
}

bool DeviceCommunicator::SendStartCommand1_24V(float voltage, int time_ms)
{
    if (!ValidateParameters1_24V(voltage, time_ms))
    {
        HandleError("Invalid parameters for 1.24V mode");
        return false;
    }
    
    Protocol::Value v_val(voltage);
    Protocol::Value t_val(time_ms);
    
    auto message = std::make_shared<Protocol::StartMessage1_24V>(v_val, t_val);
    
    if (SendMessage(message))
    {
        SetDeviceMode(MEASURING_1_24V);
        return true;
    }
    
    return false;
}

bool DeviceCommunicator::SendStartCommand2A(float voltage, int time_ms)
{
    if (!ValidateParameters2A(voltage, time_ms))
    {
        HandleError("Invalid parameters for 2A mode");
        return false;
    }
    
    Protocol::Value v_val(voltage);
    Protocol::Value t_val(time_ms);
    
    auto message = std::make_shared<Protocol::StartMessage2A>(v_val, t_val);
    
    if (SendMessage(message))
    {
        SetDeviceMode(MEASURING_2A);
        return true;
    }
    
    return false;
}

bool DeviceCommunicator::SendStartCommand3A(float voltage)
{
    if (!ValidateParameters3A(voltage))
    {
        HandleError("Invalid parameters for 3A mode");
        return false;
    }
    
    Protocol::Value v_val(voltage);
    auto message = std::make_shared<Protocol::StartMessage3A>(v_val);
    
    if (SendMessage(message))
    {
        SetDeviceMode(MEASURING_3A);
        return true;
    }
    
    return false;
}

bool DeviceCommunicator::SendStartCommand3B(float voltage)
{
    if (!ValidateParameters3B(voltage))
    {
        HandleError("Invalid parameters for 3B mode");
        return false;
    }
    
    Protocol::Value v_val(voltage);
    auto message = std::make_shared<Protocol::StartMessage3B>(v_val);
    
    if (SendMessage(message))
    {
        SetDeviceMode(MEASURING_3B);
        return true;
    }
    
    return false;
}

bool DeviceCommunicator::SendStopCommand()
{
    auto message = std::make_shared<Protocol::StopMessage>();
    
    if (SendMessage(message))
    {
        SetDeviceMode(IDLE);
        return true;
    }
    
    return false;
}

bool DeviceCommunicator::SendPauseCommand()
{
    if (device_mode == IDLE || device_mode == PAUSED)
    {
        HandleError("Cannot pause: device not measuring");
        return false;
    }
    
    auto message = std::make_shared<Protocol::PauseMessage>();
    
    if (SendMessage(message))
    {
        SetDeviceMode(PAUSED);
        return true;
    }
    
    return false;
}

bool DeviceCommunicator::SendResumeCommand()
{
    if (device_mode != PAUSED)
    {
        HandleError("Cannot resume: device not paused");
        return false;
    }
    
    auto message = std::make_shared<Protocol::ResumeMessage>();
    return SendMessage(message);
}

std::string DeviceCommunicator::GetStateString() const
{
    switch (state)
    {
    case DISCONNECTED: return "Disconnected";
    case CONNECTING: return "Connecting";
    case CONNECTED: return "Connected";
    case ERROR_STATE: return "Error";
    default: return "Unknown";
    }
}

std::string DeviceCommunicator::GetDeviceModeString() const
{
    switch (device_mode)
    {
    case IDLE: return "Idle";
    case MEASURING_1_12V: return "Measuring 1.12V";
    case MEASURING_1_24V: return "Measuring 1.24V";
    case MEASURING_2A: return "Measuring 2A";
    case MEASURING_3A: return "Measuring 3A";
    case MEASURING_3B: return "Measuring 3B";
    case PAUSED: return "Paused";
    case ERROR_MODE: return "Error";
    default: return "Unknown";
    }
}

bool DeviceCommunicator::IsDeviceBusy() const
{
    return (device_mode >= MEASURING_1_12V && device_mode <= MEASURING_3B);
}

// Валидация параметров
bool DeviceCommunicator::ValidateParameters1_12V(float voltage, int time_ms)
{
    return (voltage >= 0 && voltage <= 180.0f && time_ms >= 1 && time_ms <= 60000);
}

bool DeviceCommunicator::ValidateParameters1_24V(float voltage, int time_ms)
{
    return (voltage >= 0 && voltage <= 720.0f && time_ms >= 1 && time_ms <= 60000);
}

bool DeviceCommunicator::ValidateParameters2A(float voltage, int time_ms)
{
    return (voltage >= 0 && voltage <= 134.4f && time_ms >= 1 && time_ms <= 60000);
}

bool DeviceCommunicator::ValidateParameters3A(float voltage)
{
    return (voltage >= 0 && voltage <= 360.0f);
}

bool DeviceCommunicator::ValidateParameters3B(float voltage)
{
    return (voltage >= 0 && voltage <= 360.0f);
}

// Внутренние методы
void DeviceCommunicator::SetState(State new_state)
{
    if (state != new_state)
    {
        State old_state = state;
        state = new_state;
        
        LogMessage("State changed: " + GetStateString());
        
        if (on_state_changed)
        {
            on_state_changed(old_state, new_state);
        }
    }
}

void DeviceCommunicator::SetDeviceMode(DeviceMode new_mode)
{
    if (device_mode != new_mode)
    {
        device_mode = new_mode;
        LogMessage("Device mode changed: " + GetDeviceModeString());
    }
}

void DeviceCommunicator::HandleError(const std::string& message)
{
    errors_count++;
    LogError(message);
    
    if (on_error)
    {
        on_error(message);
    }
}

bool DeviceCommunicator::SendMessage(std::shared_ptr<Protocol::Message> message)
{
    if (state != CONNECTED)
    {
        HandleError("Cannot send message: not connected");
        return false;
    }
    
    if (!message || !message->IsValid())
    {
        HandleError("Invalid message");
        return false;
    }
    
    // Добавляем в очередь
    pending_commands.push(message);
    sent_commands_count++;
    
    return true;
}

void DeviceCommunicator::ProcessIncomingData()
{
    if (receive_buffer.size() < 16) // Минимальный размер ответа
        return;
    
    // Пытаемся найти начало сообщения (signature)
    for (size_t i = 0; i <= receive_buffer.size() - 16; i++)
    {
        uint32_t signature = Protocol::Utils::DeserializeUint32(&receive_buffer[i]);
        
        if (signature == Protocol::Utils::MESSAGE_SIGNATURE)
        {
            // Найдено начало сообщения
            std::vector<uint8_t> response_data(receive_buffer.begin() + i, receive_buffer.end());
            
            Protocol::Response response = Protocol::Response::Parse(response_data);
            
            if (response.IsValid())
            {
                // Удаляем обработанные данные из буфера
                receive_buffer.erase(receive_buffer.begin(), receive_buffer.begin() + i + 16 + response.data_size + 4);
                
                HandleResponse(response);
                
                // Обрабатываем остальные данные
                if (!receive_buffer.empty())
                {
                    ProcessIncomingData();
                }
                return;
            }
        }
    }
    
    // Если буфер слишком большой, очищаем его частично
    if (receive_buffer.size() > 1024)
    {
        receive_buffer.erase(receive_buffer.begin(), receive_buffer.begin() + 512);
    }
}

void DeviceCommunicator::ProcessPendingCommands()
{
    if (current_command != nullptr)
    {
        // Проверяем тайм-аут
        auto now = steady_clock::now();
        auto elapsed = duration_cast<milliseconds>(now - last_command_time);
        
        if (elapsed.count() > RESPONSE_TIMEOUT_MS)
        {
            HandleTimeout();
        }
        
        return; // Ждем ответ на текущую команду
    }
    
    if (!pending_commands.empty())
    {
        current_command = pending_commands.front();
        pending_commands.pop();
        
        // Отправляем команду
        std::vector<uint8_t> data = current_command->Serialize();
        
        ComPort::Send(data.data(), static_cast<int>(data.size()));
        
        last_command_time = steady_clock::now();
        
        LogMessage("Sent command: " + std::to_string(static_cast<int>(current_command->GetCommand())));
    }
}

bool DeviceCommunicator::TryConnectToPort(int port)
{
    LogMessage("Trying to connect to COM" + std::to_string(port + 1));
    
    // Здесь должна быть логика подключения к порту
    // В данной реализации используем существующий ComPort
    
    return true; // Упрощенная реализация
}

void DeviceCommunicator::UpdateConnection()
{
    auto now = steady_clock::now();
    auto elapsed = duration_cast<milliseconds>(now - connection_start_time);
    
    if (elapsed.count() > CONNECTION_TIMEOUT_MS)
    {
        SetState(ERROR_STATE);
        HandleError("Connection timeout");
        return;
    }
    
    // Здесь должна быть логика проверки соединения
    // Для упрощения считаем, что соединение установлено
    SetState(CONNECTED);
    LogMessage("Connected to COM" + std::to_string(current_com_port + 1));
}

void DeviceCommunicator::UpdateConnected()
{
    ProcessIncomingData();
    ProcessPendingCommands();
    
    ComPort::Update(); // Обновляем ComPort
}

void DeviceCommunicator::HandleResponse(const Protocol::Response& response)
{
    received_responses_count++;
    last_response_time = duration_cast<milliseconds>(steady_clock::now().time_since_epoch());
    
    // Сбрасываем текущую команду
    current_command.reset();
    
    LogMessage("Received response with status: " + std::to_string(static_cast<int>(response.status)));
    
    if (response.status != Protocol::Status::OK)
    {
        HandleError("STM32 error: " + std::to_string(static_cast<int>(response.status)));
        SetDeviceMode(ERROR_MODE);
    }
    
    if (on_response)
    {
        on_response(response);
    }
    
    // Если есть данные измерений
    if (!response.data.empty() && on_measurement_data)
    {
        on_measurement_data(response.data);
    }
}

void DeviceCommunicator::HandleTimeout()
{
    HandleError("Command timeout");
    current_command.reset();
    SetDeviceMode(ERROR_MODE);
}

std::vector<int> DeviceCommunicator::FindAvailablePorts()
{
    std::vector<bool> ports_status;
    ComPort::GetComports(ports_status);
    
    std::vector<int> available_ports;
    
    for (size_t i = 0; i < ports_status.size(); i++)
    {
        if (ports_status[i])
        {
            available_ports.push_back(static_cast<int>(i));
        }
    }
    
    return available_ports;
}

void DeviceCommunicator::LogMessage(const std::string& message)
{
    // Используем существующую систему логирования
    Log::AddString("[DeviceCommunicator] %s", message.c_str());
}

void DeviceCommunicator::LogError(const std::string& error)
{
    Log::AddString("[DeviceCommunicator ERROR] %s", error.c_str());
}
