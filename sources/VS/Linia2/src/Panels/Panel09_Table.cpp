// 2025/6/1 18:08:43 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel05_Graph/Panel05_Graph.h"
#include "Panels/Panel09_Table.h"
#include "Panels/Panel11_Menu/Panel11_Menu.h"
#include "MainWindow.h"


PanelTable *PanelTable::self = nullptr;


PanelTable::PanelTable(wxWindow* parent) :
    Panel(parent, MainWindow::WIDTH1, MainWindow::HEIGTH1 + PanelGraph::HEIGHT,
        MainWindow::WIDTH2, MainWindow::HEIGHT - MainWindow::HEIGTH1 - PanelGraph::HEIGHT - PanelButtons::HEIGHT)
{
    self = this;
}
