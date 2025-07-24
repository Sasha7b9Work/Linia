// 2025/6/1 18:08:43 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel05_Graph/Panel05_Graph.h"
#include "Panels/Panel09_Table.h"
#include "Panels/Panel11_Menu/Panel11_Menu.h"
#include "MainWindow.h"


PanelTable *PanelTable::self = nullptr;


PanelTable::PanelTable(wxWindow* parent) :
    Panel(parent, MainWindow::WIDTH1, MainWindow::HEIGTH1 + PanelGraph::HEIGHT, WIDTH, HEIGHT)
{
    self = this;

    grid = new wxGrid(this, wxID_ANY);

    grid->CreateGrid(50, 10);

    grid->SetScrollRate(10, 10);

    grid->AutoSizeColumns();

    // Размещаем grid в sizer для правильного масштабирования
    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(grid, 1, wxEXPAND | wxALL, 5);
    SetSizer(sizer);

    Layout();
}
