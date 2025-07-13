// 2025/6/1 17:20:45 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel03_Config.h"
#include "MainWindow.h"
#include "Utils/GlobalFunctions.h"
#include "Utils/SystemDepend.h"


PanelConfig *PanelConfig::self = nullptr;


PanelConfig::PanelConfig(wxWindow* parent) :
    Panel(parent, MainWindow::WIDTH1 + MainWindow::WIDTH2, 0, MainWindow::WIDTH3, HEIGHT)
{
    self = this;

    Bind(wxEVT_TOGGLEBUTTON, &PanelConfig::OnEventButton, this);
    Bind(wxEVT_RADIOBUTTON, &PanelConfig::OnEventRadioButton, this);

    const int h = 20;

    wxSize sizeChan = { 67, h };

    wxToggleButton *button = new wxToggleButton(this, ID_PAN3_BTN_CHANNEL_C, _L("Канал C"), { 0, 0 }, sizeChan);
    str_panels.push_back({ button, CreatePanel(button) });

    button = new wxToggleButton(this, ID_PAN3_BTN_CHANNEL_B, _L("Канал B"), { sizeChan.x, 0 }, sizeChan);
    str_panels.push_back({ button, CreatePanel(button) });

    button = new wxToggleButton(this, ID_PAN3_BTN_CHANNEL_S, _L("Канал S"), { sizeChan.x * 2, 0 }, sizeChan);
    str_panels.push_back({ button, CreatePanel(button) });

    wxSize sizeScheme = { 120, h };
    button = new wxToggleButton(this, ID_PAN3_BTN_SCHEME, _L("Схема включения"), { 0, h }, sizeScheme);
    str_panels.push_back({ button, CreatePanel(button) });

    button = new wxToggleButton(this, ID_PAN3_BTN_CALCULATION, _L("Расчёт"), { sizeScheme.x, h }, { MainWindow::WIDTH3 - sizeScheme.x - 3, h });
    str_panels.push_back({ button, CreatePanel(button) });

    {
        // Включаем панель

        int id = ID_PAN3_BTN_CHANNEL_C;

        wxCommandEvent evt(wxEVT_TOGGLEBUTTON, id);
        evt.SetInt(1);
        this->ProcessWindowEvent(evt);
    }
}

wxPanel *PanelConfig::CreatePanel(wxToggleButton *button)
{
    wxPanel *panel = new wxPanel(self);

    panel->SetSize({ MainWindow::WIDTH3, HEIGHT - 40 });
    panel->SetPosition({ 0, 40 });

    int x = 4;
    int w = MainWindow::WIDTH3 - 2 * x - 1;

    int id = button->GetId();

    if (id == ID_PAN3_BTN_CHANNEL_C)
    {
        wxStaticBox *boxScan = new wxStaticBox(panel, wxID_ANY, _L("Развёртка"), { x, 0 }, { w, 160 });

        {
            wxSize size_rb{ 50, 15 };

            new wxRadioButton(boxScan, ID_PAN3_CHAN_C_RB_SCAN_1, "1", { 10, SD::Y_SB(20) }, size_rb);
            new wxRadioButton(boxScan, ID_PAN3_CHAN_C_RB_SCAN_2, "2", { 70, SD::Y_SB(20) }, size_rb);

            new wxRadioButton(boxScan, ID_PAN3_CHAN_C_RB_SCAN_3, "3", { 10, SD::Y_SB(50) }, size_rb);
            new wxRadioButton(boxScan, ID_PAN3_CHAN_C_RB_SCAN_4, "4", { 70, SD::Y_SB(50) }, size_rb);

            new wxRadioButton(boxScan, ID_PAN3_CHAN_C_RB_SCAN_5, "5", { 10, SD::Y_SB(80) }, size_rb);
            new wxRadioButton(boxScan, ID_PAN3_CHAN_C_RB_SCAN_6, "6", { 70, SD::Y_SB(80) }, size_rb);

            new wxRadioButton(boxScan, ID_PAN3_CHAN_C_RB_SCAN_7, "7", { 130, SD::Y_SB(20) }, size_rb);

            {
                wxCommandEvent evt(wxEVT_RADIOBUTTON, ID_PAN3_CHAN_C_RB_SCAN_1);
                evt.SetInt(1);
                boxScan->ProcessWindowEvent(evt);
            }

            {
                wxStaticBox *boxImpulse = new wxStaticBox(boxScan, wxID_ANY, _L("Импульс"), { 10, SD::Y_SB(110) }, { 75, 40 });

                new wxStaticText(boxImpulse, wxID_ANY, "0.2 ms", { 10, SD::Y_SB(20) });
            }

            new wxCheckBox(boxScan, ID_PAN3_CHAN_C_CHECKBOX_DUTY_CYCLE, _L("Скважн. x 2"), { 100, SD::Y_SB(120) }, { 100, 20 });
        }

        wxStaticBox *boxMeter = new wxStaticBox(panel, wxID_ANY, _L("Измеритель"), { x, boxScan->GetSize().y + x }, { w, 50 } );

        {
            int y = 20;
            int dY = 5;

            new wxStaticText(boxMeter, wxID_ANY, "Uc", { 10, SD::Y_SB(y + dY) });
            new wxStaticText(boxMeter, wxID_ANY, "Ic", { 100, SD::Y_SB(y + dY) });

            wxArrayString choices;
            choices.Add("100V");

            new wxComboBox(boxMeter, ID_PAN3_CHAN_C_COMBOBOX_Uc, choices[0], { 30, SD::Y_SB(y) }, { 60, TEXTCNTRL_HEIGHT }, choices, wxCB_READONLY);

            choices.Clear();
            choices.Add("10A");

            new wxComboBox(boxMeter, ID_PAN3_CHAN_C_COMBOBOX_Ic, choices[0], { 120, SD::Y_SB(y) }, { 60, TEXTCNTRL_HEIGHT }, choices, wxCB_READONLY);
        }

        new wxStaticBox(panel, wxID_ANY, _L("Источник U"), { x, boxMeter->GetPosition().y + boxMeter->GetSize().y + x }, { w, 50 });
    }
    else if (id == ID_PAN3_BTN_CHANNEL_B)
    {

    }
    else if (id == ID_PAN3_BTN_CHANNEL_S)
    {

    }
    else if (id == ID_PAN3_BTN_SCHEME)
    {

    }
    else if (id == ID_PAN3_BTN_CALCULATION)
    {

    }

    return panel;
}


void PanelConfig::OnEventButton(wxCommandEvent &event)
{
    if (!event.IsChecked())
    {
        GF::FindToggleButton(self, event.GetId())->SetValue(true);
    }
    else
    {
        UnсheckAllAcross(event.GetId());

        EnablePanel(event.GetId());
    }
}


void PanelConfig::OnEventRadioButton(wxCommandEvent &event)
{
    GF::FindRadioButton(self, event.GetId())->SetValue(true);

    event.Skip();
}


void PanelConfig::UnсheckAllAcross(int id)
{
    for (auto &str : str_panels)
    {
        str.button->SetValue(str.button->GetId() == id);
    }
}


void PanelConfig::EnablePanel(int button_id)
{
    for (auto &str : str_panels)
    {
        str.panel->Show(str.button->GetId() == button_id);
    }
}
