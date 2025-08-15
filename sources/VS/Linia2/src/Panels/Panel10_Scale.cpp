// 2025/6/1 18:18:16 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel03_Config/Panel03_Config.h"
#include "Panels/Panel07_Indicator.h"
#include "Panels/Panel10_Scale.h"
#include "Panels/Panel11_Menu/Panel11_Menu.h"
#include "Utils/GlobalFunctions.h"
#include "Utils/SystemDepend.h"
#include "Controls/Buttons.h"


PanelScale *PanelScale::self = nullptr;


PanelScale::PanelScale(wxWindow* parent) :
    Panel(parent, MainWindow::WIDTH1 + MainWindow::WIDTH2, PanelConfig::HEIGHT + PanelIndicator::HEIGHT, WIDTH, HEIGTH)
{
    self = this;

    Bind(wxEVT_TOGGLEBUTTON, &PanelScale::OnEventButton, this);

    btnScale = new wxToggleButton(this, wxID_ANY, _L("Шкала"), { 0, 0 }, { 60, 20 });
    str_panels.push_back({ btnScale, CreatePanel(btnScale)});

    btnErrors = new wxToggleButton(this, wxID_ANY, _L("Ошибки"), { 60, 0 }, { 60, 20 });
    str_panels.push_back({ btnErrors, CreatePanel(btnErrors)});

    {
        // Включаем панель

        GF::SendCommandEvent(btnScale, wxEVT_TOGGLEBUTTON, 1);
    }
}


void PanelScale::OnEventButton(wxCommandEvent &event)
{
    int id = event.GetId();

    if(id == btnScale->GetId() || id == btnErrors->GetId())
    {
        if (!event.IsChecked())
        {
            ((wxToggleButton *)event.GetEventObject())->SetValue(true);
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

    if (id == btnScale->GetId())
    {
        CreatePanelScale(panel, x, w);
    }
    else if (id == btnErrors->GetId())
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

    wxArrayString choices =
    {
        "grid/grid.bmp",
        "grid/bmp00076.bmp",
        "grid/bmp00075.bmp"
    };

    new ButtonBitmapChoice(panel, { 10, SD::Y_SB(25) }, wxDefaultSize, choices);

    int dx = 65;

    x += dx;

    chb1 = new wxCheckBox(panel, wxID_ANY, "", { x, SD::Y_SB(y) });

    chb2 = new wxCheckBox(panel, wxID_ANY, "", { x, SD::Y_SB(y + dy) });

    chb3 = new wxCheckBox(panel, wxID_ANY, "", { x, SD::Y_SB(y + dy * 2) });

    x += dx;

    chb4 = new wxCheckBox(panel, wxID_ANY, "", { x, SD::Y_SB(y) });

    chb5 = new wxCheckBox(panel, wxID_ANY, "", { x, SD::Y_SB(y + dy) });
}


void PanelScale::CreatePanelErrors(wxPanel *, int /*x*/, int /*w*/)
{

}
