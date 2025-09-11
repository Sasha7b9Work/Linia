// 2025/6/3 13:44:28 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PanelDebug/PanelDebug.h"
#include "MainWindow.h"
#include "Panels/PanelDebug/NotebookChips.h"


PanelDebug *PanelDebug::self = nullptr;


PanelDebug::PanelDebug(wxWindow *parent) :
    Panel(parent, 0, 0, MainWindow::WIDTH, MainWindow::HEIGHT)
{
    self = this;

    new NotebookChips(this);

    wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);

    sizer->Add(NotebookChips::self, 1, wxEXPAND);

    SetSizer(sizer);

    wxPanel::Layout();
}


bool PanelDebug::Show(bool show)
{
    show ? NotebookChips::self->Init() : NotebookChips::self->DeInit();

    return wxPanel::Show(show);
}
