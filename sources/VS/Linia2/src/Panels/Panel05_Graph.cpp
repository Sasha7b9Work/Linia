// 2025/6/1 17:46:49 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel05_Graph.h"
#include "MainWindow.h"


PanelGraph *PanelGraph::self = nullptr;


PanelGraph::PanelGraph(wxWindow* parent) :
    Panel(parent, MainWindow::WIDTH1, MainWindow::HEIGTH1, MainWindow::WIDTH2, HEIGHT)
{
    self = this;
}
