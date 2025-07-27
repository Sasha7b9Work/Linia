// 2025/7/26 23:56:51 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel13_ReferenceGraphs.h"
#include "MainWindow.h"


PanelReferenceGraph *PanelReferenceGraph::self = nullptr;


PanelReferenceGraph::PanelReferenceGraph(wxWindow *parent) :
    Panel(parent, 0, 0, MainWindow::WIDTH, MainWindow::HEIGHT)
{
    self = this;

    wxSize size_button{ 75, BUTTON_HEIGHT };
    new wxButton(this, ID_BTN_RETURN_TO_MAIN_PAGE, _L("Закрыть"), { MainWindow::WIDTH - size_button.x - 5, 0 }, size_button);

    Bind(wxEVT_BUTTON, &PanelReferenceGraph::OnEventButton, this);
}


void PanelReferenceGraph::OnEventButton(wxCommandEvent &event)
{
    int id = event.GetId();

    if (id == ID_BTN_RETURN_TO_MAIN_PAGE)
    {
        MainWindow::self->SetMode(ModeMainWindow::Standard);
    }
}
