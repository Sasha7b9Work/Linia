// 2025/08/11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Panels/Panel.h"
#include "Device/DeviceCommunicator.h"

#pragma warning(push, 0)
#include <wx/wx.h>
#include <wx/combobox.h>
#include <wx/spinctrl.h>
#pragma warning(pop)

#include <memory>


// Компактная панель для управления связью с STM32 (встраивается в существующий интерфейс)
class Panel_STM32_Status : public Panel
{
public:
    Panel_STM32_Status(wxWindow* parent);
    virtual ~Panel_STM32_Status();
    
    static Panel_STM32_Status* self;

    void Update() override;

    // Получение коммуникатора для использования другими панелями
    DeviceCommunicator* GetCommunicator() const { return device_comm.get(); }

private:
    // UI элементы - Подключение
    wxStaticText* label_status;
    wxComboBox* combo_com_port;
    wxButton* button_connect;
    
    // UI элементы - Управление
    wxComboBox* combo_mode;
    wxSpinCtrlDouble* spin_voltage;
    wxSpinCtrl* spin_time;
    wxButton* button_start;
    wxButton* button_stop;
    
    // Устройство связи
    std::unique_ptr<DeviceCommunicator> device_comm;
    
    // Состояние
    bool is_initializing;
    
    // Обработчики событий
    void OnConnect(wxCommandEvent& event);
    void OnModeChanged(wxCommandEvent& event);
    void OnStart(wxCommandEvent& event);
    void OnStop(wxCommandEvent& event);
    
    // Колбэки от DeviceCommunicator
    void OnDeviceStateChanged(DeviceCommunicator::State old_state, DeviceCommunicator::State new_state);
    void OnDeviceError(const std::string& error_message);
    
    // Вспомогательные методы
    void InitializeUI();
    void RefreshComPorts();
    void UpdateControlsState();
    void ValidateAndStart();
    
    // Константы
    static const wxString MODE_NAMES[];
    static const float VOLTAGE_RANGES[];
    
    wxDECLARE_EVENT_TABLE();
};
