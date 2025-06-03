// 2025/6/1 17:14:36 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel02_ModeUpper.h"
#include "MainWindow.h"


PanelModeUpper *PanelModeUpper::self = nullptr;


PanelModeUpper::PanelModeUpper(wxWindow* parent) :
    Panel(parent, MainWindow::WIDTH1, 0, MainWindow::WIDTH2, MainWindow::HEIGTH1)
{
    self = this;
}
