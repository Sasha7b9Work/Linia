// 2025/08/11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Device/DeviceAPI.h"
#include "Utils/StringUtils.h"
#include <sstream>
#include <iomanip>
#include <chrono>

namespace Device
{
    // Глобальные переменные для управления
    static std::unique_ptr<DeviceCommunicator> g_communicator;
    static std::string g_last_error;
    static std::string g_current_session_id;
    static bool g_is_initialized = false;
    
    // Колбэки
    static StatusCallback g_status_callback;
    static ProgressCallback g_progress_callback;
    static DataCallback g_data_callback;
    
    // Внутренние функции
    static void OnDeviceStateChanged(DeviceCommunicator::State old_state, DeviceCommunicator::State new_state);
    static void OnDeviceResponse(const Protocol::Response& response);
    static void OnDeviceError(const std::string& error);
    static void OnMeasurementData(const std::vector<uint8_t>& data);
    
    static OperationResult ConvertCommunicatorResult(bool success, const std::string& error_context = "");
    static std::string GenerateSessionId();
    static void LogOperation(const std::string& operation, OperationResult result);
}

// === ИНИЦИАЛИЗАЦИЯ И ЗАВЕРШЕНИЕ ===

bool Device::Initialize(const std::string& log_directory)
{
    if (g_is_initialized)
    {
        return true; // Уже инициализировано
    }
    
    try
    {
        // Инициализация логгера
        MeasurementLogger::Initialize(log_directory);
        
        // Создание коммуникатора
        g_communicator = std::make_unique<DeviceCommunicator>();
        
        if (!g_communicator->Initialize())
        {
            g_last_error = "Failed to initialize DeviceCommunicator";
            return false;
        }
        
        // Настройка колбэков
        g_communicator->SetOnStateChanged(OnDeviceStateChanged);
        g_communicator->SetOnResponse(OnDeviceResponse);
        g_communicator->SetOnError(OnDeviceError);
        g_communicator->SetOnMeasurementData(OnMeasurementData);
        
        g_is_initialized = true;
        g_last_error.clear();
        
        LogOperation("Initialize", SUCCESS);
        
        if (g_status_callback)
        {
            g_status_callback("Device system initialized", false);
        }
        
        return true;
    }
    catch (const std::exception& e)
    {
        g_last_error = "Exception during initialization: " + std::string(e.what());
        return false;
    }
}

void Device::Shutdown()
{
    if (!g_is_initialized)
        return;
    
    try
    {
        // Завершаем текущий сеанс
        if (!g_current_session_id.empty())
        {
            EndSession();
        }
        
        // Отключаемся от устройства
        if (g_communicator && g_communicator->IsConnected())
        {
            g_communicator->Disconnect();
        }
        
        // Освобождаем ресурсы
        g_communicator.reset();
        MeasurementLogger::Shutdown();
        
        g_is_initialized = false;
        
        if (g_status_callback)
        {
            g_status_callback("Device system shutdown", false);
        }
    }
    catch (const std::exception& e)
    {
        g_last_error = "Exception during shutdown: " + std::string(e.what());
    }
}

// === УПРАВЛЕНИЕ ПОДКЛЮЧЕНИЕМ ===

Device::OperationResult Device::Connect(int com_port)
{
    if (!g_is_initialized)
    {
        g_last_error = "Device system not initialized";
        return ERROR_COMMUNICATION;
    }
    
    if (g_communicator->IsConnected())
    {
        return SUCCESS; // Уже подключено
    }
    
    if (g_status_callback)
    {
        g_status_callback("Connecting to device...", false);
    }
    
    bool success = g_communicator->Connect(com_port);
    OperationResult result = ConvertCommunicatorResult(success, "Connect");
    
    LogOperation("Connect", result);
    
    if (result == SUCCESS && g_status_callback)
    {
        g_status_callback("Connected to device: " + GetConnectionInfo(), false);
    }
    
    return result;
}

Device::OperationResult Device::Disconnect()
{
    if (!g_is_initialized)
    {
        g_last_error = "Device system not initialized";
        return ERROR_COMMUNICATION;
    }
    
    // Завершаем текущий сеанс перед отключением
    if (!g_current_session_id.empty())
    {
        EndSession();
    }
    
    g_communicator->Disconnect();
    
    LogOperation("Disconnect", SUCCESS);
    
    if (g_status_callback)
    {
        g_status_callback("Disconnected from device", false);
    }
    
    return SUCCESS;
}

bool Device::IsConnected()
{
    return g_is_initialized && g_communicator && g_communicator->IsConnected();
}

std::string Device::GetConnectionInfo()
{
    if (!IsConnected())
        return "Not connected";
    
    std::ostringstream info;
    info << "Connected (";
    info << "State: " << g_communicator->GetStateString();
    info << ", Mode: " << g_communicator->GetDeviceModeString();
    info << ")";
    
    return info.str();
}

// === УСТАНОВКА РЕЖИМОВ ИЗМЕРЕНИЯ ===

Device::OperationResult Device::SetMode1_12V(const MeasurementParams& params)
{
    if (!ValidateParams1_12V(params))
    {
        g_last_error = "Invalid parameters for 1.12V mode";
        return ERROR_INVALID_PARAMS;
    }
    
    if (!IsConnected())
    {
        g_last_error = "Device not connected";
        return ERROR_NOT_CONNECTED;
    }
    
    if (IsMeasuring())
    {
        g_last_error = "Device is busy with another measurement";
        return ERROR_DEVICE_BUSY;
    }
    
    // Автоматически начинаем новый сеанс, если нет активного
    if (g_current_session_id.empty())
    {
        StartSession("auto_1_12V");
    }
    
    if (g_status_callback)
    {
        std::ostringstream msg;
        msg << "Setting mode 1.12V: " << params.voltage << "V, " << params.time_ms << "ms";
        g_status_callback(msg.str(), false);
    }
    
    bool success = g_communicator->SendStartCommand1_12V(params.voltage, params.time_ms);
    OperationResult result = ConvertCommunicatorResult(success, "SetMode1_12V");
    
    LogOperation("SetMode1_12V", result);
    
    return result;
}

Device::OperationResult Device::SetMode1_24V(const MeasurementParams& params)
{
    if (!ValidateParams1_24V(params))
    {
        g_last_error = "Invalid parameters for 1.24V mode";
        return ERROR_INVALID_PARAMS;
    }
    
    if (!IsConnected())
    {
        g_last_error = "Device not connected";
        return ERROR_NOT_CONNECTED;
    }
    
    if (IsMeasuring())
    {
        g_last_error = "Device is busy with another measurement";
        return ERROR_DEVICE_BUSY;
    }
    
    if (g_current_session_id.empty())
    {
        StartSession("auto_1_24V");
    }
    
    if (g_status_callback)
    {
        std::ostringstream msg;
        msg << "Setting mode 1.24V: " << params.voltage << "V, " << params.time_ms << "ms";
        g_status_callback(msg.str(), false);
    }
    
    bool success = g_communicator->SendStartCommand1_24V(params.voltage, params.time_ms);
    OperationResult result = ConvertCommunicatorResult(success, "SetMode1_24V");
    
    LogOperation("SetMode1_24V", result);
    
    return result;
}

Device::OperationResult Device::SetMode2A(const MeasurementParams& params)
{
    if (!ValidateParams2A(params))
    {
        g_last_error = "Invalid parameters for 2A mode";
        return ERROR_INVALID_PARAMS;
    }
    
    if (!IsConnected())
    {
        g_last_error = "Device not connected";
        return ERROR_NOT_CONNECTED;
    }
    
    if (IsMeasuring())
    {
        g_last_error = "Device is busy with another measurement";
        return ERROR_DEVICE_BUSY;
    }
    
    if (g_current_session_id.empty())
    {
        StartSession("auto_2A");
    }
    
    if (g_status_callback)
    {
        std::ostringstream msg;
        msg << "Setting mode 2A: " << params.voltage << "V, " << params.time_ms << "ms";
        g_status_callback(msg.str(), false);
    }
    
    bool success = g_communicator->SendStartCommand2A(params.voltage, params.time_ms);
    OperationResult result = ConvertCommunicatorResult(success, "SetMode2A");
    
    LogOperation("SetMode2A", result);
    
    return result;
}

Device::OperationResult Device::SetMode3A(const MeasurementParams& params)
{
    if (!ValidateParams3A(params))
    {
        g_last_error = "Invalid parameters for 3A mode";
        return ERROR_INVALID_PARAMS;
    }
    
    if (!IsConnected())
    {
        g_last_error = "Device not connected";
        return ERROR_NOT_CONNECTED;
    }
    
    if (IsMeasuring())
    {
        g_last_error = "Device is busy with another measurement";
        return ERROR_DEVICE_BUSY;
    }
    
    if (g_current_session_id.empty())
    {
        StartSession("auto_3A");
    }
    
    if (g_status_callback)
    {
        std::ostringstream msg;
        msg << "Setting mode 3A: " << params.voltage << "V";
        g_status_callback(msg.str(), false);
    }
    
    bool success = g_communicator->SendStartCommand3A(params.voltage);
    OperationResult result = ConvertCommunicatorResult(success, "SetMode3A");
    
    LogOperation("SetMode3A", result);
    
    return result;
}

Device::OperationResult Device::SetMode3B(const MeasurementParams& params)
{
    if (!ValidateParams3B(params))
    {
        g_last_error = "Invalid parameters for 3B mode";
        return ERROR_INVALID_PARAMS;
    }
    
    if (!IsConnected())
    {
        g_last_error = "Device not connected";
        return ERROR_NOT_CONNECTED;
    }
    
    if (IsMeasuring())
    {
        g_last_error = "Device is busy with another measurement";
        return ERROR_DEVICE_BUSY;
    }
    
    if (g_current_session_id.empty())
    {
        StartSession("auto_3B");
    }
    
    if (g_status_callback)
    {
        std::ostringstream msg;
        msg << "Setting mode 3B: " << params.voltage << "V";
        g_status_callback(msg.str(), false);
    }
    
    bool success = g_communicator->SendStartCommand3B(params.voltage);
    OperationResult result = ConvertCommunicatorResult(success, "SetMode3B");
    
    LogOperation("SetMode3B", result);
    
    return result;
}

// === УПРАВЛЕНИЕ ИЗМЕРЕНИЕМ ===

Device::OperationResult Device::Stop()
{
    if (!IsConnected())
    {
        g_last_error = "Device not connected";
        return ERROR_NOT_CONNECTED;
    }
    
    bool success = g_communicator->SendStopCommand();
    OperationResult result = ConvertCommunicatorResult(success, "Stop");
    
    LogOperation("Stop", result);
    
    if (g_status_callback)
    {
        g_status_callback("Measurement stopped", false);
    }
    
    return result;
}

Device::OperationResult Device::Pause()
{
    if (!IsConnected())
    {
        g_last_error = "Device not connected";
        return ERROR_NOT_CONNECTED;
    }
    
    if (!IsMeasuring())
    {
        g_last_error = "No active measurement to pause";
        return ERROR_DEVICE_BUSY;
    }
    
    bool success = g_communicator->SendPauseCommand();
    OperationResult result = ConvertCommunicatorResult(success, "Pause");
    
    LogOperation("Pause", result);
    
    if (g_status_callback)
    {
        g_status_callback("Measurement paused", false);
    }
    
    return result;
}

Device::OperationResult Device::Resume()
{
    if (!IsConnected())
    {
        g_last_error = "Device not connected";
        return ERROR_NOT_CONNECTED;
    }
    
    if (!IsPaused())
    {
        g_last_error = "No paused measurement to resume";
        return ERROR_DEVICE_BUSY;
    }
    
    bool success = g_communicator->SendResumeCommand();
    OperationResult result = ConvertCommunicatorResult(success, "Resume");
    
    LogOperation("Resume", result);
    
    if (g_status_callback)
    {
        g_status_callback("Measurement resumed", false);
    }
    
    return result;
}

// === ИНФОРМАЦИЯ О СОСТОЯНИИ ===

std::string Device::GetCurrentMode()
{
    if (!IsConnected())
        return "Disconnected";
    
    return g_communicator->GetDeviceModeString();
}

bool Device::IsMeasuring()
{
    return IsConnected() && g_communicator->IsDeviceBusy();
}

bool Device::IsPaused()
{
    return IsConnected() && (g_communicator->GetDeviceMode() == DeviceCommunicator::PAUSED);
}

std::string Device::GetLastError()
{
    return g_last_error;
}

// === УПРАВЛЕНИЕ СЕАНСАМИ ===

Device::OperationResult Device::StartSession(const std::string& session_name)
{
    if (!g_is_initialized)
    {
        g_last_error = "Device system not initialized";
        return ERROR_COMMUNICATION;
    }
    
    // Завершаем текущий сеанс, если есть
    if (!g_current_session_id.empty())
    {
        EndSession();
    }
    
    // Генерируем ID сеанса
    g_current_session_id = session_name.empty() ? GenerateSessionId() : session_name;
    
    // Начинаем новый сеанс в логгере
    MeasurementLogger::StartSession(g_current_session_id, Protocol::Command::START_1_12V); // Placeholder
    
    LogOperation("StartSession", SUCCESS);
    
    if (g_status_callback)
    {
        g_status_callback("Started session: " + g_current_session_id, false);
    }
    
    return SUCCESS;
}

Device::OperationResult Device::EndSession()
{
    if (g_current_session_id.empty())
    {
        return SUCCESS; // Нет активного сеанса
    }
    
    MeasurementLogger::EndSession();
    
    std::string ended_session = g_current_session_id;
    g_current_session_id.clear();
    
    LogOperation("EndSession", SUCCESS);
    
    if (g_status_callback)
    {
        g_status_callback("Ended session: " + ended_session, false);
    }
    
    return SUCCESS;
}

std::string Device::GetCurrentSessionId()
{
    return g_current_session_id;
}

Device::OperationResult Device::SaveSession(const std::string& filename)
{
    if (g_current_session_id.empty())
    {
        g_last_error = "No active session to save";
        return ERROR_INVALID_PARAMS;
    }
    
    bool success = MeasurementLogger::SaveCurrentSession(filename);
    OperationResult result = success ? SUCCESS : ERROR_COMMUNICATION;
    
    if (!success)
    {
        g_last_error = "Failed to save session";
    }
    
    LogOperation("SaveSession", result);
    
    return result;
}

Device::OperationResult Device::ExportSessionCSV(const std::string& filename)
{
    if (g_current_session_id.empty())
    {
        g_last_error = "No active session to export";
        return ERROR_INVALID_PARAMS;
    }
    
    bool success = MeasurementLogger::ExportCurrentSessionToCsv(filename);
    OperationResult result = success ? SUCCESS : ERROR_COMMUNICATION;
    
    if (!success)
    {
        g_last_error = "Failed to export session to CSV";
    }
    
    LogOperation("ExportSessionCSV", result);
    
    return result;
}

Device::OperationResult Device::ExportSessionJSON(const std::string& filename)
{
    if (g_current_session_id.empty())
    {
        g_last_error = "No active session to export";
        return ERROR_INVALID_PARAMS;
    }
    
    bool success = MeasurementLogger::ExportCurrentSessionToJson(filename);
    OperationResult result = success ? SUCCESS : ERROR_COMMUNICATION;
    
    if (!success)
    {
        g_last_error = "Failed to export session to JSON";
    }
    
    LogOperation("ExportSessionJSON", result);
    
    return result;
}

// === СТАТИСТИКА И МОНИТОРИНГ ===

Device::Statistics Device::GetStatistics()
{
    Statistics stats = {};
    
    if (g_communicator)
    {
        stats.commands_sent = g_communicator->GetSentCommandsCount();
        stats.responses_received = g_communicator->GetReceivedResponsesCount();
        stats.errors_count = g_communicator->GetErrorsCount();
        stats.last_response_time = g_communicator->GetLastResponseTime();
        
        // Вычисляем качество соединения
        if (stats.commands_sent > 0)
        {
            int quality = static_cast<int>((static_cast<float>(stats.responses_received) / stats.commands_sent) * 100);
            quality = std::max(0, quality - stats.errors_count * 10);
            stats.connection_quality_percent = quality;
        }
        else
        {
            stats.connection_quality_percent = IsConnected() ? 100 : 0;
        }
    }
    
    return stats;
}

void Device::ResetStatistics()
{
    // Статистика сбрасывается при переподключении коммуникатора
    if (g_communicator)
    {
        bool was_connected = g_communicator->IsConnected();
        
        if (was_connected)
        {
            g_communicator->Disconnect();
            g_communicator->Connect(-1);
        }
    }
}

// === КОЛБЭКИ И УВЕДОМЛЕНИЯ ===

void Device::SetStatusCallback(StatusCallback callback)
{
    g_status_callback = callback;
}

void Device::SetProgressCallback(ProgressCallback callback)
{
    g_progress_callback = callback;
}

void Device::SetDataCallback(DataCallback callback)
{
    g_data_callback = callback;
}

// === УТИЛИТЫ ===

bool Device::ValidateParams1_12V(const MeasurementParams& params)
{
    return DeviceCommunicator::ValidateParameters1_12V(params.voltage, params.time_ms);
}

bool Device::ValidateParams1_24V(const MeasurementParams& params)
{
    return DeviceCommunicator::ValidateParameters1_24V(params.voltage, params.time_ms);
}

bool Device::ValidateParams2A(const MeasurementParams& params)
{
    return DeviceCommunicator::ValidateParameters2A(params.voltage, params.time_ms);
}

bool Device::ValidateParams3A(const MeasurementParams& params)
{
    return DeviceCommunicator::ValidateParameters3A(params.voltage);
}

bool Device::ValidateParams3B(const MeasurementParams& params)
{
    return DeviceCommunicator::ValidateParameters3B(params.voltage);
}

Device::ParamRanges Device::GetParamRanges1_12V()
{
    return {0.0f, 180.0f, 1, 60000, true};
}

Device::ParamRanges Device::GetParamRanges1_24V()
{
    return {0.0f, 720.0f, 1, 60000, true};
}

Device::ParamRanges Device::GetParamRanges2A()
{
    return {0.0f, 134.4f, 1, 60000, true};
}

Device::ParamRanges Device::GetParamRanges3A()
{
    return {0.0f, 360.0f, 0, 0, false};
}

Device::ParamRanges Device::GetParamRanges3B()
{
    return {0.0f, 360.0f, 0, 0, false};
}

std::string Device::ResultToString(OperationResult result)
{
    switch (result)
    {
    case SUCCESS: return "Success";
    case ERROR_NOT_CONNECTED: return "Device not connected";
    case ERROR_INVALID_PARAMS: return "Invalid parameters";
    case ERROR_DEVICE_BUSY: return "Device is busy";
    case ERROR_TIMEOUT: return "Operation timeout";
    case ERROR_COMMUNICATION: return "Communication error";
    case ERROR_HARDWARE_FAULT: return "Hardware fault";
    default: return "Unknown error";
    }
}

void Device::Update()
{
    if (g_communicator)
    {
        g_communicator->Update();
    }
}

// === ВНУТРЕННИЕ ФУНКЦИИ ===

void Device::OnDeviceStateChanged(DeviceCommunicator::State old_state, DeviceCommunicator::State new_state)
{
    std::string old_str = std::to_string(static_cast<int>(old_state));
    std::string new_str = std::to_string(static_cast<int>(new_state));
    MeasurementLogger::LogStateChange(old_str, new_str);
    
    if (g_status_callback)
    {
        std::string message = "Device state: " + std::to_string(static_cast<int>(new_state));
        g_status_callback(message, false);
    }
}

void Device::OnDeviceResponse(const Protocol::Response& response)
{
    MeasurementLogger::LogResponse(response);
    
    if (g_progress_callback)
    {
        // Для демонстрации - в реальной реализации нужно анализировать ответ
        g_progress_callback(50, "Processing response...");
    }
}

void Device::OnDeviceError(const std::string& error)
{
    g_last_error = error;
    MeasurementLogger::LogError(error);
    
    if (g_status_callback)
    {
        g_status_callback(error, true);
    }
}

void Device::OnMeasurementData(const std::vector<uint8_t>& data)
{
    MeasurementLogger::LogData(data);
    
    if (g_data_callback)
    {
        g_data_callback(data);
    }
}

Device::OperationResult Device::ConvertCommunicatorResult(bool success, const std::string& error_context)
{
    if (success)
    {
        g_last_error.clear();
        return SUCCESS;
    }
    
    // Анализируем состояние коммуникатора для определения типа ошибки
    if (!g_communicator)
    {
        g_last_error = error_context + ": Communicator not initialized";
        return ERROR_COMMUNICATION;
    }
    
    if (!g_communicator->IsConnected())
    {
        g_last_error = error_context + ": Device not connected";
        return ERROR_NOT_CONNECTED;
    }
    
    if (g_communicator->IsDeviceBusy())
    {
        g_last_error = error_context + ": Device is busy";
        return ERROR_DEVICE_BUSY;
    }
    
    g_last_error = error_context + ": Communication failed";
    return ERROR_COMMUNICATION;
}

std::string Device::GenerateSessionId()
{
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    
    std::ostringstream oss;
    oss << "session_" << std::put_time(std::localtime(&time_t), "%Y%m%d_%H%M%S");
    
    return oss.str();
}

void Device::LogOperation(const std::string& operation, OperationResult result)
{
    std::string message = operation + ": " + ResultToString(result);
    
    if (result == SUCCESS)
    {
        MeasurementLogger::LogConnection(message);
    }
    else
    {
        MeasurementLogger::LogError(message);
    }
}
