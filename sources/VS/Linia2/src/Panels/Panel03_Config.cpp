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

        int id = ID_PAN3_BTN_CHANNEL_B;

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
        CreatePanelChannelC(panel, x, w);
    }
    else if (id == ID_PAN3_BTN_CHANNEL_B)
    {
        CreatePanelChannelB(panel, x, w);
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


void PanelConfig::CreatePanelChannelC(wxPanel *panel, int x, int w)
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

        new wxCheckBox(boxScan, ID_PAN3_CHAN_C_CHECK_DUTY_CYCLE, _L("Скважн. x 2"), { 100, SD::Y_SB(120) }, { 100, 20 });
    }

    wxStaticBox *boxMeter = new wxStaticBox(panel, wxID_ANY, _L("Измеритель"), { x, boxScan->GetSize().y + x }, { w, 50 });

    {
        int y = 20;
        int dY = 3;

        new wxStaticText(boxMeter, wxID_ANY, "Uc", { 10, SD::Y_SB(y + dY) });
        new wxStaticText(boxMeter, wxID_ANY, "Ic", { 100, SD::Y_SB(y + dY) });

        wxArrayString choices;
        choices.Add("100V");

        new wxComboBox(boxMeter, ID_PAN3_CHAN_C_COMBO_Uc, choices[0], { 30, SD::Y_SB(y) }, { 60, TEXTCNTRL_HEIGHT }, choices, wxCB_READONLY);

        choices.Clear();
        choices.Add("10A");

        new wxComboBox(boxMeter, ID_PAN3_CHAN_C_COMBO_Ic, choices[0], { 120, SD::Y_SB(y) }, { 60, TEXTCNTRL_HEIGHT }, choices, wxCB_READONLY);
    }

    wxStaticBox *boxSource = new wxStaticBox(panel, wxID_ANY, _L("Источник U"), { x, boxMeter->GetPosition().y + boxMeter->GetSize().y + x }, { w, 200 });

    {
        new wxStaticText(boxSource, wxID_ANY, _L("Диапазон Uc"), { 10, SD::Y_SB(30) });

        wxArrayString choices;
        choices.Add("20V");

        new wxComboBox(boxSource, ID_PAN3_CHAN_C_COMBO_SOURCE, choices[0], { 100, SD::Y_SB(27) }, { 60, TEXTCNTRL_HEIGHT }, choices, wxCB_READONLY);

        new wxStaticText(boxSource, wxID_ANY, _L("Ограничение Uc, %%"), { 40, SD::Y_SB(55) });

        int y = 80;
        int dy = 3;

        new wxStaticText(boxSource, wxID_ANY, _L("Старт"), { 10, SD::Y_SB(y + dy) });
        new wxStaticText(boxSource, wxID_ANY, _L("Стоп"), { 10, SD::Y_SB(y + 30 + dy) });

        new wxSpinCtrl(boxSource, ID_PAN3_CHAN_C_SPIN_START, "0", { 80, SD::Y_SB(y) }, { 100, TEXTCNTRL_HEIGHT });
        new wxSpinCtrl(boxSource, ID_PAN3_CHAN_C_SPIN_STOP, "100", { 80, SD::Y_SB(y + 30) }, { 100, TEXTCNTRL_HEIGHT });
    }
}


void PanelConfig::CreatePanelChannelB(wxPanel *panel, int x, int w)
{
    wxStaticBox *boxGenerator = new wxStaticBox(panel, wxID_ANY, _L("Генератор ступенек"), { x, 0 }, { w, 300 });

    {
        int y = 25;
        new wxStaticText(boxGenerator, wxID_ANY, _L("Тип"), { 10, SD::Y_SB(y) });

        wxArrayString choices;
        choices.Add("U");
        choices.Add("I");

        new wxComboBox(boxGenerator, ID_PAN3_CHAN_B_GEN_COMBO_TYPE, choices[0], { 70, SD::Y_SB(y - 3) }, { 60, TEXTCNTRL_HEIGHT }, choices, wxCB_READONLY);

        y += 25;

        new wxStaticText(boxGenerator, wxID_ANY, _L("Импульс"), { 10, SD::Y_SB(y) });

        choices.Clear();
        choices.Add(_L("Вкл"));
        choices.Add(_L("Выкл"));

        new wxComboBox(boxGenerator, ID_PAN3_CHAN_B_GEN_COMBO_IMPULSE, choices[0], { 70, SD::Y_SB(y - 3) }, { 60, TEXTCNTRL_HEIGHT }, choices, wxCB_READONLY);

        y += 25;

        choices.Clear();
        choices.Add("2V");

        new wxStaticText(boxGenerator, wxID_ANY, _L("Амплитуда ступени"), { 10, SD::Y_SB(y) });

        new wxComboBox(boxGenerator, ID_PAN3_CHAN_B_GEN_COMBO_AMPLITUDE, choices[0], { 130, SD::Y_SB(y - 3) }, { 60, TEXTCNTRL_HEIGHT }, choices, wxCB_READONLY);

        y += 25;

        new wxCheckBox(boxGenerator, ID_PAN3_CHAN_B_GEN_CHECK_SMALL, "x 0.1", { 10, SD::Y_SB(y) }, { 60, TEXTCNTRL_HEIGHT });

        y += 25;

        new wxStaticText(boxGenerator, wxID_ANY, _L("Число ступенек"), { 10, SD::Y_SB(y + 3) });

        new wxSpinCtrl(boxGenerator, ID_PAN3_CHAN_B_GEN_SPIN_NUMBER_STEPS, "5", { 120, SD::Y_SB(y) }, { 50, TEXTCNTRL_HEIGHT });

        y += 25;

        choices.Clear();
        choices.Add(_L("прямая"));
        choices.Add(_L("обратная"));

        new wxStaticText(boxGenerator, wxID_ANY, _L("Полярность"), { 10, SD::Y_SB(y + 3) });

        new wxComboBox(boxGenerator, ID_PAN3_CHAN_B_GEN_COMBO_POLARITY, choices[0], { 100, SD::Y_SB(y) }, { 80, TEXTCNTRL_HEIGHT }, choices, wxCB_READONLY);

        y += 25;

        wxStaticBox *boxOffset = new wxStaticBox(boxGenerator, wxID_ANY, _L("Смещение"), { x, y }, { w - 10, 100 });

        {
            y = 20;

            new wxSlider(boxOffset, ID_PAN3_CHAN_B_GEN_OFFSET_SLIDER, 5, 0, 20, { 10, SD::Y_SB(y) }, { 120, TEXTCNTRL_HEIGHT });

            y += 25;

            choices.Clear();
            choices.Add(_L("прямая"));
            choices.Add(_L("обратная"));

            new wxStaticText(boxOffset, wxID_ANY, _L("Полярность"), { 10, SD::Y_SB(y + 3) });

            new wxComboBox(boxOffset, ID_PAN3_CHAN_B_GEN_OFFSET_COMBO_POLARITY, choices[0], { 100, SD::Y_SB(y) }, { 80, TEXTCNTRL_HEIGHT }, choices, wxCB_READONLY);
        }
    }

    wxStaticBox *boxLimitation = new wxStaticBox(panel, wxID_ANY, _L("Ограничение"), { x, boxGenerator->GetSize().y + x }, { w, 100 });

    {
        int y = 20;

        new wxStaticText(boxLimitation, wxID_ANY, _L("Диапазон"), { 10, SD::Y_SB(y) });

        wxArrayString choices;
        choices.Add("10 mA");

        new wxComboBox(boxLimitation, ID_PAN3_CHAN_B_LIM_COMBO_RANGE, choices[0], { 100, SD::Y_SB(y - 3) }, { 80, TEXTCNTRL_HEIGHT }, choices, wxCB_READONLY);

        y += 25;

        new wxSlider(boxLimitation, ID_PAN3_CHAN_B_LIM_SLIDER, 5, 0, 20, { 10, SD::Y_SB(y) }, { 120, TEXTCNTRL_HEIGHT });
    }
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
