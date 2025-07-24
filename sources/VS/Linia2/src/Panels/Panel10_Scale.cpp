// 2025/6/1 18:18:16 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel03_Config.h"
#include "Panels/Panel07_Indicator.h"
#include "Panels/Panel10_Scale.h"
#include "Panels/Panel11_Menu/Panel11_Menu.h"
#include "MainWindow.h"
#include "Utils/GlobalFunctions.h"
#include "Utils/SystemDepend.h"


PanelScale *PanelScale::self = nullptr;


PanelScale::PanelScale(wxWindow* parent) :
    Panel(parent, MainWindow::WIDTH1 + MainWindow::WIDTH2, PanelConfig::HEIGHT + PanelIndicator::HEIGHT, WIDTH, HEIGTH)
{
    self = this;

    Bind(wxEVT_TOGGLEBUTTON, &PanelScale::OnEventButton, this);

    wxToggleButton *button = new wxToggleButton(this, ID_PAN10_BTN_SCALE, _L("Шкала"), { 0, 0 }, { 60, 20 });
    str_panels.push_back({ button, CreatePanel(button)});

    button = new wxToggleButton(this, ID_PAN10_BTN_ERRORS, _L("Ошибки"), { 60, 0 }, { 60, 20 });
    str_panels.push_back({ button, CreatePanel(button)});

    {
        // Включаем панель

        int id = ID_PAN10_BTN_SCALE;

        wxCommandEvent evt(wxEVT_TOGGLEBUTTON, id);
        evt.SetInt(1);
        this->ProcessWindowEvent(evt);
    }
}


void PanelScale::OnEventButton(wxCommandEvent &event)
{
    int id = event.GetId();

    if(id == ID_PAN10_BTN_SCALE || id == ID_PAN10_BTN_ERRORS)
    {
        if (!event.IsChecked())
        {
            GF::FindToggleButton(self, event.GetId())->SetValue(true);
        }
        else
        {
            UncheckAllAcross(event.GetId());

            EnablePanel(event.GetId());
        }
    }
    else
    {
        event.Skip();
    }
}


void PanelScale::UncheckAllAcross(int id)
{
    for (auto &str : str_panels)
    {
        str.button->SetValue(str.button->GetId() == id);
    }
}


void PanelScale::EnablePanel(int button_id)
{
    for (auto &str : str_panels)
    {
        str.panel->Show(str.button->GetId() == button_id);
    }
}


wxPanel *PanelScale::CreatePanel(wxToggleButton *button)
{
    wxPanel *panel = new wxPanel(self);

    panel->SetSize(WIDTH, HEIGTH);
    panel->SetPosition({ 0, 40 });

    int x = 4;
    int w = WIDTH;

    int id = button->GetId();

    if (id == ID_PAN10_BTN_SCALE)
    {
        CreatePanelScale(panel, x, w);
    }
    else if (id == ID_PAN10_BTN_ERRORS)
    {
        CreatePanelErrors(panel, x, w);
    }

    return panel;
}


void PanelScale::CreatePanelScale(wxPanel *panel, int x, int /*w*/)
{
    wxSize size{ 30, 15 };

    x += 5;

    int y = 20;
    int dy = 30;

    new wxToggleButton(panel, ID_PAN10_BTN_1, "", { x, SD::Y_SB(y) }, size);

    new wxToggleButton(panel, ID_PAN10_BTN_2, "", { x, SD::Y_SB(y + dy) }, size);

    new wxToggleButton(panel, ID_PAN10_BTN_3, "", { x, SD::Y_SB(y + dy * 2) }, size);

    int dx = 65;

    x += dx;

    new wxCheckBox(panel, ID_PAN10_CHBOX_1, "", { x, SD::Y_SB(y) });

    new wxCheckBox(panel, ID_PAN10_CHBOX_2, "", { x, SD::Y_SB(y + dy) });

    new wxCheckBox(panel, ID_PAN10_CHBOX_3, "", { x, SD::Y_SB(y + dy * 2) });

    x += dx;

    new wxCheckBox(panel, ID_PAN10_CHBOX_4, "", { x, SD::Y_SB(y) });

    new wxCheckBox(panel, ID_PAN10_CHBOX_5, "", { x, SD::Y_SB(y + dy) });
}


void PanelScale::CreatePanelErrors(wxPanel *, int /*x*/, int /*w*/)
{

}
