// 2025/7/13 20:39:15 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel05_Graph/Grid.h"
#include "Panels/Panel05_Graph/Panel05_Graph.h"
#include <algorithm>


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


int Grid::RightX() const
{
    return LeftX() + LengthAxis();
}


void Grid::Draw(const std::vector<GraphEntity *> &entities)
{
    const int size_cell = SizeCell();
    const int length = LengthAxis();

    const int x_left = LeftX();
    const int y_top = TopY();

    for (int i = 0; i < 3; i++)
    {
        int x = center.x - size_cell * num_cells / 2;
        int y = center.y + (i - 1) * (num_cells / 2 * size_cell);
        Line(x, y, x + num_cells * size_cell, y).Draw(*wxBLACK);

        x = center.x + (i - 1) * (num_cells / 2 * size_cell);
        y = center.y - size_cell * num_cells / 2;

        Line(x, y, x, y + num_cells * size_cell).Draw();
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

    for (auto *entity : entities)
    {
        entity->Draw(this);
    }

    DrawLabelsOnAxis();

    DrawMouseMarkers();
}


void Grid::DrawLabelsOnAxis() const
{
    PanelGraph::self->SetColor(*wxBLACK);

    Text::SetFont();

    int d = 2;

    {
        Text(FullTitleX()).DrawAboutCenterDown(center.x, BottomY() + 25);

        for (int i = -4; i < 6; i++)
        {
            wxPoint coord = GetCoordPointAxisX(i);

            if (BottomY() < PanelGraph::HEIGHT)
            {
                Text(GetValuePointAxisX(i)).DrawAboutCenterDown(coord.x, coord.y + d);
            }
            else
            {
                Text(GetValuePointAxisX(i)).DrawAboutCenterDown(coord.x, PanelGraph::HEIGHT - 25, true, *wxWHITE);
            }
        }
    }

    {
        Text(titleY).DrawAboutCenterLeft(LeftX() - 30, center.y);

        for (int i = -4; i < 6; i++)
        {
            wxPoint coord = GetCoordPointAxisY(-i);

            if (LeftX() > 0)
            {
                Text(GetValuePointAxisY(i)).DrawAboutCenterLeft(coord.x - d, coord.y);
            }
            else
            {
                Text(GetValuePointAxisY(i)).DrawAboutCenterRigth(0 + d, coord.y, true, *wxWHITE);
            }
        }
    }
}


wxString Grid::GetValuePointAxisX(int num) const
{
    double step = rangeX / 5.0;

    if (rangeX >= 1e3)
    {
        step /= 1e3;
    }
    else if (rangeX >= 1)
    {

    }
    else if (rangeX >= 1e-3)
    {
        step *= 1e3;
    }
    else if (rangeX >= 1e-6)
    {
        step *= 1e6;
    }
    else
    {
        step *= 1e9;
    }

    return wxString::Format("%.1f", step * num);
}


wxString Grid::GetValuePointAxisY(int num) const
{
    double step = rangeY / 5.0;

    if (rangeY >= 1e3)
    {
        step /= 1e3;
    }
    else if (rangeY >= 1)
    {

    }
    else if (rangeY >= 1e-3)
    {
        step *= 1e3;
    }
    else if (rangeY >= 1e-6)
    {
        step *= 1e6;
    }
    else
    {
        step *= 1e9;
    }

    return wxString::Format("%.1f", step * num);
}


wxString Grid::FullTitleX() const
{
    wxString prefix;

    if (rangeX >= 1e3)
    {
        prefix = "k";
    }
    else if (rangeX >= 1.0)
    {

    }
    else if (rangeX >= 1e-3)
    {
        prefix ="m";
    }
    else if (rangeX >= 1e-6)
    {
        prefix = "u";
    }
    else if (rangeX >= 1e-9)
    {
        prefix = "n";
    }

    return titleX + "," + prefix + unitsX;
}


wxPoint Grid::GetCoordPointAxisX(int num) const
{
    return { center.x + SizeCell() * num, BottomY() };
}


wxPoint Grid::GetCoordPointAxisY(int num) const
{
    return { LeftX(), center.y + SizeCell() * num };
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


wxPoint Grid::ValuesToCoord(double x, double y) const
{
    double cells_in_x = x / rangeX * 5.0;

    double cells_in_y = y / rangeY * 5.0;

    return { (int)(center.x + cells_in_x * SizeCell() + 0.5), (int)(center.y - cells_in_y * SizeCell() + 0.5) };
}


wxPoint2DDouble Grid::CoordToValues(const wxPoint &coord) const
{
//    double x = 1.0;

//    int coord_x = center.x + x / rangeX * 5.0 * SizeCell();

    /*
        coord_x - center.x = x / rangeX * 5 * SizeCell()
        (coord_x - center.x) / (5 * SizeCell()) = x / rangeX
        x = rangeX * (coord_x - center.x) / (5 * SizeCell())
    */

    return {
        rangeX * (coord.x - center.x) / (5.0 * SizeCell()),
        rangeY * (coord.y - center.y) / (5.0 * SizeCell())
    };
}


void Grid::OnMouseMove(const wxPoint &position)
{
    pos_mouse = position;
}


void Grid::DrawMouseMarkers() const
{
    if (PanelGraph::self->mouse_is_pressed)
    {
        return;
    }

    if (pos_mouse.y < TopY() ||
        pos_mouse.y > BottomY() ||
        pos_mouse.x < LeftX() ||
        pos_mouse.x > RightX())
    {
        return;
    }

    Text::SetFont();

    PanelGraph::self->SetColor(*wxBLACK);

    wxPoint2DDouble value = CoordToValues(pos_mouse);

    Text(wxString::Format("%.1f : %.1f", value.m_x, -value.m_y)).DrawAboutRightUp(pos_mouse.x + 5, pos_mouse.y - 5, true, *wxWHITE, true);

    if (PanelGraph::self->track_y)
    {
        Line(LeftX(), pos_mouse.y, RightX(), pos_mouse.y).Draw(*wxBLACK);
    }

    if (PanelGraph::self->track_x)
    {
        Line(pos_mouse.x, TopY(), pos_mouse.x, BottomY()).Draw(*wxBLACK);
    }
}
