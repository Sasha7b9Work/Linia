// 2025/6/3 13:44:28 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PanelDebug/PanelDebug.h"
#include "MainWindow.h"
#include "Panels/PanelDebug/NotebookDebug.h"
#include "Utils/Configurator.h"


PanelDebug *PanelDebug::self = nullptr;


PanelDebug::PanelDebug(wxWindow *parent) :
    Panel(parent, 0, 0, MainWindow::WIDTH, MainWindow::HEIGHT)
{
    self = this;

    new NotebookDebug(this);

    wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);

    sizer->Add(NotebookDebug::self, 1, wxEXPAND);

    SetSizer(sizer);

    wxPanel::Layout();

    Unpack();
}


bool PanelDebug::Show(bool show)
{
    show ? NotebookDebug::self->Init() : NotebookDebug::self->DeInit();

    return wxPanel::Show(show);
}


void PanelDebug::Update()
{
    NotebookDebug::self->Update();
}


void PanelDebug::Unpack()
{
    NotebookDebug::self->Unpack();
}


void PanelDebug::Pack()
{
    NotebookDebug::self->Pack();
}
