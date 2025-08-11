// 2025/08/11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "defines.h"
#include "Device/Protocol.h"
#include <vector>
#include <string>
#include <chrono>
#include <memory>
#include <fstream>


// Система логирования и сохранения результатов измерений
namespace MeasurementLogger
{
    // Типы событий для логирования
    enum EventType
    {
        COMMAND_SENT = 0,
        RESPONSE_RECEIVED,
        MEASUREMENT_DATA,
        ERROR_OCCURRED,
        STATE_CHANGED,
        CONNECTION_EVENT
    };

    // Структура записи в логе
    struct LogEntry
    {
        std::chrono::system_clock::time_point timestamp;
        EventType event_type;
        std::string message;
        std::vector<uint8_t> data; // Дополнительные данные
        
        LogEntry(EventType type, const std::string& msg)
            : timestamp(std::chrono::system_clock::now())
            , event_type(type)
            , message(msg)
        {}
        
        LogEntry(EventType type, const std::string& msg, const std::vector<uint8_t>& raw_data)
            : timestamp(std::chrono::system_clock::now())
            , event_type(type)
            , message(msg)
            , data(raw_data)
        {}
        
        std::string ToString() const;
        std::string GetEventTypeString() const;
    };

    // Структура сеанса измерений
    struct MeasurementSession
    {
        std::string session_id;
        std::chrono::system_clock::time_point start_time;
        std::chrono::system_clock::time_point end_time;
        Protocol::Command measurement_type;
        Protocol::Value voltage;
        Protocol::Value time_duration;
        std::vector<LogEntry> events;
        std::vector<std::vector<uint8_t>> measurement_data;
        bool is_completed;
        bool has_errors;
        
        MeasurementSession(const std::string& id, Protocol::Command cmd);
        
        void AddEvent(const LogEntry& entry);
        void AddMeasurementData(const std::vector<uint8_t>& data);
        void Complete();
        void MarkError();
        
        std::string GetSummary() const;
        size_t GetTotalDataSize() const;
        std::chrono::milliseconds GetDuration() const;
    };

    // Основной класс логгера
    class Logger
    {
    public:
        Logger();
        ~Logger();

        bool Initialize(const std::string& log_directory = "");
        void Shutdown();

        // Управление сеансами
        void StartSession(const std::string& session_id, Protocol::Command command);
        void EndSession();
        bool IsSessionActive() const { return current_session != nullptr; }
        std::string GetCurrentSessionId() const;

        // Логирование событий
        void LogCommandSent(Protocol::Command command, const std::string& details = "");
        void LogResponseReceived(const Protocol::Response& response);
        void LogMeasurementData(const std::vector<uint8_t>& data);
        void LogError(const std::string& error_message);
        void LogStateChange(const std::string& old_state, const std::string& new_state);
        void LogConnectionEvent(const std::string& event);

        // Сохранение данных
        bool SaveSessionToFile(const MeasurementSession& session, const std::string& filename = "");
        bool SaveAllSessionsToFile(const std::string& filename = "");
        bool ExportSessionToCsv(const MeasurementSession& session, const std::string& filename = "");
        bool ExportSessionToJson(const MeasurementSession& session, const std::string& filename = "");

        // Управление историей
        void ClearHistory();
        void SetMaxHistorySize(size_t max_sessions) { max_history_size = max_sessions; }
        size_t GetHistorySize() const { return completed_sessions.size(); }
        
        // Получение данных
        std::vector<std::shared_ptr<MeasurementSession>> GetCompletedSessions() const;
        std::shared_ptr<MeasurementSession> GetSession(const std::string& session_id) const;
        std::vector<LogEntry> GetRecentEvents(size_t count = 100) const;

        // Настройки
        void SetAutoSave(bool enabled) { auto_save_enabled = enabled; }
        void SetCompressionEnabled(bool enabled) { compression_enabled = enabled; }
        void SetMaxLogFileSize(size_t max_size_mb) { max_log_file_size = max_size_mb * 1024 * 1024; }

    private:
        std::string log_directory;
        std::string current_log_file;
        std::ofstream log_stream;
        
        std::shared_ptr<MeasurementSession> current_session;
        std::vector<std::shared_ptr<MeasurementSession>> completed_sessions;
        std::vector<LogEntry> global_events;
        
        size_t max_history_size;
        size_t max_log_file_size;
        bool auto_save_enabled;
        bool compression_enabled;
        
        // Внутренние методы
        void WriteToLogFile(const LogEntry& entry);
        void RotateLogFileIfNeeded();
        std::string GenerateLogFileName() const;
        std::string GenerateSessionFileName(const MeasurementSession& session) const;
        void CleanupOldSessions();
        
        // Сериализация
        std::string SerializeSession(const MeasurementSession& session) const;
        std::string SerializeSessionToCsv(const MeasurementSession& session) const;
        std::string SerializeSessionToJson(const MeasurementSession& session) const;
    };

    // Глобальный экземпляр логгера
    extern Logger* g_logger;

    // Удобные функции для быстрого доступа
    void Initialize(const std::string& log_directory = "");
    void Shutdown();
    
    void StartSession(const std::string& session_id, Protocol::Command command);
    void EndSession();
    
    void LogCommand(Protocol::Command command, const std::string& details = "");
    void LogResponse(const Protocol::Response& response);
    void LogData(const std::vector<uint8_t>& data);
    void LogError(const std::string& error);
    void LogStateChange(const std::string& old_state, const std::string& new_state);
    void LogConnection(const std::string& event);
    
    bool SaveCurrentSession(const std::string& filename = "");
    bool ExportCurrentSessionToCsv(const std::string& filename = "");
    bool ExportCurrentSessionToJson(const std::string& filename = "");
}
