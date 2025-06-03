// 2025/6/1 18:25:37 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel03_Config.h"
#include "Panels/Panel07_Indicator.h"
#include "MainWindow.h"


PanelIndicator *PanelIndicator::self = nullptr;


PanelIndicator::PanelIndicator(wxWindow* parent) :
    Panel(parent, MainWindow::WIDTH1 + MainWindow::WIDTH2, PanelConfig::HEIGHT,
        MainWindow::WIDTH3, HEIGHT)
{
    self = this;
}
