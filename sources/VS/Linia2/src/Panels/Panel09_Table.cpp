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

    {
        titles_columns.Add(_L("Точка"));
        titles_columns.Add("Uc, V");
        titles_columns.Add("Ic, A");
        titles_columns.Add("Ib, A");
        titles_columns.Add(" ");
    }

    grid = new wxGrid(this, wxID_ANY);

    grid->SetRowLabelSize(0);

    grid->CreateGrid(50, 5);

    SetTitlesColumn();

    grid->SetScrollRate(10, 10);

    grid->AutoSizeColumns();

    // Размещаем grid в sizer для правильного масштабирования
    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(grid, 1, wxEXPAND | wxALL, 5);
    SetSizer(sizer);

    Layout();

    SetAutoSizeColumns();
}


void PanelTable::SetTitlesColumn()
{
    for (uint i = 0; i < 5; i++)
    {
        grid->SetColLabelValue((int)i, titles_columns[i]);
    }
}


void PanelTable::SetAutoSizeColumns()
{
    int gridWidth = grid->GetClientSize().GetWidth();
    int colsCount = grid->GetNumberCols();
    int colWidth = (gridWidth - grid->GetRowLabelSize()) / colsCount;

    for (int col = 0; col < colsCount; ++col)
    {
        grid->SetColSize(col, colWidth);
    }
}
