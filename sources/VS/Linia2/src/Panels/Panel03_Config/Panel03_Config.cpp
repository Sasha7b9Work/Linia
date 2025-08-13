// 2025/6/1 17:20:45 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel03_Config/Panel03_Config.h"
#include "MainWindow.h"
#include "Utils/GlobalFunctions.h"
#include "Utils/SystemDepend.h"
#include "Controls/SpinBox.h"
#include "Panels/Panel03_Config/PanelScheme/WindowLibraryTests.h"
#include "Panels/Panel03_Config/PanelScheme/PanelScheme.h"
#include "Panels/Panel03_Config/PanelChannelC/PanelChannelC.h"


PanelConfig *PanelConfig::self = nullptr;


PanelConfig::PanelConfig(wxWindow* parent) :
    Panel(parent, MainWindow::WIDTH1 + MainWindow::WIDTH2, 0, MainWindow::WIDTH3, HEIGHT)
{
    self = this;

    Bind(wxEVT_TOGGLEBUTTON, &PanelConfig::OnEventToggleButton, this);

    const int h = 20;

    wxSize sizeChan = { 67, h };

    btnChannelC = new wxToggleButton(this, wxID_ANY, _L("Канал C"), { 0, 0 }, sizeChan);
    str_panels.push_back({ btnChannelC, CreatePanel(btnChannelC) });

    btnChannelB = new wxToggleButton(this, wxID_ANY, _L("Канал B"), { sizeChan.x, 0 }, sizeChan);
    str_panels.push_back({ btnChannelB, CreatePanel(btnChannelB) });

    btnChannelS = new wxToggleButton(this, wxID_ANY, _L("Канал S"), { sizeChan.x * 2, 0 }, sizeChan);
    str_panels.push_back({ btnChannelS, CreatePanel(btnChannelS) });

    wxSize sizeScheme = { 120, h };
    btnScheme = new wxToggleButton(this, wxID_ANY, _L("Схема включения"), { 0, h }, sizeScheme);
    str_panels.push_back({ btnScheme, CreatePanel(btnScheme) });

    btnCalculate = new wxToggleButton(this, wxID_ANY, _L("Расчёт"), { sizeScheme.x, h }, { MainWindow::WIDTH3 - sizeScheme.x - 3, h });
    str_panels.push_back({ btnCalculate, CreatePanel(btnCalculate) });

    {
        // Включаем панель

        int id = btnChannelC->GetId();

        wxCommandEvent evt(wxEVT_TOGGLEBUTTON, id);
        evt.SetInt(1);
        this->ProcessWindowEvent(evt);
    }
}

wxPanel *PanelConfig::CreatePanel(wxToggleButton *button)
{
    int x = 4;
    int w = MainWindow::WIDTH3 - 2 * x - 1;

    int id = button->GetId();

    if (btnChannelC && id == btnChannelC->GetId())
    {
        return new PanelChannelC(self, x, w);
    }
    else if (btnScheme && id == btnScheme->GetId())
    {
        return new PanelScheme(self, x);
    }

    wxPanel *panel = new wxPanel(self);

    panel->SetSize({ MainWindow::WIDTH3, HEIGHT - 40 });
    panel->SetPosition({ 0, 40 });

    if (id == btnChannelB->GetId())
    {
        CreatePanelChannelB(panel, x, w);
    }
    else if (id == btnChannelS->GetId())
    {
        CreatePanelChannelS(panel, x, w);
    }
    else if (id == btnCalculate->GetId())
    {
        CreatePanelCalculate(panel, x, w);
    }

    return panel;
}


void PanelConfig::CreatePanelChannelB(wxPanel *panel, int x, int w)
{
    wxStaticBox *boxGenerator = new wxStaticBox(panel, wxID_ANY, _L("Генератор ступенек"), { x, 0 }, { w, 300 });

    {
        int y = 25;

        wxArrayString choices;
        choices.Add("U");
        choices.Add("I");

        new ButtonsCombo(boxGenerator, "Тип", { 70, SD::Y_SB(y - 3) }, 100, choices, 0, 1);

        y += 25;

        choices.Clear();
        choices.Add(_L("Вкл"));
        choices.Add(_L("Выкл"));

        new ButtonsCombo(boxGenerator, "Импульс", {70, SD::Y_SB(y - 3)}, 100, choices, 0, 1);

        y += 25;

        choices.Clear();
        choices.Add("2V");

        new wxStaticText(boxGenerator, wxID_ANY, _L("Амплитуда ступени"), { 10, SD::Y_SB(y) });

        new wxComboBox(boxGenerator, wxID_ANY, choices[0], { 130, SD::Y_SB(y - 3) }, { 60, TEXTCNTRL_HEIGHT }, choices, wxCB_READONLY);

        y += 25;

        new wxCheckBox(boxGenerator, wxID_ANY, "x 0.1", { 10, SD::Y_SB(y) }, { 60, TEXTCNTRL_HEIGHT });

        y += 25;

        new wxStaticText(boxGenerator, wxID_ANY, _L("Число ступенек"), { 10, SD::Y_SB(y + 3) });

        new SpinBox(boxGenerator, wxID_ANY, "5", { 120, SD::Y_SB(y) }, { 50, TEXTCNTRL_HEIGHT });

        y += 25;

        choices.Clear();
        choices.Add(_L("прямая"));
        choices.Add(_L("обратная"));

        new wxStaticText(boxGenerator, wxID_ANY, _L("Полярность"), { 10, SD::Y_SB(y + 3) });

        new wxComboBox(boxGenerator, wxID_ANY, choices[0], { 100, SD::Y_SB(y) }, { 80, TEXTCNTRL_HEIGHT }, choices, wxCB_READONLY);

        y += 25;

        wxStaticBox *boxOffset = new wxStaticBox(boxGenerator, wxID_ANY, _L("Смещение"), { x, y }, { w - 10, 100 });

        {
            y = 20;

            new wxSlider(boxOffset, wxID_ANY, 5, 0, 20, { 10, SD::Y_SB(y) }, { 120, TEXTCNTRL_HEIGHT });

            y += 25;

            choices.Clear();
            choices.Add(_L("прямая"));
            choices.Add(_L("обратная"));

            new wxStaticText(boxOffset, wxID_ANY, _L("Полярность"), { 10, SD::Y_SB(y + 3) });

            new wxComboBox(boxOffset, wxID_ANY, choices[0], { 100, SD::Y_SB(y) }, { 80, TEXTCNTRL_HEIGHT }, choices, wxCB_READONLY);
        }
    }

    wxStaticBox *boxLimitation = new wxStaticBox(panel, wxID_ANY, _L("Ограничение"), { x, boxGenerator->GetSize().y + x }, { w, 100 });

    {
        int y = 20;

        new wxStaticText(boxLimitation, wxID_ANY, _L("Диапазон"), { 10, SD::Y_SB(y) });

        wxArrayString choices;
        choices.Add("10 mA");

        new wxComboBox(boxLimitation, wxID_ANY, choices[0], { 100, SD::Y_SB(y - 3) }, { 80, TEXTCNTRL_HEIGHT }, choices, wxCB_READONLY);

        y += 25;

        new wxSlider(boxLimitation, wxID_ANY, 5, 0, 20, { 10, SD::Y_SB(y) }, { 120, TEXTCNTRL_HEIGHT });
    }
}


void PanelConfig::CreatePanelChannelS(wxPanel *panel, int x, int w)
{
    wxStaticBox *boxGenerator = new wxStaticBox(panel, wxID_ANY, _L("Генератор ступенек"), { x, 0 }, { w, 300 });

    {
        int y = 25;
        new wxStaticText(boxGenerator, wxID_ANY, _L("Тип"), { 10, SD::Y_SB(y) });

        wxArrayString choices;
        choices.Add("U");
        choices.Add("I");

        new wxComboBox(boxGenerator, wxID_ANY, choices[0], { 70, SD::Y_SB(y - 3) }, { 60, TEXTCNTRL_HEIGHT }, choices, wxCB_READONLY);

        y += 25;

        new wxStaticText(boxGenerator, wxID_ANY, _L("Импульс"), { 10, SD::Y_SB(y) });

        choices.Clear();
        choices.Add(_L("Вкл"));
        choices.Add(_L("Выкл"));

        new wxComboBox(boxGenerator, wxID_ANY, choices[0], { 70, SD::Y_SB(y - 3) }, { 60, TEXTCNTRL_HEIGHT }, choices, wxCB_READONLY);

        y += 25;

        choices.Clear();
        choices.Add("2V");

        new wxStaticText(boxGenerator, wxID_ANY, _L("Амплитуда ступени"), { 10, SD::Y_SB(y) });

        new wxComboBox(boxGenerator, wxID_ANY, choices[0], { 130, SD::Y_SB(y - 3) }, { 60, TEXTCNTRL_HEIGHT }, choices, wxCB_READONLY);

        y += 25;

        y += 25;

        new wxStaticText(boxGenerator, wxID_ANY, _L("Число ступенек"), { 10, SD::Y_SB(y + 3) });

        new SpinBox(boxGenerator, wxID_ANY, "5", { 120, SD::Y_SB(y) }, { 50, TEXTCNTRL_HEIGHT });

        y += 25;

        choices.Clear();
        choices.Add(_L("прямая"));
        choices.Add(_L("обратная"));

        new wxStaticText(boxGenerator, wxID_ANY, _L("Полярность"), { 10, SD::Y_SB(y + 3) });

        new wxComboBox(boxGenerator, wxID_ANY, choices[0], { 100, SD::Y_SB(y) }, { 80, TEXTCNTRL_HEIGHT }, choices, wxCB_READONLY);

        y += 25;

        wxStaticBox *boxOffset = new wxStaticBox(boxGenerator, wxID_ANY, _L("Смещение"), { x, y }, { w - 10, 100 });

        {
            y = 20;

            new wxSlider(boxOffset, wxID_ANY, 5, 0, 20, { 10, SD::Y_SB(y) }, { 120, TEXTCNTRL_HEIGHT });

            y += 25;

            choices.Clear();
            choices.Add(_L("прямая"));
            choices.Add(_L("обратная"));

            new wxStaticText(boxOffset, wxID_ANY, _L("Полярность"), { 10, SD::Y_SB(y + 3) });

            new wxComboBox(boxOffset, wxID_ANY, choices[0], { 100, SD::Y_SB(y) }, { 80, TEXTCNTRL_HEIGHT }, choices, wxCB_READONLY);
        }
    }

    wxStaticBox *boxLimitation = new wxStaticBox(panel, wxID_ANY, _L("Ограничение"), { x, boxGenerator->GetSize().y + x }, { w, 100 });

    {
        int y = 20;

        new wxStaticText(boxLimitation, wxID_ANY, _L("Диапазон"), { 10, SD::Y_SB(y) });

        wxArrayString choices;
        choices.Add("10 mA");

        new wxComboBox(boxLimitation, wxID_ANY, choices[0], { 100, SD::Y_SB(y - 3) }, { 80, TEXTCNTRL_HEIGHT }, choices, wxCB_READONLY);

        y += 25;

        new wxSlider(boxLimitation, wxID_ANY, 5, 0, 20, { 10, SD::Y_SB(y) }, { 120, TEXTCNTRL_HEIGHT });
    }
}


void PanelConfig::CreatePanelCalculate(wxPanel *panel, int x, int w)
{
    wxStaticBox *boxGraph = new wxStaticBox(panel, wxID_ANY, _L("График"), { x, 0 }, { w, 75 });

    int y = 20;
    const int dy = 3;

    {
        new wxStaticText(boxGraph, wxID_ANY, _L("Ось X"), { 10, SD::Y_SB(y + dy) });

        new wxTextCtrl(boxGraph, wxID_ANY, "Ud", { 100, SD::Y_SB(y) }, { 50, TEXTCNTRL_HEIGHT });

        y += 25;

        new wxStaticText(boxGraph, wxID_ANY, _L("Ось Y"), { 10, SD::Y_SB(y + dy) });

        new wxTextCtrl(boxGraph, wxID_ANY, "Id", { 100, SD::Y_SB(y) }, { 50, TEXTCNTRL_HEIGHT });
    }

    wxStaticBox *boxParameter = new wxStaticBox(panel, wxID_ANY, _L("Параметр"), { x, boxGraph->GetSize().y + x }, { w, 200 });

    {
        y = 20;

        new wxStaticText(boxParameter, wxID_ANY, _L("Найти"), { 10, SD::Y_SB(y + dy) });
        new wxStaticText(boxParameter, wxID_ANY, _L("при"), { 100, SD::Y_SB(y + dy) });

        new wxTextCtrl(boxParameter, wxID_ANY, "Id", { 50, SD::Y_SB(y) }, { 40, TEXTCNTRL_HEIGHT });
        new wxTextCtrl(boxParameter, wxID_ANY, "Ud", { 140, SD::Y_SB(y) }, { 40, TEXTCNTRL_HEIGHT });

        y += 30;

        new wxStaticText(boxParameter, wxID_ANY, _L("равном"), { 10, SD::Y_SB(y) });

        y += 20;

        new wxStaticText(boxParameter, wxID_ANY, _L("значению") + " 1", { 10, SD::Y_SB(y + dy) });
        new wxTextCtrl(boxParameter, wxID_ANY, "6", { 100, SD::Y_SB(y) }, { 80, TEXTCNTRL_HEIGHT });

        y += 25;

        new wxStaticText(boxParameter, wxID_ANY, _L("значению") + " 2", { 10, SD::Y_SB(y + dy) });
        new wxTextCtrl(boxParameter, wxID_ANY, "6", { 100, SD::Y_SB(y) }, { 80, TEXTCNTRL_HEIGHT });

        y += 25;

        new wxStaticText(boxParameter, wxID_ANY, _L("на ступеньке") + " № 5", { 10, SD::Y_SB(y + dy) });

        wxStaticBox *boxCalculate = new wxStaticBox(boxParameter, wxID_ANY, _L("Рассчитать"), { x, y }, { w - 10, 60 });

        {
            y = 25;

            new wxRadioButton(boxCalculate, wxID_ANY, "dY/dX", { 10, SD::Y_SB(y) });
            new wxRadioButton(boxCalculate, wxID_ANY, "dX/dY", { 100, SD::Y_SB(y) });
        }
    }

    wxStaticBox *boxBorder = new wxStaticBox(panel, wxID_ANY, _L("Пороговые значения"), { x, boxParameter->GetPosition().y + boxParameter->GetSize().y + x }, { w, 80 });

    {
        new wxStaticText(boxBorder, wxID_ANY, "MIN", { 10, SD::Y_SB(y + dy) });
        new wxTextCtrl(boxBorder, wxID_ANY, "3", { 100, SD::Y_SB(y) }, { 80, TEXTCNTRL_HEIGHT });

        y += 25;

        new wxStaticText(boxBorder, wxID_ANY, "MAX", { 10, SD::Y_SB(y + dy) });
        new wxTextCtrl(boxBorder, wxID_ANY, "4", { 100, SD::Y_SB(y) }, { 80, TEXTCNTRL_HEIGHT });
    }
}


void PanelConfig::OnEventToggleButton(wxCommandEvent &event)
{
    if (!event.IsChecked())
    {
        ((wxToggleButton *)event.GetEventObject())->SetValue(true);
    }
    else
    {
        UnсheckAllAcross(event.GetId());

        EnablePanel(event.GetId());
    }
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
