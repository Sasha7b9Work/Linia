// 2025/6/1 17:20:45 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel03_Config/Panel03_Config.h"
#include "MainWindow.h"
#include "Utils/GlobalFunctions.h"
#include "Utils/SystemDepend.h"
#include "Controls/SpinBox.h"
#include "Panels/Panel03_Config/PanelScheme/WindowLibraryTests.h"
#include "Panels/Panel03_Config/PanelScheme/PanelScheme.h"
#include "Panels/Panel03_Config/PanelChannelBS/PanelChannelBS.h"
#include "Panels/Panel03_Config/PanelChannelC/PanelChannelC.h"
#include "Panels/Panel03_Config/PanelCalculate.h"


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

        int id = btnChannelB->GetId();

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

    if (btnChannelB && id == btnChannelB->GetId())
    {
        return new PanelChannelB(self, x, w);
    }
    else if (btnChannelS && id == btnChannelS->GetId())
    {
        return new PanelChannelS(self, x, w);
    }
    else if (btnChannelC && id == btnChannelC->GetId())
    {
        return new PanelChannelC(self, x, w);
    }
    else if (btnScheme && id == btnScheme->GetId())
    {
        return new PanelScheme(self, x);
    }
    else if (btnCalculate && id == btnCalculate->GetId())
    {
        return new PanelCalculate(self, x, w);
    }

    return nullptr;
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
