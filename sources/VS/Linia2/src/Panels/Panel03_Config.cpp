// 2025/6/1 17:20:45 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel03_Config.h"
#include "MainWindow.h"
#include "Utils/GlobalFunctions.h"


PanelConfig *PanelConfig::self = nullptr;


PanelConfig::PanelConfig(wxWindow* parent) :
    Panel(parent, MainWindow::WIDTH1 + MainWindow::WIDTH2, 0, MainWindow::WIDTH3, HEIGHT)
{
    self = this;

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

    Bind(wxEVT_TOGGLEBUTTON, &PanelConfig::OnEventButton, this);

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
        wxStaticBox *boxScan = new wxStaticBox(panel, wxID_ANY, _L("Развёртка"), { x, 0 }, { w, 100 });

        wxStaticBox *boxMeter = new wxStaticBox(panel, wxID_ANY, _L("Измеритель"), { x, boxScan->GetSize().y + x }, { w, 50 } );

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
