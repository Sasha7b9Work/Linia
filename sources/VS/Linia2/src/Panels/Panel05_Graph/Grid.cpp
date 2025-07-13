// 2025/7/13 20:39:15 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel05_Graph/Grid.h"
#include "Panels/Panel05_Graph/Panel05_Graph.h"


Grid::Grid()
{
    center = { PanelGraph::WIDTH / 2, PanelGraph::HEIGHT / 2 };
}


void Grid::Draw()
{
    for (int i = 0; i < 3; i++)
    {
        int x = center.x - size_cell * num_cells / 2;
        int y = center.y + (i - 1) * (num_cells / 2 * size_cell);
        PanelGraph::self->DrawLine(x, y, x + num_cells * size_cell, y, *wxBLACK);

        x = center.x + (i - 1) * (num_cells / 2 * size_cell);
        y = center.y - size_cell * num_cells / 2;

        PanelGraph::self->DrawLine(x, y, x, y + num_cells * size_cell);
    }
}
