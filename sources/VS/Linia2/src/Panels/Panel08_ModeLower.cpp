// 2025/6/1 18:03:13 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel04_Model.h"
#include "Panels/Panel06_Percents.h"
#include "Panels/Panel08_ModeLower.h"
#include "Panels/Panel11_Menu/Panel11_Menu.h"
#include "MainWindow.h"


PanelModeLower *PanelModeLower::self = nullptr;


PanelModeLower::PanelModeLower(wxWindow *parent) :
    Panel(parent, 0, MainWindow::HEIGTH1 + PanelModel::HEIGHT + PanelPercents::HEIGHT,
        MainWindow::WIDTH1, MainWindow::HEIGHT - MainWindow::HEIGTH1 - PanelModel::HEIGHT - PanelPercents::HEIGHT - PanelButtons::HEIGHT)
{
    self = this;
}
