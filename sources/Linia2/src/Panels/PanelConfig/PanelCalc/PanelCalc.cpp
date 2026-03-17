// 2025/6/1 17:41:03 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PanelConfig/PanelCalc/PanelCalc.h"
#include "Utils/GlobalFunctions.h"
#include "IPPP/Tests/Model.h"
#include "Panels/PanelConfig/PanelConfig.h"
#include "Panels/WindowCalculation/WindowCalculation.h"
#include "Utils/SystemDepend.h"
#include "Controls/StaticBox.h"


PanelCalc *PanelCalc::self = nullptr;


PanelCalc::PanelCalc(wxWindow* parent, int x, int w, int /*_h*/) :
    wxPanel(parent)
{
    self = this;

    wxPanel::SetName("PanelCalc");

    wxPanel::SetSize({ MainWindow::WIDTH3, PanelConfig::HEIGHT - PanelConfig::HEIGHT_BUTTONS });
    wxPanel::SetPosition({ 0, PanelConfig::HEIGHT_BUTTONS });

    StaticBox *box = new StaticBox(this, "", { x, SD::DSBY() }, { w, 320 });

    {
        btnCursors = new wxButton(box, wxID_ANY, "Курсоры", SD::XY0(), { 100, 20 });
    }

    Bind(wxEVT_BUTTON, &PanelCalc::OnEventButton, this);
}


void PanelCalc::OnEventButton(wxCommandEvent &event)
{
    wxObject *obj = event.GetEventObject();

    if (obj == btnCursors)
    {
        if (!wndCursors)
        {
            wndCursors = new WindowCalculation(TheMainWindow);

            wndCursors->Bind(wxEVT_CLOSE_WINDOW, [this](wxCloseEvent &event)
                {
                    wndCursors = nullptr;
                    event.Skip();
                });
        }

        if (wndCursors)
        {
            wndCursors->ShowModal();
        }
    }

    event.Skip();
}
