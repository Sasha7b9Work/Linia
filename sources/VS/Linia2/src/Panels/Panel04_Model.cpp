// 2025/6/1 17:41:03 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel04_Model.h"
#include "MainWindow.h"


PanelModel *PanelModel::self = nullptr;


PanelModel::PanelModel(wxWindow* parent) :
    Panel(parent, 0, MainWindow::HEIGTH1, MainWindow::WIDTH1, HEIGHT)
{
    self = this;
}
