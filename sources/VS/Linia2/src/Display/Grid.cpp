// 2025/7/13 20:39:15 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Grid.h"
#include "Display/Display.h"
#include "Display/WindowScale.h"
#include "Utils/Math.h"
#include <algorithm>


Grid::Grid()
{
    CalculateCenter();
}


void Grid::CalculateCenter()
{
    center.x = Display::self->GetSize().x / 2 + SizeCell() * 5                      // Перемещаемся к правой границе сетки
        - (int)(WindowScale::rangeX.max / UnitsInCellX() * (double)SizeCell());     // И отсчитываем назад - влево

    center.y = Display::HEIGHT / 2 - SizeCell() * 5
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
    const int x_right = RightX();
    const int y_top = TopY();
    const int y_bottom = BottomY();

    {
        // Горизонтальные линии
        Line(x_left, y_top, RightX(), y_top).Draw(*wxBLACK);

        if (Math::InRange(center.y, y_top, y_bottom))
        {
            Line(x_left, center.y, RightX(), center.y).Draw();
        }

        Line(x_left, BottomY(), RightX(), BottomY()).Draw();

        // Вертикальные линии
        Line(x_left, y_top, x_left, BottomY()).Draw();

        if (Math::InRange(center.x, x_left, x_right))
        {
            Line(center.x, y_top, center.x, BottomY()).Draw();
        }

        Line(RightX(), y_top, RightX(), BottomY()).Draw();
    }

    int d = 4 * scale;

    // Рисуем вертикальные линии справа от нуля
    for (int i = 1; i < 100; i++)
    {
        int x = center.x + i * size_cell;

        if (x < x_right)
        {
            if (x > x_left)
            {
                DrawVPointLine(x, y_top, d, length);
            }
        }
        else
        {
            break;
        }
    }

    // Рисуем вертикальные линии слева от нуля
    for (int i = 1; i < 100; i++)
    {
        int x = center.x - i * size_cell;

        if (x > x_left)
        {
            if (x < x_right)
            {
                DrawVPointLine(x, y_top, d, length);
            }
        }
        else
        {
            break;
        }
    }

    // Рисуем горизонтальные линии сверху от нуля
    for (int i = 1; i < 100; i++)
    {
        int y = center.y - i * size_cell;

        if (y > y_top)
        {
            if (y < y_bottom)
            {
                DrawHPointLine(x_left, y, d, length);
            }
        }
        else
        {
            break;
        }
    }

    // Рисуем горизонтальные линии снизу от нуля
    for (int i = 1; i < 100; i++)
    {
        int y = center.y + i * size_cell;

        if (y < y_bottom)
        {
            if (y > y_top)
            {
                DrawHPointLine(x_left, y, d, length);
            }
        }
        else
        {
            break;
        }
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
    Display::self->SetColor(*wxBLACK);

    Text::SetFont();

    int d = 2;

    {
        if (Math::InRange(center.x, LeftX(), RightX()))
        {
            Text(WindowScale::rangeX.FullTitle()).DrawAboutCenterDown(center.x, BottomY() + 25);
        }

        for (int i = -100; i < 100; i++)
        {
            wxPoint coord = GetCoordPointAxisX(i);

            if (BottomY() < Display::HEIGHT)
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
                    Text(WindowScale::rangeX.GetValuePointAxis(i)).DrawAboutCenterDown(coord.x, Display::HEIGHT - 25, true, *wxWHITE);
                }
            }
        }
    }

    {
        if (Math::InRange(center.y, TopY(), BottomY()))
        {
            Text(WindowScale::rangeY.FullTitle()).DrawAboutCenterLeft(LeftX() - 30, center.y);
        }

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


void Grid::MoveGridOn(const wxPoint &delta)
{
    center += delta;
}


void Grid::MoveMeasuresOn(const wxPoint &delta)
{
    double units_on_pixel = UnitsInCellX() / SizeCell();

    double delta_x = -delta.x * units_on_pixel;

    WindowScale::rangeX += delta_x;

    units_on_pixel = UnitsInCellY() / SizeCell();

    double delta_y = delta.y * units_on_pixel;

    WindowScale::rangeY += delta_y;

    center += delta;
}


void Grid::ScaleMeasuresOn(const wxPoint &, int delta)
{
    if (delta < 0)
    {
        WindowScale::rangeX *= 1.5;
        WindowScale::rangeY *= 1.5;
    }
    else
    {
        WindowScale::rangeX *= 1 / 1.5;
        WindowScale::rangeY *= 1 / 1.5;
    }

    CalculateCenter();

    Display::self->Draw();
}


void Grid::ScaleMeasuresOnX(int delta)
{
    if (delta < 0)
    {
        WindowScale::rangeX *= 1.5;
    }
    else
    {
        WindowScale::rangeX *= 1 / 1.5;
    }

    CalculateCenter();

    Display::self->Draw();
}


void Grid::ScaleMeasuresOnY(int delta)
{
    if (delta < 0)
    {
        WindowScale::rangeY *= 1.5;
    }
    else
    {
        WindowScale::rangeY *= 1 / 1.5;
    }

    CalculateCenter();

    Display::self->Draw();
}


int Grid::SizeCell() const
{
    return 40 * scale;
}


void Grid::ScaleGridOn(const wxPoint &pos, int delta)
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
    if (Display::self->mouse_is_pressed)
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

    Display::self->SetColor(*wxBLACK);

    wxPoint2DDouble value = CoordToValues(pos_mouse);

    Text(wxString::Format("%.1f : %.1f", value.m_x, -value.m_y)).DrawAboutRightUp(pos_mouse.x + 5, pos_mouse.y - 5, true, *wxWHITE, true);

    if (Display::self->track_y)
    {
        Line(LeftX(), pos_mouse.y, RightX(), pos_mouse.y).Draw(*wxBLACK);
    }

    if (Display::self->track_x)
    {
        Line(pos_mouse.x, TopY(), pos_mouse.x, BottomY()).Draw(*wxBLACK);
    }
}
