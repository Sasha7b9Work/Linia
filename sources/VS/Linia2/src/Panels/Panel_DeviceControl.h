// 2025/08/11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Panels/Panel.h"
#include "Device/DeviceCommunicator.h"
#include "Device/MeasurementLogger.h"

#pragma warning(push, 0)
#include <wx/wx.h>
#include <wx/spinctrl.h>
#include <wx/combobox.h>
#include <wx/stattext.h>
#include <wx/gauge.h>
#pragma warning(pop)

#include <memory>


// Панель управления устройством STM32
class Panel_DeviceControl : public Panel
{
public:
    Panel_DeviceControl(wxWindow* parent);
    virtual ~Panel_DeviceControl();

    void Update() override;

    // Обновление статуса устройства
    void UpdateDeviceStatus();
    void UpdateConnectionStatus();
    void UpdateMeasurementProgress();

private:
    // UI элементы - Подключение
    wxStaticText* label_connection_status;
    wxComboBox* combo_com_port;
    wxButton* button_connect;
    wxButton* button_disconnect;
    wxButton* button_refresh_ports;
    
    // UI элементы - Режимы измерения
    wxStaticText* label_measurement_mode;
    wxComboBox* combo_measurement_mode;
    
    // UI элементы - Параметры
    wxStaticText* label_voltage;
    wxSpinCtrlDouble* spin_voltage;
    wxStaticText* label_voltage_units;
    wxStaticText* label_voltage_range;
    
    wxStaticText* label_time;
    wxSpinCtrl* spin_time;
    wxStaticText* label_time_units;
    
    // UI элементы - Управление
    wxButton* button_start;
    wxButton* button_stop;
    wxButton* button_pause;
    wxButton* button_resume;
    
    // UI элементы - Статус
    wxStaticText* label_device_status;
    wxStaticText* label_device_mode;
    wxStaticText* label_last_response;
    wxGauge* gauge_connection_quality;
    
    // UI элементы - Логирование
    wxStaticText* label_current_session;
    wxButton* button_start_logging;
    wxButton* button_stop_logging;
    wxButton* button_save_session;
    wxButton* button_export_csv;
    wxButton* button_export_json;
    
    // UI элементы - Статистика
    wxStaticText* label_commands_sent;
    wxStaticText* label_responses_received;
    wxStaticText* label_errors_count;
    wxStaticText* label_data_received;
    
    // Устройство связи
    std::unique_ptr<DeviceCommunicator> device_comm;
    
    // Состояние
    bool is_measurement_active;
    std::string current_session_id;
    size_t total_data_received;
    
    // Обработчики событий
    void OnConnect(wxCommandEvent& event);
    void OnDisconnect(wxCommandEvent& event);
    void OnRefreshPorts(wxCommandEvent& event);
    void OnMeasurementModeChanged(wxCommandEvent& event);
    void OnVoltageChanged(wxSpinDoubleEvent& event);
    void OnTimeChanged(wxSpinEvent& event);
    void OnStart(wxCommandEvent& event);
    void OnStop(wxCommandEvent& event);
    void OnPause(wxCommandEvent& event);
    void OnResume(wxCommandEvent& event);
    void OnStartLogging(wxCommandEvent& event);
    void OnStopLogging(wxCommandEvent& event);
    void OnSaveSession(wxCommandEvent& event);
    void OnExportCsv(wxCommandEvent& event);
    void OnExportJson(wxCommandEvent& event);
    
    // Колбэки от DeviceCommunicator
    void OnDeviceStateChanged(DeviceCommunicator::State old_state, DeviceCommunicator::State new_state);
    void OnDeviceResponse(const Protocol::Response& response);
    void OnDeviceError(const std::string& error_message);
    void OnMeasurementData(const std::vector<uint8_t>& data);
    
    // Вспомогательные методы
    void InitializeUI();
    void RefreshComPorts();
    void UpdateVoltageRange();
    void UpdateTimeVisibility();
    void ValidateParameters();
    void EnableMeasurementControls(bool enable);
    void StartMeasurement();
    void GenerateSessionId();
    
    // Константы для UI
    enum MeasurementMode
    {
        MODE_1_12V = 0,
        MODE_1_24V,
        MODE_2A,
        MODE_3A,
        MODE_3B
    };
    
    static const wxString MODE_NAMES[];
    static const float VOLTAGE_RANGES[];
    static const bool TIME_REQUIRED[];
    
    wxDECLARE_EVENT_TABLE();
};
