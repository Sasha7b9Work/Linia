// 2025/6/1 17:50:50 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel06_Percents.h"
#include "Panels/Panel04_Model.h"
#include "MainWindow.h"


PanelPercents *PanelPercents::self = nullptr;


PanelPercents::PanelPercents(wxWindow* parent) :
    Panel(parent, 0, MainWindow::HEIGTH1 + PanelModel::HEIGHT, MainWindow::WIDTH1, HEIGHT)
{
    self = this;
}
