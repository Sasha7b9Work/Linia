// 2025/7/13 20:39:15 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel05_Graph/Grid.h"
#include "Panels/Panel05_Graph/Panel05_Graph.h"
#include "Panels/Panel05_Graph/WindowScale.h"
#include "Utils/Math.h"
#include <algorithm>


Grid::Grid()
{
    CalculateCenter();
}


void Grid::CalculateCenter()
{
    center.x = PanelGraph::WIDTH / 2 + SizeCell() * 5                              // Перемещаемся к правой границе сетки
        - (int)(WindowScale::rangeX.max / UnitsInCellX() * (double)SizeCell());     // И отсчитываем назад - влево

    center.y = PanelGraph::HEIGHT / 2 - SizeCell() * 5
        +(int)(WindowScale::rangeY.max / UnitsInCellY() * (double)SizeCell());
}


int Grid::BottomY() const
{
    return TopY() + LengthAxis();
}


int Grid::TopY() const
{
    return center.y - (int)(WindowScale::rangeY.max / UnitsInCellY() * SizeCell());
}


int Grid::LengthAxis() const
{
    return SizeCell() * num_cells;
}


int Grid::LeftX() const
{
    return center.x + (int)(WindowScale::rangeX.min / UnitsInCellX() * SizeCell());
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

    {
        // Горизонтальные линии
        Line(x_left, y_top, RightX(), y_top).Draw(*wxBLACK);

        Line(x_left, center.y, RightX(), center.y).Draw();

        Line(x_left, BottomY(), RightX(), BottomY()).Draw();

        // Вертикальные линии
        Line(x_left, y_top, x_left, BottomY()).Draw();

        Line(center.x, y_top, center.x, BottomY()).Draw();

        Line(RightX(), y_top, RightX(), BottomY()).Draw();
    }

    int d = 4 * scale;

    for (int i = 1; i < 10; i++)
    {
        DrawVPointLine(x_left + i * size_cell, y_top, d, length);
        DrawHPointLine(x_left, y_top + i * size_cell, d, length);
    }

    d = 8 * scale;

    for (int i = 1; i < 3; i++)
    {
        DrawVPointLine(x_left + i, y_top, d, length);
        DrawHPointLine(x_left, BottomY() - i, d, length);
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
        Text(WindowScale::rangeX.FullTitle()).DrawAboutCenterDown(center.x, BottomY() + 25);

        for (int i = -100; i < 100; i++)
        {
            wxPoint coord = GetCoordPointAxisX(i);

            if (BottomY() < PanelGraph::HEIGHT)
            {
                if (Math::InRange(coord.x, LeftX() + 1, RightX()))
                {
                    Text(WindowScale::rangeX.GetValuePointAxis(i)).DrawAboutCenterDown(coord.x, coord.y + d);
                }
            }
            else
            {
                if (Math::InRange(coord.x, LeftX() + 1, RightX()))
                {
                    Text(WindowScale::rangeX.GetValuePointAxis(i)).DrawAboutCenterDown(coord.x, PanelGraph::HEIGHT - 25, true, *wxWHITE);
                }
            }
        }
    }

    {
        Text(WindowScale::rangeY.FullTitle()).DrawAboutCenterLeft(LeftX() - 30, center.y);

        for (int i = -100; i < 100; i++)
        {
            wxPoint coord = GetCoordPointAxisY(-i);

            if (LeftX() > 0)
            {
                if (Math::InRange(coord.y, TopY(), BottomY() - 1))
                {
                    Text(WindowScale::rangeY.GetValuePointAxis(i)).DrawAboutCenterLeft(coord.x - d, coord.y);
                }
            }
            else
            {
                if (Math::InRange(coord.y, TopY(), BottomY() - 1))
                {
                    Text(WindowScale::rangeY.GetValuePointAxis(i)).DrawAboutCenterRigth(0 + d, coord.y, true, *wxWHITE);
                }
            }
        }
    }
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


double Grid::UnitsInCellX() const
{
    return WindowScale::rangeX.Amplitude() / num_cells;
}


double Grid::UnitsInCellY() const
{
    return WindowScale::rangeY.Amplitude() / num_cells;
}


wxPoint Grid::ValuesToCoord(double x, double y) const
{
    double cells_in_x = x / WindowScale::rangeX.Amplitude() * 10.0;

    double cells_in_y = y / WindowScale::rangeY.Amplitude() * 10.0;

    return { (int)(center.x + cells_in_x * SizeCell() + 0.5), (int)(center.y - cells_in_y * SizeCell() + 0.5) };
}


wxPoint2DDouble Grid::CoordToValues(const wxPoint &coord) const
{
    return {
        WindowScale::rangeX.Amplitude() * (coord.x - center.x) / (10.0 * SizeCell()),
        WindowScale::rangeY.Amplitude() * (coord.y - center.y) / (10.0 * SizeCell())
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
