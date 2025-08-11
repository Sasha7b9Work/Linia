// 2025/08/11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel_DeviceControl.h"
#include "Utils/StringUtils.h"
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <sstream>
#include <iomanip>

// Константы для режимов измерения
const wxString Panel_DeviceControl::MODE_NAMES[] = {
    wxT("1.12V режим"),
    wxT("1.24V режим"),
    wxT("2A режим"),
    wxT("3A режим"),
    wxT("3B режим")
};

const float Panel_DeviceControl::VOLTAGE_RANGES[] = {
    180.0f,  // MODE_1_12V
    720.0f,  // MODE_1_24V
    134.4f,  // MODE_2A
    360.0f,  // MODE_3A
    360.0f   // MODE_3B
};

const bool Panel_DeviceControl::TIME_REQUIRED[] = {
    true,   // MODE_1_12V
    true,   // MODE_1_24V
    true,   // MODE_2A
    false,  // MODE_3A
    false   // MODE_3B
};

// Event table
wxBEGIN_EVENT_TABLE(Panel_DeviceControl, Panel)
    EVT_BUTTON(1001, Panel_DeviceControl::OnConnect)
    EVT_BUTTON(1002, Panel_DeviceControl::OnDisconnect)
    EVT_BUTTON(1003, Panel_DeviceControl::OnRefreshPorts)
    EVT_COMBOBOX(1004, Panel_DeviceControl::OnMeasurementModeChanged)
    EVT_SPINCTRLDOUBLE(1005, Panel_DeviceControl::OnVoltageChanged)
    EVT_SPINCTRL(1006, Panel_DeviceControl::OnTimeChanged)
    EVT_BUTTON(1007, Panel_DeviceControl::OnStart)
    EVT_BUTTON(1008, Panel_DeviceControl::OnStop)
    EVT_BUTTON(1009, Panel_DeviceControl::OnPause)
    EVT_BUTTON(1010, Panel_DeviceControl::OnResume)
    EVT_BUTTON(1011, Panel_DeviceControl::OnStartLogging)
    EVT_BUTTON(1012, Panel_DeviceControl::OnStopLogging)
    EVT_BUTTON(1013, Panel_DeviceControl::OnSaveSession)
    EVT_BUTTON(1014, Panel_DeviceControl::OnExportCsv)
    EVT_BUTTON(1015, Panel_DeviceControl::OnExportJson)
wxEND_EVENT_TABLE()

Panel_DeviceControl::Panel_DeviceControl(wxWindow* parent)
    : Panel(parent)
    , is_measurement_active(false)
    , total_data_received(0)
{
    SetBackgroundColour(wxColour(240, 240, 240));
    
    // Инициализация DeviceCommunicator
    device_comm = std::make_unique<DeviceCommunicator>();
    device_comm->Initialize();
    
    // Настройка колбэков
    device_comm->SetOnStateChanged([this](DeviceCommunicator::State old_state, DeviceCommunicator::State new_state) {
        OnDeviceStateChanged(old_state, new_state);
    });
    
    device_comm->SetOnResponse([this](const Protocol::Response& response) {
        OnDeviceResponse(response);
    });
    
    device_comm->SetOnError([this](const std::string& error) {
        OnDeviceError(error);
    });
    
    device_comm->SetOnMeasurementData([this](const std::vector<uint8_t>& data) {
        OnMeasurementData(data);
    });
    
    // Инициализация логгера
    MeasurementLogger::Initialize("logs");
    
    InitializeUI();
    RefreshComPorts();
    UpdateDeviceStatus();
}

Panel_DeviceControl::~Panel_DeviceControl()
{
    if (device_comm)
    {
        device_comm->Shutdown();
    }
    
    MeasurementLogger::Shutdown();
}

void Panel_DeviceControl::InitializeUI()
{
    wxBoxSizer* main_sizer = new wxBoxSizer(wxVERTICAL);
    
    // --- Секция подключения ---
    wxStaticBoxSizer* connection_box = new wxStaticBoxSizer(wxVERTICAL, this, wxT("Подключение к устройству"));
    
    // Статус подключения
    label_connection_status = new wxStaticText(this, wxID_ANY, wxT("Статус: Отключено"));
    label_connection_status->SetFont(wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    connection_box->Add(label_connection_status, 0, wxALL, 5);
    
    // Выбор COM порта
    wxBoxSizer* port_sizer = new wxBoxSizer(wxHORIZONTAL);
    port_sizer->Add(new wxStaticText(this, wxID_ANY, wxT("COM порт:")), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    
    combo_com_port = new wxComboBox(this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(100, -1), 0, nullptr, wxCB_READONLY);
    port_sizer->Add(combo_com_port, 0, wxALL, 5);
    
    button_refresh_ports = new wxButton(this, 1003, wxT("Обновить"), wxDefaultPosition, wxSize(80, -1));
    port_sizer->Add(button_refresh_ports, 0, wxALL, 5);
    
    connection_box->Add(port_sizer, 0, wxEXPAND);
    
    // Кнопки подключения
    wxBoxSizer* connect_sizer = new wxBoxSizer(wxHORIZONTAL);
    button_connect = new wxButton(this, 1001, wxT("Подключить"), wxDefaultPosition, wxSize(100, -1));
    button_disconnect = new wxButton(this, 1002, wxT("Отключить"), wxDefaultPosition, wxSize(100, -1));
    button_disconnect->Enable(false);
    
    connect_sizer->Add(button_connect, 0, wxALL, 5);
    connect_sizer->Add(button_disconnect, 0, wxALL, 5);
    connection_box->Add(connect_sizer, 0, wxEXPAND);
    
    // Качество соединения
    wxBoxSizer* quality_sizer = new wxBoxSizer(wxHORIZONTAL);
    quality_sizer->Add(new wxStaticText(this, wxID_ANY, wxT("Качество связи:")), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    gauge_connection_quality = new wxGauge(this, wxID_ANY, 100, wxDefaultPosition, wxSize(100, 20));
    quality_sizer->Add(gauge_connection_quality, 0, wxALL, 5);
    connection_box->Add(quality_sizer, 0, wxEXPAND);
    
    main_sizer->Add(connection_box, 0, wxEXPAND | wxALL, 5);
    
    // --- Секция параметров измерения ---
    wxStaticBoxSizer* measurement_box = new wxStaticBoxSizer(wxVERTICAL, this, wxT("Параметры измерения"));
    
    // Режим измерения
    wxBoxSizer* mode_sizer = new wxBoxSizer(wxHORIZONTAL);
    label_measurement_mode = new wxStaticText(this, wxID_ANY, wxT("Режим:"));
    mode_sizer->Add(label_measurement_mode, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    
    combo_measurement_mode = new wxComboBox(this, 1004, wxT(""), wxDefaultPosition, wxSize(150, -1), 
                                          sizeof(MODE_NAMES)/sizeof(MODE_NAMES[0]), MODE_NAMES, wxCB_READONLY);
    combo_measurement_mode->SetSelection(0);
    mode_sizer->Add(combo_measurement_mode, 0, wxALL, 5);
    measurement_box->Add(mode_sizer, 0, wxEXPAND);
    
    // Напряжение
    wxBoxSizer* voltage_sizer = new wxBoxSizer(wxHORIZONTAL);
    label_voltage = new wxStaticText(this, wxID_ANY, wxT("Напряжение:"));
    voltage_sizer->Add(label_voltage, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    
    spin_voltage = new wxSpinCtrlDouble(this, 1005, wxT(""), wxDefaultPosition, wxSize(100, -1));
    spin_voltage->SetRange(0.0, 180.0);
    spin_voltage->SetValue(12.0);
    spin_voltage->SetIncrement(0.1);
    spin_voltage->SetDigits(1);
    voltage_sizer->Add(spin_voltage, 0, wxALL, 5);
    
    label_voltage_units = new wxStaticText(this, wxID_ANY, wxT("В"));
    voltage_sizer->Add(label_voltage_units, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    
    label_voltage_range = new wxStaticText(this, wxID_ANY, wxT("(0.0 - 180.0 В)"));
    label_voltage_range->SetFont(wxFont(8, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_ITALIC, wxFONTWEIGHT_NORMAL));
    voltage_sizer->Add(label_voltage_range, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    
    measurement_box->Add(voltage_sizer, 0, wxEXPAND);
    
    // Время
    wxBoxSizer* time_sizer = new wxBoxSizer(wxHORIZONTAL);
    label_time = new wxStaticText(this, wxID_ANY, wxT("Время:"));
    time_sizer->Add(label_time, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    
    spin_time = new wxSpinCtrl(this, 1006, wxT(""), wxDefaultPosition, wxSize(100, -1));
    spin_time->SetRange(1, 60000);
    spin_time->SetValue(1000);
    time_sizer->Add(spin_time, 0, wxALL, 5);
    
    label_time_units = new wxStaticText(this, wxID_ANY, wxT("мс"));
    time_sizer->Add(label_time_units, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    
    measurement_box->Add(time_sizer, 0, wxEXPAND);
    
    main_sizer->Add(measurement_box, 0, wxEXPAND | wxALL, 5);
    
    // --- Секция управления ---
    wxStaticBoxSizer* control_box = new wxStaticBoxSizer(wxVERTICAL, this, wxT("Управление измерением"));
    
    wxBoxSizer* control_sizer = new wxBoxSizer(wxHORIZONTAL);
    button_start = new wxButton(this, 1007, wxT("Старт"), wxDefaultPosition, wxSize(80, -1));
    button_stop = new wxButton(this, 1008, wxT("Стоп"), wxDefaultPosition, wxSize(80, -1));
    button_pause = new wxButton(this, 1009, wxT("Пауза"), wxDefaultPosition, wxSize(80, -1));
    button_resume = new wxButton(this, 1010, wxT("Продолжить"), wxDefaultPosition, wxSize(90, -1));
    
    button_start->SetBackgroundColour(wxColour(0, 255, 0));
    button_stop->SetBackgroundColour(wxColour(255, 0, 0));
    button_pause->SetBackgroundColour(wxColour(255, 255, 0));
    button_resume->SetBackgroundColour(wxColour(0, 255, 255));
    
    control_sizer->Add(button_start, 0, wxALL, 5);
    control_sizer->Add(button_stop, 0, wxALL, 5);
    control_sizer->Add(button_pause, 0, wxALL, 5);
    control_sizer->Add(button_resume, 0, wxALL, 5);
    
    control_box->Add(control_sizer, 0, wxEXPAND);
    main_sizer->Add(control_box, 0, wxEXPAND | wxALL, 5);
    
    // --- Секция статуса ---
    wxStaticBoxSizer* status_box = new wxStaticBoxSizer(wxVERTICAL, this, wxT("Статус устройства"));
    
    label_device_status = new wxStaticText(this, wxID_ANY, wxT("Состояние: Отключено"));
    status_box->Add(label_device_status, 0, wxALL, 5);
    
    label_device_mode = new wxStaticText(this, wxID_ANY, wxT("Режим: Простой"));
    status_box->Add(label_device_mode, 0, wxALL, 5);
    
    label_last_response = new wxStaticText(this, wxID_ANY, wxT("Последний ответ: нет"));
    status_box->Add(label_last_response, 0, wxALL, 5);
    
    main_sizer->Add(status_box, 0, wxEXPAND | wxALL, 5);
    
    // --- Секция логирования ---
    wxStaticBoxSizer* logging_box = new wxStaticBoxSizer(wxVERTICAL, this, wxT("Логирование"));
    
    label_current_session = new wxStaticText(this, wxID_ANY, wxT("Сеанс: не активен"));
    logging_box->Add(label_current_session, 0, wxALL, 5);
    
    wxBoxSizer* logging_sizer = new wxBoxSizer(wxHORIZONTAL);
    button_start_logging = new wxButton(this, 1011, wxT("Начать"), wxDefaultPosition, wxSize(80, -1));
    button_stop_logging = new wxButton(this, 1012, wxT("Закончить"), wxDefaultPosition, wxSize(80, -1));
    button_save_session = new wxButton(this, 1013, wxT("Сохранить"), wxDefaultPosition, wxSize(80, -1));
    
    logging_sizer->Add(button_start_logging, 0, wxALL, 5);
    logging_sizer->Add(button_stop_logging, 0, wxALL, 5);
    logging_sizer->Add(button_save_session, 0, wxALL, 5);
    logging_box->Add(logging_sizer, 0, wxEXPAND);
    
    wxBoxSizer* export_sizer = new wxBoxSizer(wxHORIZONTAL);
    button_export_csv = new wxButton(this, 1014, wxT("Экспорт CSV"), wxDefaultPosition, wxSize(90, -1));
    button_export_json = new wxButton(this, 1015, wxT("Экспорт JSON"), wxDefaultPosition, wxSize(90, -1));
    
    export_sizer->Add(button_export_csv, 0, wxALL, 5);
    export_sizer->Add(button_export_json, 0, wxALL, 5);
    logging_box->Add(export_sizer, 0, wxEXPAND);
    
    main_sizer->Add(logging_box, 0, wxEXPAND | wxALL, 5);
    
    // --- Секция статистики ---
    wxStaticBoxSizer* stats_box = new wxStaticBoxSizer(wxVERTICAL, this, wxT("Статистика"));
    
    label_commands_sent = new wxStaticText(this, wxID_ANY, wxT("Команд отправлено: 0"));
    label_responses_received = new wxStaticText(this, wxID_ANY, wxT("Ответов получено: 0"));
    label_errors_count = new wxStaticText(this, wxID_ANY, wxT("Ошибок: 0"));
    label_data_received = new wxStaticText(this, wxID_ANY, wxT("Данных получено: 0 байт"));
    
    stats_box->Add(label_commands_sent, 0, wxALL, 5);
    stats_box->Add(label_responses_received, 0, wxALL, 5);
    stats_box->Add(label_errors_count, 0, wxALL, 5);
    stats_box->Add(label_data_received, 0, wxALL, 5);
    
    main_sizer->Add(stats_box, 0, wxEXPAND | wxALL, 5);
    
    SetSizer(main_sizer);
    
    // Инициализация состояния
    EnableMeasurementControls(false);
    UpdateVoltageRange();
    UpdateTimeVisibility();
}

void Panel_DeviceControl::Update()
{
    if (device_comm)
    {
        device_comm->Update();
    }
    
    UpdateDeviceStatus();
    UpdateConnectionStatus();
    UpdateMeasurementProgress();
}

void Panel_DeviceControl::RefreshComPorts()
{
    combo_com_port->Clear();
    combo_com_port->Append(wxT("Автопоиск"));
    
    for (int i = 0; i < 32; i++)
    {
        combo_com_port->Append(wxString::Format(wxT("COM%d"), i + 1));
    }
    
    combo_com_port->SetSelection(0);
}

void Panel_DeviceControl::UpdateVoltageRange()
{
    int mode = combo_measurement_mode->GetSelection();
    if (mode >= 0 && mode < sizeof(VOLTAGE_RANGES)/sizeof(VOLTAGE_RANGES[0]))
    {
        float max_voltage = VOLTAGE_RANGES[mode];
        spin_voltage->SetRange(0.0, max_voltage);
        
        wxString range_text = wxString::Format(wxT("(0.0 - %.1f В)"), max_voltage);
        label_voltage_range->SetLabel(range_text);
        
        if (spin_voltage->GetValue() > max_voltage)
        {
            spin_voltage->SetValue(max_voltage / 2.0);
        }
    }
}

void Panel_DeviceControl::UpdateTimeVisibility()
{
    int mode = combo_measurement_mode->GetSelection();
    if (mode >= 0 && mode < sizeof(TIME_REQUIRED)/sizeof(TIME_REQUIRED[0]))
    {
        bool show_time = TIME_REQUIRED[mode];
        
        label_time->Show(show_time);
        spin_time->Show(show_time);
        label_time_units->Show(show_time);
        
        Layout();
    }
}

void Panel_DeviceControl::ValidateParameters()
{
    int mode = combo_measurement_mode->GetSelection();
    float voltage = static_cast<float>(spin_voltage->GetValue());
    int time_ms = spin_time->GetValue();
    
    bool valid = false;
    
    switch (mode)
    {
    case MODE_1_12V:
        valid = DeviceCommunicator::ValidateParameters1_12V(voltage, time_ms);
        break;
    case MODE_1_24V:
        valid = DeviceCommunicator::ValidateParameters1_24V(voltage, time_ms);
        break;
    case MODE_2A:
        valid = DeviceCommunicator::ValidateParameters2A(voltage, time_ms);
        break;
    case MODE_3A:
        valid = DeviceCommunicator::ValidateParameters3A(voltage);
        break;
    case MODE_3B:
        valid = DeviceCommunicator::ValidateParameters3B(voltage);
        break;
    }
    
    button_start->Enable(valid && device_comm && device_comm->IsConnected() && !is_measurement_active);
}

void Panel_DeviceControl::EnableMeasurementControls(bool enable)
{
    combo_measurement_mode->Enable(enable);
    spin_voltage->Enable(enable);
    spin_time->Enable(enable);
    
    ValidateParameters();
}

void Panel_DeviceControl::UpdateDeviceStatus()
{
    if (!device_comm) return;
    
    // Обновляем статус устройства
    wxString status_text = wxString::Format(wxT("Состояние: %s"), 
                                          wxString(device_comm->GetStateString()));
    label_device_status->SetLabel(status_text);
    
    wxString mode_text = wxString::Format(wxT("Режим: %s"), 
                                        wxString(device_comm->GetDeviceModeString()));
    label_device_mode->SetLabel(mode_text);
    
    // Обновляем статистику
    wxString commands_text = wxString::Format(wxT("Команд отправлено: %d"), 
                                            device_comm->GetSentCommandsCount());
    label_commands_sent->SetLabel(commands_text);
    
    wxString responses_text = wxString::Format(wxT("Ответов получено: %d"), 
                                             device_comm->GetReceivedResponsesCount());
    label_responses_received->SetLabel(responses_text);
    
    wxString errors_text = wxString::Format(wxT("Ошибок: %d"), 
                                          device_comm->GetErrorsCount());
    label_errors_count->SetLabel(errors_text);
    
    wxString data_text = wxString::Format(wxT("Данных получено: %zu байт"), 
                                        total_data_received);
    label_data_received->SetLabel(data_text);
    
    // Обновляем время последнего ответа
    auto last_response = device_comm->GetLastResponseTime();
    if (last_response.count() > 0)
    {
        wxString response_text = wxString::Format(wxT("Последний ответ: %lld мс назад"), 
                                                last_response.count());
        label_last_response->SetLabel(response_text);
    }
}

void Panel_DeviceControl::UpdateConnectionStatus()
{
    if (!device_comm) return;
    
    bool connected = device_comm->IsConnected();
    
    wxString status_text = connected ? wxT("Статус: Подключено") : wxT("Статус: Отключено");
    label_connection_status->SetLabel(status_text);
    
    if (connected)
    {
        label_connection_status->SetForegroundColour(wxColour(0, 128, 0));
    }
    else
    {
        label_connection_status->SetForegroundColour(wxColour(128, 0, 0));
    }
    
    button_connect->Enable(!connected);
    button_disconnect->Enable(connected);
    
    EnableMeasurementControls(connected);
    
    // Обновляем качество соединения (упрощенный алгоритм)
    int quality = 0;
    if (connected)
    {
        int sent = device_comm->GetSentCommandsCount();
        int received = device_comm->GetReceivedResponsesCount();
        int errors = device_comm->GetErrorsCount();
        
        if (sent > 0)
        {
            quality = static_cast<int>((static_cast<float>(received) / sent) * 100);
            quality = std::max(0, quality - errors * 10);
        }
        else
        {
            quality = 100;
        }
    }
    
    gauge_connection_quality->SetValue(quality);
}

void Panel_DeviceControl::UpdateMeasurementProgress()
{
    is_measurement_active = device_comm && device_comm->IsDeviceBusy();
    
    button_stop->Enable(is_measurement_active);
    button_pause->Enable(is_measurement_active);
    button_resume->Enable(device_comm && device_comm->GetDeviceMode() == DeviceCommunicator::PAUSED);
    
    ValidateParameters();
}

void Panel_DeviceControl::GenerateSessionId()
{
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    
    std::ostringstream oss;
    oss << "session_" << std::put_time(std::localtime(&time_t), "%Y%m%d_%H%M%S");
    
    current_session_id = oss.str();
}

// Event handlers
void Panel_DeviceControl::OnConnect(wxCommandEvent& event)
{
    if (!device_comm) return;
    
    int port_selection = combo_com_port->GetSelection();
    int com_port = (port_selection == 0) ? -1 : (port_selection - 1);
    
    if (device_comm->Connect(com_port))
    {
        MeasurementLogger::LogConnection("Connected to device");
    }
}

void Panel_DeviceControl::OnDisconnect(wxCommandEvent& event)
{
    if (device_comm)
    {
        device_comm->Disconnect();
        MeasurementLogger::LogConnection("Disconnected from device");
    }
}

void Panel_DeviceControl::OnRefreshPorts(wxCommandEvent& event)
{
    RefreshComPorts();
}

void Panel_DeviceControl::OnMeasurementModeChanged(wxCommandEvent& event)
{
    UpdateVoltageRange();
    UpdateTimeVisibility();
    ValidateParameters();
}

void Panel_DeviceControl::OnVoltageChanged(wxSpinDoubleEvent& event)
{
    ValidateParameters();
}

void Panel_DeviceControl::OnTimeChanged(wxSpinEvent& event)
{
    ValidateParameters();
}

void Panel_DeviceControl::OnStart(wxCommandEvent& event)
{
    StartMeasurement();
}

void Panel_DeviceControl::OnStop(wxCommandEvent& event)
{
    if (device_comm)
    {
        device_comm->SendStopCommand();
        MeasurementLogger::LogCommand(Protocol::Command::STOP, "User initiated stop");
    }
}

void Panel_DeviceControl::OnPause(wxCommandEvent& event)
{
    if (device_comm)
    {
        device_comm->SendPauseCommand();
        MeasurementLogger::LogCommand(Protocol::Command::PAUSE, "User initiated pause");
    }
}

void Panel_DeviceControl::OnResume(wxCommandEvent& event)
{
    if (device_comm)
    {
        device_comm->SendResumeCommand();
        MeasurementLogger::LogCommand(Protocol::Command::RESUME, "User initiated resume");
    }
}

void Panel_DeviceControl::OnStartLogging(wxCommandEvent& event)
{
    GenerateSessionId();
    
    Protocol::Command cmd = static_cast<Protocol::Command>(combo_measurement_mode->GetSelection());
    MeasurementLogger::StartSession(current_session_id, cmd);
    
    wxString session_text = wxString::Format(wxT("Сеанс: %s (активен)"), wxString(current_session_id));
    label_current_session->SetLabel(session_text);
    
    button_start_logging->Enable(false);
    button_stop_logging->Enable(true);
}

void Panel_DeviceControl::OnStopLogging(wxCommandEvent& event)
{
    MeasurementLogger::EndSession();
    
    label_current_session->SetLabel(wxT("Сеанс: не активен"));
    
    button_start_logging->Enable(true);
    button_stop_logging->Enable(false);
}

void Panel_DeviceControl::OnSaveSession(wxCommandEvent& event)
{
    wxFileDialog saveFileDialog(this,
                               wxT("Сохранить сеанс измерений"),
                               wxT(""),
                               wxString(current_session_id) + wxT(".log"),
                               wxT("Log files (*.log)|*.log"),
                               wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
    
    if (saveFileDialog.ShowModal() == wxID_CANCEL)
        return;
    
    std::string filename = saveFileDialog.GetPath().ToStdString();
    if (MeasurementLogger::SaveCurrentSession(filename))
    {
        wxMessageBox(wxT("Сеанс успешно сохранен"), wxT("Сохранение"), wxOK | wxICON_INFORMATION);
    }
    else
    {
        wxMessageBox(wxT("Ошибка при сохранении сеанса"), wxT("Ошибка"), wxOK | wxICON_ERROR);
    }
}

void Panel_DeviceControl::OnExportCsv(wxCommandEvent& event)
{
    wxFileDialog saveFileDialog(this,
                               wxT("Экспорт в CSV"),
                               wxT(""),
                               wxString(current_session_id) + wxT(".csv"),
                               wxT("CSV files (*.csv)|*.csv"),
                               wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
    
    if (saveFileDialog.ShowModal() == wxID_CANCEL)
        return;
    
    std::string filename = saveFileDialog.GetPath().ToStdString();
    if (MeasurementLogger::ExportCurrentSessionToCsv(filename))
    {
        wxMessageBox(wxT("Данные экспортированы в CSV"), wxT("Экспорт"), wxOK | wxICON_INFORMATION);
    }
    else
    {
        wxMessageBox(wxT("Ошибка при экспорте в CSV"), wxT("Ошибка"), wxOK | wxICON_ERROR);
    }
}

void Panel_DeviceControl::OnExportJson(wxCommandEvent& event)
{
    wxFileDialog saveFileDialog(this,
                               wxT("Экспорт в JSON"),
                               wxT(""),
                               wxString(current_session_id) + wxT(".json"),
                               wxT("JSON files (*.json)|*.json"),
                               wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
    
    if (saveFileDialog.ShowModal() == wxID_CANCEL)
        return;
    
    std::string filename = saveFileDialog.GetPath().ToStdString();
    if (MeasurementLogger::ExportCurrentSessionToJson(filename))
    {
        wxMessageBox(wxT("Данные экспортированы в JSON"), wxT("Экспорт"), wxOK | wxICON_INFORMATION);
    }
    else
    {
        wxMessageBox(wxT("Ошибка при экспорте в JSON"), wxT("Ошибка"), wxOK | wxICON_ERROR);
    }
}

void Panel_DeviceControl::StartMeasurement()
{
    if (!device_comm || !device_comm->IsConnected()) return;
    
    int mode = combo_measurement_mode->GetSelection();
    float voltage = static_cast<float>(spin_voltage->GetValue());
    int time_ms = spin_time->GetValue();
    
    bool success = false;
    std::string details = wxString::Format(wxT("V=%.1f, T=%d"), voltage, time_ms).ToStdString();
    
    switch (mode)
    {
    case MODE_1_12V:
        success = device_comm->SendStartCommand1_12V(voltage, time_ms);
        MeasurementLogger::LogCommand(Protocol::Command::START_1_12V, details);
        break;
    case MODE_1_24V:
        success = device_comm->SendStartCommand1_24V(voltage, time_ms);
        MeasurementLogger::LogCommand(Protocol::Command::START_1_24V, details);
        break;
    case MODE_2A:
        success = device_comm->SendStartCommand2A(voltage, time_ms);
        MeasurementLogger::LogCommand(Protocol::Command::START_2A, details);
        break;
    case MODE_3A:
        success = device_comm->SendStartCommand3A(voltage);
        MeasurementLogger::LogCommand(Protocol::Command::START_3A, details);
        break;
    case MODE_3B:
        success = device_comm->SendStartCommand3B(voltage);
        MeasurementLogger::LogCommand(Protocol::Command::START_3B, details);
        break;
    }
    
    if (!success)
    {
        wxMessageBox(wxT("Ошибка при запуске измерения"), wxT("Ошибка"), wxOK | wxICON_ERROR);
    }
}

// Device callbacks
void Panel_DeviceControl::OnDeviceStateChanged(DeviceCommunicator::State old_state, DeviceCommunicator::State new_state)
{
    std::string message = "State changed: " + std::to_string(static_cast<int>(old_state)) + 
                         " -> " + std::to_string(static_cast<int>(new_state));
    MeasurementLogger::LogStateChange(std::to_string(static_cast<int>(old_state)), 
                                    std::to_string(static_cast<int>(new_state)));
}

void Panel_DeviceControl::OnDeviceResponse(const Protocol::Response& response)
{
    MeasurementLogger::LogResponse(response);
}

void Panel_DeviceControl::OnDeviceError(const std::string& error_message)
{
    MeasurementLogger::LogError(error_message);
    
    wxString wx_error = wxString(error_message);
    wxMessageBox(wx_error, wxT("Ошибка устройства"), wxOK | wxICON_ERROR);
}

void Panel_DeviceControl::OnMeasurementData(const std::vector<uint8_t>& data)
{
    total_data_received += data.size();
    MeasurementLogger::LogData(data);
}
