// 2025/6/3 13:44:28 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PanelDebug/PanelDebug.h"
#include "MainWindow.h"
#include "Panels/PanelDebug/Notebook/NotebookDebug.h"
#include "Panels/PanelDebug/PanelRight.h"


PanelDebug *ThePanelDebug = nullptr;


PanelDebug::PanelDebug(wxWindow *parent) :
    Panel(parent, 0, 0, MainWindow::WIDTH, MainWindow::HEIGHT)
{
    ThePanelDebug = this;

    new NotebookDebug(this);

    new PanelRight(this);

    wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);

    sizer->Add(NotebookDebug::self, 1, wxEXPAND);

    sizer->Add(ThePanelRight, 0, wxEXPAND);

    SetSizer(sizer);

    wxPanel::Layout();
    wxPanel::Fit();

    Unpack();
}


bool PanelDebug::Show(bool show)
{
    show ? NotebookDebug::self->Init() : NotebookDebug::self->DeInit();

    return wxPanel::Show(show);
}


void PanelDebug::Update()
{
    NotebookDebug::self->PeriodicTask();

//    ThePanelRight->PeriodicTask();
}


void PanelDebug::Unpack()
{
    NotebookDebug::self->Unpack();
}


void PanelDebug::Pack()
{
    NotebookDebug::self->Pack();
}
