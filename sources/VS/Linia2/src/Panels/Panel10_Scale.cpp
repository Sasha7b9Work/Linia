// 2025/6/1 18:18:16 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel03_Config.h"
#include "Panels/Panel07_Indicator.h"
#include "Panels/Panel10_Scale.h"
#include "Panels/Panel11_Menu/Panel11_Menu.h"
#include "MainWindow.h"
#include "Utils/GlobalFunctions.h"


PanelScale *PanelScale::self = nullptr;


PanelScale::PanelScale(wxWindow* parent) :
    Panel(parent, MainWindow::WIDTH1 + MainWindow::WIDTH2, PanelConfig::HEIGHT + PanelIndicator::HEIGHT,
        MainWindow::WIDTH3, MainWindow::HEIGHT - PanelConfig::HEIGHT - PanelIndicator::HEIGHT)
{
    self = this;

    Bind(wxEVT_TOGGLEBUTTON, &PanelScale::OnEventButton, this);

    wxToggleButton *button = new wxToggleButton(this, ID_PAN10_BTN_SCALE, _L("Øêàëà"), { 0, 0 }, { 60, 20 });
    str_panels.push_back({ button, nullptr });

    button = new wxToggleButton(this, ID_PAN10_BTN_ERRORS, _L("Îøèáêè"), { 60, 0 }, { 60, 20 });
    str_panels.push_back({ button, nullptr });

    {
        // Âêëþ÷àåì ïàíåëü

        int id = ID_PAN10_BTN_SCALE;

        wxCommandEvent evt(wxEVT_TOGGLEBUTTON, id);
        evt.SetInt(1);
        this->ProcessWindowEvent(evt);
    }
}


void PanelScale::OnEventButton(wxCommandEvent &event)
{
    if (!event.IsChecked())
    {
        GF::FindToggleButton(self, event.GetId())->SetValue(true);
    }
    else
    {
        UnñheckAllAcross(event.GetId());

        EnablePanel(event.GetId());
    }
}


void PanelScale::UnñheckAllAcross(int id)
{
    for (auto &str : str_panels)
    {
        str.button->SetValue(str.button->GetId() == id);
    }
}


void PanelScale::EnablePanel(int button_id)
{

}
