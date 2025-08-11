// 2025/08/11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel_STM32_Status.h"
#include "Device/MeasurementLogger.h"
#include "MainWindow.h"
#include <wx/sizer.h>
#include <sstream>
#include <iomanip>

// Константы для режимов измерения
const wxString Panel_STM32_Status::MODE_NAMES[] = {
    wxT("1.12V"),
    wxT("1.24V"),
    wxT("2A"),
    wxT("3A"),
    wxT("3B")
};

const float Panel_STM32_Status::VOLTAGE_RANGES[] = {
    180.0f,  // MODE_1_12V
    720.0f,  // MODE_1_24V
    134.4f,  // MODE_2A
    360.0f,  // MODE_3A
    360.0f   // MODE_3B
};

// Event table
wxBEGIN_EVENT_TABLE(Panel_STM32_Status, Panel)
    EVT_BUTTON(2001, Panel_STM32_Status::OnConnect)
    EVT_COMBOBOX(2002, Panel_STM32_Status::OnModeChanged)
    EVT_BUTTON(2003, Panel_STM32_Status::OnStart)
    EVT_BUTTON(2004, Panel_STM32_Status::OnStop)
wxEND_EVENT_TABLE()

Panel_STM32_Status* Panel_STM32_Status::self = nullptr;

Panel_STM32_Status::Panel_STM32_Status(wxWindow* parent)
    : Panel(parent, 0, MainWindow::HEIGTH1, MainWindow::WIDTH1, 300)  // Размещаем под Panel01_Name
    , is_initializing(true)
{
    self = this;
    SetBackgroundColour(wxColour(250, 250, 250));
    
    // Инициализация DeviceCommunicator
    device_comm = std::make_unique<DeviceCommunicator>();
    device_comm->Initialize();
    
    // Настройка колбэков
    device_comm->SetOnStateChanged([this](DeviceCommunicator::State old_state, DeviceCommunicator::State new_state) {
        OnDeviceStateChanged(old_state, new_state);
    });
    
    device_comm->SetOnError([this](const std::string& error) {
        OnDeviceError(error);
    });
    
    // Инициализация логгера
    MeasurementLogger::Initialize("logs");
    
    InitializeUI();
    RefreshComPorts();
    
    is_initializing = false;
    UpdateControlsState();
}

Panel_STM32_Status::~Panel_STM32_Status()
{
    if (device_comm)
    {
        device_comm->Shutdown();
    }
    
    MeasurementLogger::Shutdown();
}

void Panel_STM32_Status::InitializeUI()
{
    wxBoxSizer* main_sizer = new wxBoxSizer(wxVERTICAL);
    
    // Заголовок
    wxStaticText* title = new wxStaticText(this, wxID_ANY, wxT("STM32 Управление"));
    title->SetFont(wxFont(8, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    main_sizer->Add(title, 0, wxALL | wxALIGN_CENTER, 2);
    
    // Статус подключения
    label_status = new wxStaticText(this, wxID_ANY, wxT("Отключено"));
    label_status->SetFont(wxFont(7, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
    label_status->SetForegroundColour(wxColour(128, 0, 0));
    main_sizer->Add(label_status, 0, wxALL | wxALIGN_CENTER, 2);
    
    // COM порт и подключение
    wxBoxSizer* port_sizer = new wxBoxSizer(wxHORIZONTAL);
    
    wxStaticText* port_label = new wxStaticText(this, wxID_ANY, wxT("Порт:"));
    port_label->SetFont(wxFont(7, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
    port_sizer->Add(port_label, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);
    
    combo_com_port = new wxComboBox(this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(70, -1), 0, nullptr, wxCB_READONLY);
    combo_com_port->SetFont(wxFont(7, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
    port_sizer->Add(combo_com_port, 0, wxALL, 2);
    
    button_connect = new wxButton(this, 2001, wxT("Подкл"), wxDefaultPosition, wxSize(50, 25));
    button_connect->SetFont(wxFont(7, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
    port_sizer->Add(button_connect, 0, wxALL, 2);
    
    main_sizer->Add(port_sizer, 0, wxEXPAND);
    
    // Режим измерения
    wxBoxSizer* mode_sizer = new wxBoxSizer(wxHORIZONTAL);
    
    wxStaticText* mode_label = new wxStaticText(this, wxID_ANY, wxT("Режим:"));
    mode_label->SetFont(wxFont(7, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
    mode_sizer->Add(mode_label, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);
    
    combo_mode = new wxComboBox(this, 2002, wxT(""), wxDefaultPosition, wxSize(60, -1), 
                               sizeof(MODE_NAMES)/sizeof(MODE_NAMES[0]), MODE_NAMES, wxCB_READONLY);
    combo_mode->SetFont(wxFont(7, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
    combo_mode->SetSelection(0);
    mode_sizer->Add(combo_mode, 0, wxALL, 2);
    
    main_sizer->Add(mode_sizer, 0, wxEXPAND);
    
    // Напряжение
    wxBoxSizer* voltage_sizer = new wxBoxSizer(wxHORIZONTAL);
    
    wxStaticText* voltage_label = new wxStaticText(this, wxID_ANY, wxT("U,В:"));
    voltage_label->SetFont(wxFont(7, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
    voltage_sizer->Add(voltage_label, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);
    
    spin_voltage = new wxSpinCtrlDouble(this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(60, -1));
    spin_voltage->SetFont(wxFont(7, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
    spin_voltage->SetRange(0.0, 180.0);
    spin_voltage->SetValue(12.0);
    spin_voltage->SetIncrement(0.1);
    spin_voltage->SetDigits(1);
    voltage_sizer->Add(spin_voltage, 0, wxALL, 2);
    
    main_sizer->Add(voltage_sizer, 0, wxEXPAND);
    
    // Время
    wxBoxSizer* time_sizer = new wxBoxSizer(wxHORIZONTAL);
    
    wxStaticText* time_label = new wxStaticText(this, wxID_ANY, wxT("T,мс:"));
    time_label->SetFont(wxFont(7, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
    time_sizer->Add(time_label, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);
    
    spin_time = new wxSpinCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(60, -1));
    spin_time->SetFont(wxFont(7, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
    spin_time->SetRange(1, 60000);
    spin_time->SetValue(1000);
    time_sizer->Add(spin_time, 0, wxALL, 2);
    
    main_sizer->Add(time_sizer, 0, wxEXPAND);
    
    // Кнопки управления
    wxBoxSizer* control_sizer = new wxBoxSizer(wxHORIZONTAL);
    
    button_start = new wxButton(this, 2003, wxT("Старт"), wxDefaultPosition, wxSize(50, 25));
    button_start->SetFont(wxFont(7, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
    button_start->SetBackgroundColour(wxColour(0, 255, 0));
    control_sizer->Add(button_start, 0, wxALL, 2);
    
    button_stop = new wxButton(this, 2004, wxT("Стоп"), wxDefaultPosition, wxSize(50, 25));
    button_stop->SetFont(wxFont(7, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
    button_stop->SetBackgroundColour(wxColour(255, 0, 0));
    control_sizer->Add(button_stop, 0, wxALL, 2);
    
    main_sizer->Add(control_sizer, 0, wxEXPAND);
    
    SetSizer(main_sizer);
}

void Panel_STM32_Status::Update()
{
    if (device_comm && !is_initializing)
    {
        device_comm->Update();
        UpdateControlsState();
    }
}

void Panel_STM32_Status::RefreshComPorts()
{
    combo_com_port->Clear();
    combo_com_port->Append(wxT("Авто"));
    
    for (int i = 0; i < 16; i++)
    {
        combo_com_port->Append(wxString::Format(wxT("COM%d"), i + 1));
    }
    
    combo_com_port->SetSelection(0);
}

void Panel_STM32_Status::UpdateControlsState()
{
    if (!device_comm) return;
    
    bool connected = device_comm->IsConnected();
    bool busy = device_comm->IsDeviceBusy();
    
    // Обновляем статус
    if (connected)
    {
        if (busy)
        {
            label_status->SetLabel(wxT("Измерение..."));
            label_status->SetForegroundColour(wxColour(0, 0, 255));
        }
        else
        {
            label_status->SetLabel(wxT("Подключено"));
            label_status->SetForegroundColour(wxColour(0, 128, 0));
        }
    }
    else
    {
        label_status->SetLabel(wxT("Отключено"));
        label_status->SetForegroundColour(wxColour(128, 0, 0));
    }
    
    // Обновляем кнопки
    button_connect->SetLabel(connected ? wxT("Откл") : wxT("Подкл"));
    button_start->Enable(connected && !busy);
    button_stop->Enable(connected && busy);
    
    // Обновляем параметры
    combo_mode->Enable(connected && !busy);
    spin_voltage->Enable(connected && !busy);
    spin_time->Enable(connected && !busy);
    
    // Обновляем диапазон напряжения
    int mode = combo_mode->GetSelection();
    if (mode >= 0 && mode < sizeof(VOLTAGE_RANGES)/sizeof(VOLTAGE_RANGES[0]))
    {
        float max_voltage = VOLTAGE_RANGES[mode];
        spin_voltage->SetRange(0.0, max_voltage);
        
        if (spin_voltage->GetValue() > max_voltage)
        {
            spin_voltage->SetValue(max_voltage / 2.0);
        }
    }
    
    Refresh();
}

void Panel_STM32_Status::ValidateAndStart()
{
    if (!device_comm || !device_comm->IsConnected()) return;
    
    int mode = combo_mode->GetSelection();
    float voltage = static_cast<float>(spin_voltage->GetValue());
    int time_ms = spin_time->GetValue();
    
    bool success = false;
    
    // Генерируем ID сеанса
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    
    std::ostringstream oss;
    oss << "measurement_" << std::put_time(std::localtime(&time_t), "%H%M%S");
    std::string session_id = oss.str();
    
    // Начинаем логирование
    Protocol::Command cmd = static_cast<Protocol::Command>(mode);
    MeasurementLogger::StartSession(session_id, cmd);
    
    // Отправляем команду
    switch (mode)
    {
    case 0: // MODE_1_12V
        success = device_comm->SendStartCommand1_12V(voltage, time_ms);
        break;
    case 1: // MODE_1_24V
        success = device_comm->SendStartCommand1_24V(voltage, time_ms);
        break;
    case 2: // MODE_2A
        success = device_comm->SendStartCommand2A(voltage, time_ms);
        break;
    case 3: // MODE_3A
        success = device_comm->SendStartCommand3A(voltage);
        break;
    case 4: // MODE_3B
        success = device_comm->SendStartCommand3B(voltage);
        break;
    }
    
    if (!success)
    {
        MeasurementLogger::EndSession();
        wxMessageBox(wxT("Ошибка запуска измерения"), wxT("Ошибка"), wxOK | wxICON_ERROR);
    }
}

// Event handlers
void Panel_STM32_Status::OnConnect(wxCommandEvent& event)
{
    if (!device_comm) return;
    
    if (device_comm->IsConnected())
    {
        device_comm->Disconnect();
        MeasurementLogger::LogConnection("User disconnected");
    }
    else
    {
        int port_selection = combo_com_port->GetSelection();
        int com_port = (port_selection == 0) ? -1 : (port_selection - 1);
        
        if (device_comm->Connect(com_port))
        {
            MeasurementLogger::LogConnection("User connected");
        }
    }
}

void Panel_STM32_Status::OnModeChanged(wxCommandEvent& event)
{
    UpdateControlsState();
}

void Panel_STM32_Status::OnStart(wxCommandEvent& event)
{
    ValidateAndStart();
}

void Panel_STM32_Status::OnStop(wxCommandEvent& event)
{
    if (device_comm)
    {
        device_comm->SendStopCommand();
        MeasurementLogger::EndSession();
        MeasurementLogger::LogCommand(Protocol::Command::STOP, "User stopped measurement");
    }
}

// Device callbacks
void Panel_STM32_Status::OnDeviceStateChanged(DeviceCommunicator::State old_state, DeviceCommunicator::State new_state)
{
    CallAfter([this]() {
        UpdateControlsState();
    });
    
    std::string old_str = std::to_string(static_cast<int>(old_state));
    std::string new_str = std::to_string(static_cast<int>(new_state));
    MeasurementLogger::LogStateChange(old_str, new_str);
}

void Panel_STM32_Status::OnDeviceError(const std::string& error_message)
{
    CallAfter([this, error_message]() {
        wxString wx_error = wxString(error_message);
        wxMessageBox(wx_error, wxT("Ошибка STM32"), wxOK | wxICON_ERROR);
    });
    
    MeasurementLogger::LogError(error_message);
}
