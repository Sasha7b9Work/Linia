// 2025/6/1 18:18:16 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel03_Config.h"
#include "Panels/Panel07_Indicator.h"
#include "Panels/Panel10_Scale.h"
#include "Panels/Panel11_Menu/Panel11_Menu.h"
#include "MainWindow.h"


PanelScale *PanelScale::self = nullptr;


PanelScale::PanelScale(wxWindow* parent) :
    Panel(parent, MainWindow::WIDTH1 + MainWindow::WIDTH2, PanelConfig::HEIGHT + PanelIndicator::HEIGHT,
        MainWindow::WIDTH3, MainWindow::HEIGHT - PanelConfig::HEIGHT - PanelIndicator::HEIGHT)
{
    self = this;
}
