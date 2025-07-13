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
    const int x_left = center.x - size_cell * num_cells / 2;
    const int y_top = center.y - size_cell * num_cells / 2;

    int length = size_cell * num_cells;

    for (int i = 0; i < 3; i++)
    {
        int x = center.x - size_cell * num_cells / 2;
        int y = center.y + (i - 1) * (num_cells / 2 * size_cell);
        PanelGraph::self->DrawLine(x, y, x + num_cells * size_cell, y, *wxBLACK);

        x = center.x + (i - 1) * (num_cells / 2 * size_cell);
        y = center.y - size_cell * num_cells / 2;

        PanelGraph::self->DrawLine(x, y, x, y + num_cells * size_cell);
    }

    for (int i = 0; i < 3; i++)
    {
        DrawVPointLine(x_left + i + 1, y_top, 8, length);
        DrawHPointLine(x_left, y_top + num_cells * size_cell - i - 1, 8, length);
    }

    for (int i = 0; i < 4; i++)
    {
        DrawVPointLine(x_left + (i + 1) * size_cell, y_top, 4, length);
        DrawVPointLine(center.x + (i + 1) * size_cell, y_top, 4, length);
        DrawHPointLine(x_left, y_top + (i + 1) * size_cell, 4, length);
        DrawHPointLine(x_left, center.y + (i + 1) * size_cell, 4, length);
    }
}


void Grid::DrawVPointLine(int x, int y, int d, int height)
{
    for (int i = y; i < y + height; i += d)
    {
        Point().Draw(x, i);
    }
}


void Grid::DrawHPointLine(int x, int y, int d, int width)
{
    for (int i = x; i < x + width; i += d)
    {
        Point().Draw(i, y);
    }
}
