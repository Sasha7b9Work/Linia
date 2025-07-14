// 2025/7/13 20:39:15 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel05_Graph/Grid.h"
#include "Panels/Panel05_Graph/Panel05_Graph.h"


Grid::Grid()
{
    center = { PanelGraph::WIDTH / 2, PanelGraph::HEIGHT / 2 };
}


int Grid::BottomY() const
{
    return TopY() + LengthAxis();
}


int Grid::TopY() const
{
    return center.y - SizeCell() * num_cells / 2;
}


int Grid::LengthAxis() const
{
    return SizeCell() * num_cells;
}


int Grid::LeftX() const
{
    return center.x - SizeCell() * num_cells / 2;
}


void Grid::Draw()
{
    const int size_cell = SizeCell();
    const int length = LengthAxis();

    const int x_left = LeftX();
    const int y_top = TopY();

    for (int i = 0; i < 3; i++)
    {
        int x = center.x - size_cell * num_cells / 2;
        int y = center.y + (i - 1) * (num_cells / 2 * size_cell);
        PanelGraph::self->DrawLine(x, y, x + num_cells * size_cell, y, *wxBLACK);

        x = center.x + (i - 1) * (num_cells / 2 * size_cell);
        y = center.y - size_cell * num_cells / 2;

        PanelGraph::self->DrawLine(x, y, x, y + num_cells * size_cell);
    }

    int d = 8 * scale;

    for (int i = 0; i < 3; i++)
    {
        DrawVPointLine(x_left + i + 1, y_top, d, length);
        DrawHPointLine(x_left, y_top + num_cells * size_cell - i - 1, d, length);
    }

    d = 4 * scale;

    for (int i = 0; i < 4; i++)
    {
        DrawVPointLine(x_left + (i + 1) * size_cell, y_top, d, length);
        DrawVPointLine(center.x + (i + 1) * size_cell, y_top, d, length);
        DrawHPointLine(x_left, y_top + (i + 1) * size_cell, d, length);
        DrawHPointLine(x_left, center.y + (i + 1) * size_cell, d, length);
    }

    DrawLabelsOnAxis();
}


void Grid::DrawLabelsOnAxis() const
{
    Text::SetFont();

    Text(unitsX).DrawAboutCenterDown(center.x, BottomY());
    Text(unitsY).DrawAboutCenterLeft(LeftX(), center.y);

    for (int i = -5; i < 6; i++)
    {
        if (i != 0)
        {
            wxPoint coord = GetPointAxisX(i);

            Text("0").DrawAboutCenterDown(coord.x, coord.y);
        }
    }
}


wxPoint Grid::GetPointAxisX(int num) const
{
    return { center.x + SizeCell() * num, BottomY() };
}


void Grid::MoveOn(const wxPoint &delta)
{
    center += delta;
}


int Grid::SizeCell() const
{
    return 40 * scale;
}


void Grid::ScaleOn(const wxPoint &pos, int delta)
{
    wxPoint delta_center = center - pos;

    if (delta > 0 && scale < 5)
    {
        scale++;

        center += delta_center;
    }
    else if (delta < 0 && scale > 1)
    {
        scale--;

        center -= delta_center / 2;
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
