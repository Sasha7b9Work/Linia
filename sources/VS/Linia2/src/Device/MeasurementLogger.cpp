// 2025/08/11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Device/MeasurementLogger.h"
#include "Utils/StringUtils.h"
#include <filesystem>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <fstream>

namespace fs = std::filesystem;
using namespace std::chrono;

namespace MeasurementLogger
{
    Logger* g_logger = nullptr;
}

// LogEntry implementation
std::string MeasurementLogger::LogEntry::ToString() const
{
    std::ostringstream oss;
    
    auto time_t = system_clock::to_time_t(timestamp);
    auto ms = duration_cast<milliseconds>(timestamp.time_since_epoch()) % 1000;
    
    oss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
    oss << "." << std::setfill('0') << std::setw(3) << ms.count();
    oss << " [" << GetEventTypeString() << "] " << message;
    
    if (!data.empty())
    {
        oss << " (data: " << data.size() << " bytes)";
    }
    
    return oss.str();
}

std::string MeasurementLogger::LogEntry::GetEventTypeString() const
{
    switch (event_type)
    {
    case COMMAND_SENT: return "CMD_SENT";
    case RESPONSE_RECEIVED: return "RESPONSE";
    case MEASUREMENT_DATA: return "DATA";
    case ERROR_OCCURRED: return "ERROR";
    case STATE_CHANGED: return "STATE";
    case CONNECTION_EVENT: return "CONNECTION";
    default: return "UNKNOWN";
    }
}

// MeasurementSession implementation
MeasurementLogger::MeasurementSession::MeasurementSession(const std::string& id, Protocol::Command cmd)
    : session_id(id)
    , start_time(system_clock::now())
    , measurement_type(cmd)
    , voltage(0)
    , time_duration(0)
    , is_completed(false)
    , has_errors(false)
{
}

void MeasurementLogger::MeasurementSession::AddEvent(const LogEntry& entry)
{
    events.push_back(entry);
    
    if (entry.event_type == ERROR_OCCURRED)
    {
        has_errors = true;
    }
}

void MeasurementLogger::MeasurementSession::AddMeasurementData(const std::vector<uint8_t>& data)
{
    measurement_data.push_back(data);
    AddEvent(LogEntry(MEASUREMENT_DATA, "Measurement data received", data));
}

void MeasurementLogger::MeasurementSession::Complete()
{
    end_time = system_clock::now();
    is_completed = true;
    AddEvent(LogEntry(STATE_CHANGED, "Session completed"));
}

void MeasurementLogger::MeasurementSession::MarkError()
{
    has_errors = true;
    AddEvent(LogEntry(ERROR_OCCURRED, "Session marked with error"));
}

std::string MeasurementLogger::MeasurementSession::GetSummary() const
{
    std::ostringstream oss;
    
    oss << "Session: " << session_id << "\n";
    oss << "Type: " << static_cast<int>(measurement_type) << "\n";
    oss << "Duration: " << GetDuration().count() << " ms\n";
    oss << "Events: " << events.size() << "\n";
    oss << "Data chunks: " << measurement_data.size() << "\n";
    oss << "Total data: " << GetTotalDataSize() << " bytes\n";
    oss << "Status: " << (is_completed ? "Completed" : "Active");
    if (has_errors) oss << " (with errors)";
    
    return oss.str();
}

size_t MeasurementLogger::MeasurementSession::GetTotalDataSize() const
{
    size_t total = 0;
    for (const auto& data : measurement_data)
    {
        total += data.size();
    }
    return total;
}

std::chrono::milliseconds MeasurementLogger::MeasurementSession::GetDuration() const
{
    auto end = is_completed ? end_time : system_clock::now();
    return duration_cast<milliseconds>(end - start_time);
}

// Logger implementation
MeasurementLogger::Logger::Logger()
    : max_history_size(100)
    , max_log_file_size(10 * 1024 * 1024) // 10 MB
    , auto_save_enabled(true)
    , compression_enabled(false)
{
}

MeasurementLogger::Logger::~Logger()
{
    Shutdown();
}

bool MeasurementLogger::Logger::Initialize(const std::string& log_directory)
{
    if (log_directory.empty())
    {
        this->log_directory = "logs";
    }
    else
    {
        this->log_directory = log_directory;
    }
    
    // Создаем директорию для логов
    try
    {
        fs::create_directories(this->log_directory);
    }
    catch (const std::exception& e)
    {
        // Не удалось создать директорию, используем текущую
        this->log_directory = ".";
    }
    
    // Открываем файл лога
    current_log_file = GenerateLogFileName();
    log_stream.open(current_log_file, std::ios::app);
    
    if (!log_stream.is_open())
    {
        return false;
    }
    
    // Записываем заголовок
    LogEntry init_entry(CONNECTION_EVENT, "Logger initialized");
    WriteToLogFile(init_entry);
    
    return true;
}

void MeasurementLogger::Logger::Shutdown()
{
    if (current_session)
    {
        EndSession();
    }
    
    if (log_stream.is_open())
    {
        LogEntry shutdown_entry(CONNECTION_EVENT, "Logger shutdown");
        WriteToLogFile(shutdown_entry);
        log_stream.close();
    }
    
    if (auto_save_enabled)
    {
        SaveAllSessionsToFile();
    }
}

void MeasurementLogger::Logger::StartSession(const std::string& session_id, Protocol::Command command)
{
    if (current_session)
    {
        EndSession();
    }
    
    current_session = std::make_shared<MeasurementSession>(session_id, command);
    
    LogEntry entry(STATE_CHANGED, "Session started: " + session_id);
    WriteToLogFile(entry);
    current_session->AddEvent(entry);
}

void MeasurementLogger::Logger::EndSession()
{
    if (!current_session)
        return;
    
    current_session->Complete();
    
    LogEntry entry(STATE_CHANGED, "Session ended: " + current_session->session_id);
    WriteToLogFile(entry);
    current_session->AddEvent(entry);
    
    // Добавляем в историю
    completed_sessions.push_back(current_session);
    
    // Автосохранение
    if (auto_save_enabled)
    {
        SaveSessionToFile(*current_session);
    }
    
    current_session.reset();
    
    // Очистка старых сеансов
    CleanupOldSessions();
}

std::string MeasurementLogger::Logger::GetCurrentSessionId() const
{
    return current_session ? current_session->session_id : "";
}

void MeasurementLogger::Logger::LogCommandSent(Protocol::Command command, const std::string& details)
{
    std::string message = "Command sent: " + std::to_string(static_cast<int>(command));
    if (!details.empty())
    {
        message += " (" + details + ")";
    }
    
    LogEntry entry(COMMAND_SENT, message);
    WriteToLogFile(entry);
    global_events.push_back(entry);
    
    if (current_session)
    {
        current_session->AddEvent(entry);
    }
}

void MeasurementLogger::Logger::LogResponseReceived(const Protocol::Response& response)
{
    std::string message = "Response received: status=" + std::to_string(static_cast<int>(response.status));
    if (response.data_size > 0)
    {
        message += ", data_size=" + std::to_string(response.data_size);
    }
    
    LogEntry entry(RESPONSE_RECEIVED, message);
    WriteToLogFile(entry);
    global_events.push_back(entry);
    
    if (current_session)
    {
        current_session->AddEvent(entry);
    }
}

void MeasurementLogger::Logger::LogMeasurementData(const std::vector<uint8_t>& data)
{
    std::string message = "Measurement data: " + std::to_string(data.size()) + " bytes";
    
    LogEntry entry(MEASUREMENT_DATA, message, data);
    WriteToLogFile(entry);
    global_events.push_back(entry);
    
    if (current_session)
    {
        current_session->AddMeasurementData(data);
    }
}

void MeasurementLogger::Logger::LogError(const std::string& error_message)
{
    LogEntry entry(ERROR_OCCURRED, error_message);
    WriteToLogFile(entry);
    global_events.push_back(entry);
    
    if (current_session)
    {
        current_session->AddEvent(entry);
        current_session->MarkError();
    }
}

void MeasurementLogger::Logger::LogStateChange(const std::string& old_state, const std::string& new_state)
{
    std::string message = "State change: " + old_state + " -> " + new_state;
    
    LogEntry entry(STATE_CHANGED, message);
    WriteToLogFile(entry);
    global_events.push_back(entry);
    
    if (current_session)
    {
        current_session->AddEvent(entry);
    }
}

void MeasurementLogger::Logger::LogConnectionEvent(const std::string& event)
{
    LogEntry entry(CONNECTION_EVENT, event);
    WriteToLogFile(entry);
    global_events.push_back(entry);
    
    if (current_session)
    {
        current_session->AddEvent(entry);
    }
}

bool MeasurementLogger::Logger::SaveSessionToFile(const MeasurementSession& session, const std::string& filename)
{
    std::string file_path;
    
    if (filename.empty())
    {
        file_path = GenerateSessionFileName(session);
    }
    else
    {
        file_path = log_directory + "/" + filename;
    }
    
    std::ofstream file(file_path);
    if (!file.is_open())
        return false;
    
    file << SerializeSession(session);
    return true;
}

bool MeasurementLogger::Logger::SaveAllSessionsToFile(const std::string& filename)
{
    std::string file_path = filename.empty() ? 
        (log_directory + "/all_sessions.log") : 
        (log_directory + "/" + filename);
    
    std::ofstream file(file_path);
    if (!file.is_open())
        return false;
    
    for (const auto& session : completed_sessions)
    {
        file << SerializeSession(*session) << "\n" << std::string(80, '=') << "\n";
    }
    
    return true;
}

bool MeasurementLogger::Logger::ExportSessionToCsv(const MeasurementSession& session, const std::string& filename)
{
    std::string file_path = filename.empty() ? 
        GenerateSessionFileName(session) + ".csv" : 
        (log_directory + "/" + filename);
    
    std::ofstream file(file_path);
    if (!file.is_open())
        return false;
    
    file << SerializeSessionToCsv(session);
    return true;
}

bool MeasurementLogger::Logger::ExportSessionToJson(const MeasurementSession& session, const std::string& filename)
{
    std::string file_path = filename.empty() ? 
        GenerateSessionFileName(session) + ".json" : 
        (log_directory + "/" + filename);
    
    std::ofstream file(file_path);
    if (!file.is_open())
        return false;
    
    file << SerializeSessionToJson(session);
    return true;
}

void MeasurementLogger::Logger::ClearHistory()
{
    completed_sessions.clear();
    global_events.clear();
}

std::vector<std::shared_ptr<MeasurementLogger::MeasurementSession>> MeasurementLogger::Logger::GetCompletedSessions() const
{
    return completed_sessions;
}

std::shared_ptr<MeasurementLogger::MeasurementSession> MeasurementLogger::Logger::GetSession(const std::string& session_id) const
{
    auto it = std::find_if(completed_sessions.begin(), completed_sessions.end(),
        [&session_id](const std::shared_ptr<MeasurementSession>& session) {
            return session->session_id == session_id;
        });
    
    if (it != completed_sessions.end())
        return *it;
    
    if (current_session && current_session->session_id == session_id)
        return current_session;
    
    return nullptr;
}

std::vector<MeasurementLogger::LogEntry> MeasurementLogger::Logger::GetRecentEvents(size_t count) const
{
    std::vector<LogEntry> result;
    
    size_t start_idx = global_events.size() > count ? global_events.size() - count : 0;
    
    for (size_t i = start_idx; i < global_events.size(); i++)
    {
        result.push_back(global_events[i]);
    }
    
    return result;
}

// Внутренние методы
void MeasurementLogger::Logger::WriteToLogFile(const LogEntry& entry)
{
    if (log_stream.is_open())
    {
        log_stream << entry.ToString() << std::endl;
        log_stream.flush();
        
        RotateLogFileIfNeeded();
    }
}

void MeasurementLogger::Logger::RotateLogFileIfNeeded()
{
    if (!log_stream.is_open())
        return;
    
    log_stream.seekp(0, std::ios::end);
    size_t file_size = static_cast<size_t>(log_stream.tellp());
    
    if (file_size > max_log_file_size)
    {
        log_stream.close();
        
        // Переименовываем старый файл
        std::string old_file = current_log_file + ".old";
        fs::rename(current_log_file, old_file);
        
        // Создаем новый файл
        current_log_file = GenerateLogFileName();
        log_stream.open(current_log_file, std::ios::app);
    }
}

std::string MeasurementLogger::Logger::GenerateLogFileName() const
{
    auto now = system_clock::now();
    auto time_t = system_clock::to_time_t(now);
    
    std::ostringstream oss;
    oss << log_directory << "/linia_" 
        << std::put_time(std::localtime(&time_t), "%Y%m%d_%H%M%S") 
        << ".log";
    
    return oss.str();
}

std::string MeasurementLogger::Logger::GenerateSessionFileName(const MeasurementSession& session) const
{
    auto time_t = system_clock::to_time_t(session.start_time);
    
    std::ostringstream oss;
    oss << log_directory << "/session_" 
        << std::put_time(std::localtime(&time_t), "%Y%m%d_%H%M%S")
        << "_" << session.session_id;
    
    return oss.str();
}

void MeasurementLogger::Logger::CleanupOldSessions()
{
    while (completed_sessions.size() > max_history_size)
    {
        completed_sessions.erase(completed_sessions.begin());
    }
}

std::string MeasurementLogger::Logger::SerializeSession(const MeasurementSession& session) const
{
    std::ostringstream oss;
    
    oss << "SESSION: " << session.session_id << "\n";
    oss << "TYPE: " << static_cast<int>(session.measurement_type) << "\n";
    oss << "START: " << system_clock::to_time_t(session.start_time) << "\n";
    
    if (session.is_completed)
    {
        oss << "END: " << system_clock::to_time_t(session.end_time) << "\n";
        oss << "DURATION: " << session.GetDuration().count() << " ms\n";
    }
    
    oss << "STATUS: " << (session.is_completed ? "COMPLETED" : "ACTIVE");
    if (session.has_errors) oss << " (ERRORS)";
    oss << "\n";
    
    oss << "EVENTS:\n";
    for (const auto& event : session.events)
    {
        oss << "  " << event.ToString() << "\n";
    }
    
    oss << "DATA_CHUNKS: " << session.measurement_data.size() << "\n";
    oss << "TOTAL_DATA_SIZE: " << session.GetTotalDataSize() << " bytes\n";
    
    return oss.str();
}

std::string MeasurementLogger::Logger::SerializeSessionToCsv(const MeasurementSession& session) const
{
    std::ostringstream oss;
    
    // Заголовок CSV
    oss << "Timestamp,EventType,Message,DataSize\n";
    
    for (const auto& event : session.events)
    {
        auto time_t = system_clock::to_time_t(event.timestamp);
        auto ms = duration_cast<milliseconds>(event.timestamp.time_since_epoch()) % 1000;
        
        oss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
        oss << "." << std::setfill('0') << std::setw(3) << ms.count();
        oss << "," << event.GetEventTypeString();
        oss << ",\"" << event.message << "\"";
        oss << "," << event.data.size();
        oss << "\n";
    }
    
    return oss.str();
}

std::string MeasurementLogger::Logger::SerializeSessionToJson(const MeasurementSession& session) const
{
    std::ostringstream oss;
    
    oss << "{\n";
    oss << "  \"session_id\": \"" << session.session_id << "\",\n";
    oss << "  \"measurement_type\": " << static_cast<int>(session.measurement_type) << ",\n";
    oss << "  \"start_time\": " << system_clock::to_time_t(session.start_time) << ",\n";
    
    if (session.is_completed)
    {
        oss << "  \"end_time\": " << system_clock::to_time_t(session.end_time) << ",\n";
        oss << "  \"duration_ms\": " << session.GetDuration().count() << ",\n";
    }
    
    oss << "  \"is_completed\": " << (session.is_completed ? "true" : "false") << ",\n";
    oss << "  \"has_errors\": " << (session.has_errors ? "true" : "false") << ",\n";
    oss << "  \"total_data_size\": " << session.GetTotalDataSize() << ",\n";
    
    oss << "  \"events\": [\n";
    for (size_t i = 0; i < session.events.size(); i++)
    {
        const auto& event = session.events[i];
        
        oss << "    {\n";
        oss << "      \"timestamp\": " << system_clock::to_time_t(event.timestamp) << ",\n";
        oss << "      \"type\": \"" << event.GetEventTypeString() << "\",\n";
        oss << "      \"message\": \"" << event.message << "\",\n";
        oss << "      \"data_size\": " << event.data.size() << "\n";
        oss << "    }";
        
        if (i < session.events.size() - 1) oss << ",";
        oss << "\n";
    }
    oss << "  ]\n";
    oss << "}\n";
    
    return oss.str();
}

// Глобальные функции
void MeasurementLogger::Initialize(const std::string& log_directory)
{
    if (g_logger == nullptr)
    {
        g_logger = new Logger();
    }
    g_logger->Initialize(log_directory);
}

void MeasurementLogger::Shutdown()
{
    if (g_logger)
    {
        g_logger->Shutdown();
        delete g_logger;
        g_logger = nullptr;
    }
}

void MeasurementLogger::StartSession(const std::string& session_id, Protocol::Command command)
{
    if (g_logger) g_logger->StartSession(session_id, command);
}

void MeasurementLogger::EndSession()
{
    if (g_logger) g_logger->EndSession();
}

void MeasurementLogger::LogCommand(Protocol::Command command, const std::string& details)
{
    if (g_logger) g_logger->LogCommandSent(command, details);
}

void MeasurementLogger::LogResponse(const Protocol::Response& response)
{
    if (g_logger) g_logger->LogResponseReceived(response);
}

void MeasurementLogger::LogData(const std::vector<uint8_t>& data)
{
    if (g_logger) g_logger->LogMeasurementData(data);
}

void MeasurementLogger::LogError(const std::string& error)
{
    if (g_logger) g_logger->LogError(error);
}

void MeasurementLogger::LogStateChange(const std::string& old_state, const std::string& new_state)
{
    if (g_logger) g_logger->LogStateChange(old_state, new_state);
}

void MeasurementLogger::LogConnection(const std::string& event)
{
    if (g_logger) g_logger->LogConnectionEvent(event);
}

bool MeasurementLogger::SaveCurrentSession(const std::string& filename)
{
    if (!g_logger || !g_logger->IsSessionActive()) return false;
    
    auto session = g_logger->GetSession(g_logger->GetCurrentSessionId());
    if (!session) return false;
    
    return g_logger->SaveSessionToFile(*session, filename);
}

bool MeasurementLogger::ExportCurrentSessionToCsv(const std::string& filename)
{
    if (!g_logger || !g_logger->IsSessionActive()) return false;
    
    auto session = g_logger->GetSession(g_logger->GetCurrentSessionId());
    if (!session) return false;
    
    return g_logger->ExportSessionToCsv(*session, filename);
}

bool MeasurementLogger::ExportCurrentSessionToJson(const std::string& filename)
{
    if (!g_logger || !g_logger->IsSessionActive()) return false;
    
    auto session = g_logger->GetSession(g_logger->GetCurrentSessionId());
    if (!session) return false;
    
    return g_logger->ExportSessionToJson(*session, filename);
}
