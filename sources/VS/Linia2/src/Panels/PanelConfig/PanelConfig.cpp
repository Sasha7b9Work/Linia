// 2025/6/1 17:20:45 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PanelConfig/PanelConfig.h"
#include "MainWindow.h"
#include "Utils/GlobalFunctions.h"
#include "Utils/SystemDepend.h"
#include "Controls/SpinBox.h"
#include "Panels/PanelConfig/PanelScheme/WindowLibraryTests.h"
#include "Panels/PanelConfig/PanelScheme/PanelScheme.h"
#include "Panels/PanelConfig/PanelChannelBS.h"
#include "Panels/PanelConfig/PanelChannelC.h"
#include "Panels/PanelConfig/PanelCalculate.h"
#include "Utils/Configurator.h"


PanelConfig *PanelConfig::self = nullptr;


PanelConfig::PanelConfig(wxWindow* parent) :
    Panel(parent, MainWindow::WIDTH1 + MainWindow::WIDTH2, 0, MainWindow::WIDTH3, HEIGHT)
{
    self = this;

    Bind(wxEVT_TOGGLEBUTTON, &PanelConfig::OnEventToggleButton, this);

    const int h = 20;

    wxSize sizeChan = { 67, h };

    btnChannelC = new wxToggleButton(this, wxID_ANY, "Канал C", { 0, 0 }, sizeChan);
    str_panels.emplace_back(StructPanel{ btnChannelC, CreatePanel(btnChannelC) });

    btnChannelB = new wxToggleButton(this, wxID_ANY, "Канал B", { sizeChan.x, 0 }, sizeChan);
    str_panels.emplace_back(StructPanel{ btnChannelB, CreatePanel(btnChannelB) });

    btnChannelS = new wxToggleButton(this, wxID_ANY, "Канал S", { sizeChan.x * 2, 0 }, sizeChan);
    str_panels.emplace_back(StructPanel{ btnChannelS, CreatePanel(btnChannelS) });

    wxSize sizeScheme = { 120, h };
    btnScheme = new wxToggleButton(this, wxID_ANY, "Схема включения", { 0, h }, sizeScheme);
    str_panels.emplace_back(StructPanel{ btnScheme, CreatePanel(btnScheme) });

    btnCalculate = new wxToggleButton(this, wxID_ANY, "Расчёт", { sizeScheme.x, h }, { MainWindow::WIDTH3 - sizeScheme.x - 3, h });
    str_panels.emplace_back(StructPanel{ btnCalculate, CreatePanel(btnCalculate) });

    {
        // Включаем панель

        GF::SendCommandEvent(btnChannelC, wxEVT_TOGGLEBUTTON, 1);
    }
}

wxPanel *PanelConfig::CreatePanel(wxToggleButton *button)
{
    int x = 4;
    int w = MainWindow::WIDTH3 - 2 * x - 1;
    int h = HEIGHT - 47;

    if (button == btnChannelB)
    {
        return new PanelChannelB(self, x, w, h);
    }
    else if (button == btnChannelS)
    {
        return new PanelChannelS(self, x, w, h);
    }
    else if (button == btnChannelC)
    {
        return new PanelChannelC(self, x, w, h);
    }
    else if (button == btnScheme)
    {
        return new PanelScheme(self, x, w, h);
    }
    else if (button == btnCalculate)
    {
        return new PanelCalculate(self, x, w, h);
    }

    return nullptr;
}


void PanelConfig::OnEventToggleButton(wxCommandEvent &event)
{
    int id = event.GetId();

    if (id == btnChannelC->GetId() ||
        id == btnChannelB->GetId() ||
        id == btnChannelS->GetId() ||
        id == btnScheme->GetId() ||
        id == btnCalculate->GetId())
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

    event.Skip();
}


void PanelConfig::SetCurrentPanel(const wxString &name)
{
    for (auto &str : str_panels)
    {
        if (str.panel->GetName() == name)
        {
            UnсheckAllAcross(str.button->GetId());
            EnablePanel(str.button->GetId());
        }
    }
}


void PanelConfig::UnсheckAllAcross(int id)
{
    for (auto &str : str_panels)
    {
        bool value = str.button->GetId() == id;
        str.button->SetValue(value);

        wxFont font = str.button->GetFont();
        font.SetWeight(value ? wxFONTWEIGHT_BOLD : wxFONTWEIGHT_NORMAL);
        str.button->SetFont(font);
    }
}


void PanelConfig::EnablePanel(int button_id)
{
    for (auto &str : str_panels)
    {
        str.panel->Show(str.button->GetId() == button_id);
    }
}


wxPanel *PanelConfig::GetCurrentPanel()
{
    for (auto &str : str_panels)
    {
        if (str.panel->IsShown())
        {
            return str.panel;
        }
    }

    return str_panels[0].panel;
}


void PanelConfig::Pack()
{
    PanelChannelC::self->Pack();
    PanelChannelB::self->Pack();
    PanelChannelS::self->Pack();
    PanelScheme::self->Pack();

    Config::WriteString("CurrentPanel", GetCurrentPanel()->GetName());
}


void PanelConfig::Unpack()
{
    PanelChannelC::self->Unpack();
    PanelChannelB::self->Unpack();
    PanelChannelS::self->Unpack();
    PanelScheme::self->Unpack();

    SetCurrentPanel(Config::ReadString("CurrentPanel"));
}


void PanelConfig::EnableSubPanels(bool enable)
{
    for (auto &pan : str_panels)
    {
        pan.panel->Enable(enable);
    }
}
