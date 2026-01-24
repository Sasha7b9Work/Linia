// 2026/01/24 21:42:08 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PanelDebug/PageRight.h"
#include "MainWindow.h"


PanelRight *PanelRight::self = nullptr;


PanelRight::PanelRight(wxWindow *parent) : wxPanel(parent)
{
    self = this;

    wxSize size_button{ 75, BUTTON_HEIGHT };

    btnReturn = new wxButton(this, wxID_ANY, "Закрыть", { 10, 10 }, size_button);

    SetMinSize({ 100, -1 });

    Bind(wxEVT_BUTTON, &PanelRight::OnEventButton, this);
}


void PanelRight::OnEventButton(wxCommandEvent &event)
{
    int id = event.GetId();

    if (id == btnReturn->GetId())
    {
        MainWindow::self->SetMode(ModeMainWindow::Standard);
    }
}
