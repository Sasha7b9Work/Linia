// 2025/08/11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "defines.h"
#include "Device/DeviceCommunicator.h"
#include "Device/MeasurementLogger.h"
#include <memory>
#include <functional>
#include <string>


// Высокоуровневый API для управления устройством
// Предоставляет простые функции SetModeX() для установки режимов STM32
namespace Device
{
    // Параметры для режимов измерения
    struct MeasurementParams
    {
        float voltage;      // Напряжение в вольтах
        int time_ms;        // Время в миллисекундах (для режимов, где требуется)
        
        MeasurementParams(float v = 0.0f, int t = 1000) : voltage(v), time_ms(t) {}
    };

    // Результат выполнения операции
    enum OperationResult
    {
        SUCCESS = 0,
        ERROR_NOT_CONNECTED,
        ERROR_INVALID_PARAMS,
        ERROR_DEVICE_BUSY,
        ERROR_TIMEOUT,
        ERROR_COMMUNICATION,
        ERROR_HARDWARE_FAULT
    };

    // Колбэк для уведомлений о состоянии
    using StatusCallback = std::function<void(const std::string& message, bool is_error)>;
    using ProgressCallback = std::function<void(int percentage, const std::string& status)>;
    using DataCallback = std::function<void(const std::vector<uint8_t>& data)>;

    // === ОСНОВНЫЕ ФУНКЦИИ УПРАВЛЕНИЯ ===

    // Инициализация системы управления устройством
    bool Initialize(const std::string& log_directory = "logs");
    
    // Завершение работы с устройством
    void Shutdown();

    // === УПРАВЛЕНИЕ ПОДКЛЮЧЕНИЕМ ===

    // Подключение к устройству (автопоиск или конкретный порт)
    OperationResult Connect(int com_port = -1);
    
    // Отключение от устройства
    OperationResult Disconnect();
    
    // Проверка состояния подключения
    bool IsConnected();
    
    // Получение информации о подключении
    std::string GetConnectionInfo();

    // === УСТАНОВКА РЕЖИМОВ ИЗМЕРЕНИЯ ===

    // Режим 1.12V (напряжение: 0-180В, время: 1-60000мс)
    OperationResult SetMode1_12V(const MeasurementParams& params);
    
    // Режим 1.24V (напряжение: 0-720В, время: 1-60000мс)
    OperationResult SetMode1_24V(const MeasurementParams& params);
    
    // Режим 2A (напряжение: 0-134.4В, время: 1-60000мс)
    OperationResult SetMode2A(const MeasurementParams& params);
    
    // Режим 3A (напряжение: 0-360В, время не используется)
    OperationResult SetMode3A(const MeasurementParams& params);
    
    // Режим 3B (напряжение: 0-360В, время не используется)
    OperationResult SetMode3B(const MeasurementParams& params);

    // === УПРАВЛЕНИЕ ИЗМЕРЕНИЕМ ===

    // Остановка текущего измерения
    OperationResult Stop();
    
    // Пауза измерения
    OperationResult Pause();
    
    // Возобновление измерения
    OperationResult Resume();

    // === ИНФОРМАЦИЯ О СОСТОЯНИИ ===

    // Получение текущего режима устройства
    std::string GetCurrentMode();
    
    // Проверка, выполняется ли измерение
    bool IsMeasuring();
    
    // Проверка, на паузе ли измерение
    bool IsPaused();
    
    // Получение последней ошибки
    std::string GetLastError();

    // === УПРАВЛЕНИЕ СЕАНСАМИ ===

    // Начало нового сеанса измерений
    OperationResult StartSession(const std::string& session_name = "");
    
    // Завершение текущего сеанса
    OperationResult EndSession();
    
    // Получение ID текущего сеанса
    std::string GetCurrentSessionId();
    
    // Сохранение текущего сеанса
    OperationResult SaveSession(const std::string& filename = "");
    
    // Экспорт в CSV
    OperationResult ExportSessionCSV(const std::string& filename = "");
    
    // Экспорт в JSON  
    OperationResult ExportSessionJSON(const std::string& filename = "");

    // === СТАТИСТИКА И МОНИТОРИНГ ===

    // Получение статистики связи
    struct Statistics
    {
        int commands_sent;
        int responses_received;
        int errors_count;
        int connection_quality_percent;
        std::chrono::milliseconds last_response_time;
        size_t total_data_received;
    };
    
    Statistics GetStatistics();
    
    // Сброс статистики
    void ResetStatistics();

    // === КОЛБЭКИ И УВЕДОМЛЕНИЯ ===

    // Установка колбэка для статусных сообщений
    void SetStatusCallback(StatusCallback callback);
    
    // Установка колбэка для прогресса операций
    void SetProgressCallback(ProgressCallback callback);
    
    // Установка колбэка для получения данных измерений
    void SetDataCallback(DataCallback callback);

    // === УТИЛИТЫ ===

    // Валидация параметров для режима
    bool ValidateParams1_12V(const MeasurementParams& params);
    bool ValidateParams1_24V(const MeasurementParams& params);
    bool ValidateParams2A(const MeasurementParams& params);
    bool ValidateParams3A(const MeasurementParams& params);
    bool ValidateParams3B(const MeasurementParams& params);
    
    // Получение диапазонов параметров для режима
    struct ParamRanges
    {
        float min_voltage, max_voltage;
        int min_time, max_time;
        bool time_required;
    };
    
    ParamRanges GetParamRanges1_12V();
    ParamRanges GetParamRanges1_24V();
    ParamRanges GetParamRanges2A();
    ParamRanges GetParamRanges3A();
    ParamRanges GetParamRanges3B();
    
    // Преобразование результата в строку
    std::string ResultToString(OperationResult result);
    
    // Обновление состояния (вызывать в основном цикле)
    void Update();
}
