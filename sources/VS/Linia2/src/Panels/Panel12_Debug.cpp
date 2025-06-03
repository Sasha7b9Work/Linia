// 2025/6/3 13:44:28 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel12_Debug.h"
#include "MainWindow.h"


PanelDebug *PanelDebug::self = nullptr;


PanelDebug::PanelDebug(wxWindow *parent) :
    Panel(parent, 0, 0, MainWindow::WIDTH, MainWindow::HEIGHT)
{
    self = this;
}
