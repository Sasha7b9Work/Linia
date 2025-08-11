// 2025/08/11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
// Пример использования Device API для управления STM32

#include "defines.h"
#include "Device/DeviceAPI.h"
#include <iostream>
#include <string>

// Примеры колбэков для мониторинга
void OnStatusUpdate(const std::string& message, bool is_error)
{
    if (is_error)
    {
        std::cout << "[ERROR] " << message << std::endl;
    }
    else
    {
        std::cout << "[STATUS] " << message << std::endl;
    }
}

void OnProgressUpdate(int progress, const std::string& message)
{
    std::cout << "[PROGRESS] " << progress << "% - " << message << std::endl;
}

void OnDataReceived(const std::vector<uint8_t>& data)
{
    std::cout << "[DATA] Received " << data.size() << " bytes" << std::endl;
}

// === ПРИМЕР 1: ПРОСТОЕ ИЗМЕРЕНИЕ В РЕЖИМЕ 1.12V ===
void Example_SimpleMode1_12V()
{
    std::cout << "\n=== Example: Simple Mode 1.12V ===" << std::endl;
    
    // Инициализация системы
    if (!Device::Initialize("./logs"))
    {
        std::cout << "Failed to initialize device system" << std::endl;
        return;
    }
    
    // Настройка колбэков
    Device::SetStatusCallback(OnStatusUpdate);
    Device::SetProgressCallback(OnProgressUpdate);
    Device::SetDataCallback(OnDataReceived);
    
    // Подключение к устройству
    Device::OperationResult result = Device::Connect(3); // COM3
    if (result != Device::SUCCESS)
    {
        std::cout << "Connection failed: " << Device::ResultToString(result) << std::endl;
        Device::Shutdown();
        return;
    }
    
    // Установка режима 1.12V с напряжением 12V на 5 секунд
    Device::MeasurementParams params = {12.0f, 5000};
    result = Device::SetMode1_12V(params);
    
    if (result == Device::SUCCESS)
    {
        std::cout << "Mode 1.12V started successfully" << std::endl;
        
        // Ожидаем завершения (в реальном приложении это делается через колбэки)
        std::cout << "Current mode: " << Device::GetCurrentMode() << std::endl;
        std::cout << "Is measuring: " << (Device::IsMeasuring() ? "Yes" : "No") << std::endl;
    }
    else
    {
        std::cout << "Failed to start mode 1.12V: " << Device::ResultToString(result) << std::endl;
    }
    
    // Отключение
    Device::Disconnect();
    Device::Shutdown();
}

// === ПРИМЕР 2: РАБОТА С СЕАНСАМИ ===
void Example_SessionManagement()
{
    std::cout << "\n=== Example: Session Management ===" << std::endl;
    
    Device::Initialize("./logs");
    Device::SetStatusCallback(OnStatusUpdate);
    
    if (Device::Connect(3) != Device::SUCCESS)
    {
        std::cout << "Connection failed" << std::endl;
        Device::Shutdown();
        return;
    }
    
    // Начинаем новый сеанс
    Device::StartSession("Test_Session_001");
    std::cout << "Current session: " << Device::GetCurrentSessionId() << std::endl;
    
    // Выполняем несколько измерений
    Device::MeasurementParams params1 = {10.0f, 3000};
    Device::SetMode1_12V(params1);
    
    // Пауза между измерениями
    std::cout << "Pausing measurement..." << std::endl;
    Device::Pause();
    
    // Возобновление
    std::cout << "Resuming measurement..." << std::endl;
    Device::Resume();
    
    // Переключение на другой режим
    Device::MeasurementParams params2 = {24.0f, 2000};
    Device::SetMode1_24V(params2);
    
    // Сохранение сеанса
    Device::SaveSession("test_session.log");
    Device::ExportSessionCSV("test_session.csv");
    Device::ExportSessionJSON("test_session.json");
    
    // Завершение сеанса
    Device::EndSession();
    
    Device::Disconnect();
    Device::Shutdown();
}

// === ПРИМЕР 3: АВТОМАТИЧЕСКАЯ ПОСЛЕДОВАТЕЛЬНОСТЬ ИЗМЕРЕНИЙ ===
void Example_AutomaticSequence()
{
    std::cout << "\n=== Example: Automatic Measurement Sequence ===" << std::endl;
    
    struct MeasurementStep
    {
        std::string name;
        Device::OperationResult (*function)(const Device::MeasurementParams&);
        Device::MeasurementParams params;
        bool (*validator)(const Device::MeasurementParams&);
    };
    
    // Последовательность измерений
    std::vector<MeasurementStep> sequence = {
        {"Mode 1.12V - 10V", Device::SetMode1_12V, {10.0f, 2000}, Device::ValidateParams1_12V},
        {"Mode 1.24V - 20V", Device::SetMode1_24V, {20.0f, 3000}, Device::ValidateParams1_24V},
        {"Mode 2A - 15V", Device::SetMode2A, {15.0f, 1500}, Device::ValidateParams2A},
        {"Mode 3A - 25V", Device::SetMode3A, {25.0f, 0}, Device::ValidateParams3A},
        {"Mode 3B - 30V", Device::SetMode3B, {30.0f, 0}, Device::ValidateParams3B}
    };
    
    Device::Initialize("./logs");
    Device::SetStatusCallback(OnStatusUpdate);
    Device::SetProgressCallback(OnProgressUpdate);
    
    if (Device::Connect(3) != Device::SUCCESS)
    {
        std::cout << "Connection failed" << std::endl;
        Device::Shutdown();
        return;
    }
    
    Device::StartSession("Automatic_Sequence");
    
    for (const auto& step : sequence)
    {
        std::cout << "\nExecuting: " << step.name << std::endl;
        
        // Валидация параметров
        if (!step.validator(step.params))
        {
            std::cout << "Invalid parameters for " << step.name << std::endl;
            continue;
        }
        
        // Выполнение измерения
        Device::OperationResult result = step.function(step.params);
        
        if (result == Device::SUCCESS)
        {
            std::cout << step.name << " started successfully" << std::endl;
            
            // Ждем завершения (упрощенно)
            // В реальном приложении используйте колбэки для мониторинга
        }
        else
        {
            std::cout << step.name << " failed: " << Device::ResultToString(result) << std::endl;
        }
        
        // Небольшая задержка между измерениями
        // Sleep(1000); // Uncomment if needed
    }
    
    Device::ExportSessionCSV("automatic_sequence.csv");
    Device::EndSession();
    Device::Disconnect();
    Device::Shutdown();
}

// === ПРИМЕР 4: МОНИТОРИНГ И СТАТИСТИКА ===
void Example_MonitoringAndStats()
{
    std::cout << "\n=== Example: Monitoring and Statistics ===" << std::endl;
    
    Device::Initialize("./logs");
    Device::SetStatusCallback(OnStatusUpdate);
    
    if (Device::Connect(3) != Device::SUCCESS)
    {
        std::cout << "Connection failed" << std::endl;
        Device::Shutdown();
        return;
    }
    
    // Получение информации о подключении
    std::cout << "Connection info: " << Device::GetConnectionInfo() << std::endl;
    
    // Выполняем несколько операций для накопления статистики
    Device::MeasurementParams params = {15.0f, 1000};
    Device::SetMode1_12V(params);
    Device::Stop();
    Device::SetMode2A(params);
    Device::Stop();
    
    // Получение статистики
    Device::Statistics stats = Device::GetStatistics();
    std::cout << "\nCommunication Statistics:" << std::endl;
    std::cout << "Commands sent: " << stats.commands_sent << std::endl;
    std::cout << "Responses received: " << stats.responses_received << std::endl;
    std::cout << "Errors count: " << stats.errors_count << std::endl;
    std::cout << "Connection quality: " << stats.connection_quality_percent << "%" << std::endl;
    std::cout << "Last response time: " << stats.last_response_time << " ms" << std::endl;
    
    // Проверка состояния устройства
    std::cout << "\nDevice Status:" << std::endl;
    std::cout << "Current mode: " << Device::GetCurrentMode() << std::endl;
    std::cout << "Is connected: " << (Device::IsConnected() ? "Yes" : "No") << std::endl;
    std::cout << "Is measuring: " << (Device::IsMeasuring() ? "Yes" : "No") << std::endl;
    std::cout << "Is paused: " << (Device::IsPaused() ? "Yes" : "No") << std::endl;
    
    Device::Disconnect();
    Device::Shutdown();
}

// === ПРИМЕР 5: ОБРАБОТКА ОШИБОК ===
void Example_ErrorHandling()
{
    std::cout << "\n=== Example: Error Handling ===" << std::endl;
    
    // Попытка использования без инициализации
    Device::OperationResult result = Device::Connect(3);
    std::cout << "Connect without init: " << Device::ResultToString(result) << std::endl;
    std::cout << "Last error: " << Device::GetLastError() << std::endl;
    
    Device::Initialize("./logs");
    
    // Попытка установки режима без подключения
    Device::MeasurementParams params = {10.0f, 1000};
    result = Device::SetMode1_12V(params);
    std::cout << "Mode without connection: " << Device::ResultToString(result) << std::endl;
    std::cout << "Last error: " << Device::GetLastError() << std::endl;
    
    // Попытка подключения к несуществующему порту
    result = Device::Connect(99);
    std::cout << "Connect to invalid port: " << Device::ResultToString(result) << std::endl;
    std::cout << "Last error: " << Device::GetLastError() << std::endl;
    
    // Попытка установки неверных параметров
    Device::MeasurementParams invalid_params = {1000.0f, 100000}; // Выходят за допустимые пределы
    if (Device::Connect(3) == Device::SUCCESS)
    {
        result = Device::SetMode1_12V(invalid_params);
        std::cout << "Invalid params: " << Device::ResultToString(result) << std::endl;
        std::cout << "Last error: " << Device::GetLastError() << std::endl;
        Device::Disconnect();
    }
    
    Device::Shutdown();
}

// === ГЛАВНАЯ ФУНКЦИЯ ПРИМЕРОВ ===
int main()
{
    std::cout << "Device API Examples\n" << std::endl;
    
    // Запустите любой из примеров
    Example_SimpleMode1_12V();
    Example_SessionManagement();
    Example_AutomaticSequence();
    Example_MonitoringAndStats();
    Example_ErrorHandling();
    
    std::cout << "\nAll examples completed." << std::endl;
    return 0;
}
