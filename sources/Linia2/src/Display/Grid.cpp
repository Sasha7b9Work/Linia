// 2025/7/13 20:39:15 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Grid.h"
#include "Display/Display.h"
#include "MainWindow.h"
#include "Utils/Math.h"
#include "Display/GraphEntity.h"


Grid::Grid()
{
    Reset();
}


void Grid::CalculateCenter()
{
    wxSize display_size = Display::self->GetSize();

    center = { display_size.x / 2, display_size.y / 2 };
}


void Grid::Reset()
{
    scale = 1;

    CalculateCenter();
}


int Grid::BottomY() const
{
    return TopY() + LengthAxis();
}


int Grid::TopY() const
{
    return center.y - (int)(rangeY.max / UnitsInCellY() * SizeCell());
}


int Grid::LengthAxis() const
{
    return SizeCell() * num_cells;
}


int Grid::LeftX() const
{
    return center.x + (int)(rangeX.min / UnitsInCellX() * SizeCell());
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

    int d = 5 * scale;

    {
        // Горизонтальные линии
        Line(x_left, y_top, RightX(), y_top).Draw(*wxBLACK);

        if (Math::InRange(center.y, y_top, y_bottom))
        {
            DrawHPointLineRight2(center.x, center.y, d, length - (center.x - x_left));
            DrawHPointLineLeft2(center.x, center.y, d, length - (x_right - center.x));
        }

        Line(x_left, BottomY(), RightX(), BottomY()).Draw();

        // Вертикальные линии
        Line(x_left, y_top, x_left, BottomY()).Draw();

        if (Math::InRange(center.x, x_left, x_right))
        {
            DrawVPointLineDown2(center.x, center.y, d, length - (center.y - y_top));
            DrawVPointLineUp2(center.x, center.y, d, length - (y_bottom - center.y));
        }

        Line(RightX(), y_top, RightX(), BottomY()).Draw();
    }

    // Рисуем вертикальные линии справа от нуля
    for (int i = 1; i < 100; i++)
    {
        int x = center.x + i * size_cell;

        if (x < x_right)
        {
            if (x > x_left)
            {
                DrawVPointLineDown(x, center.y, d, length - (center.y - y_top));
                DrawVPointLineUp(x, center.y, d, length - (y_bottom - center.y));
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
                DrawVPointLineDown(x, center.y, d, length - (center.y - y_top));
                DrawVPointLineUp(x, center.y, d, length - (y_bottom - center.y));
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
                DrawHPointLineRight(center.x, y, d, length - (center.x - x_left));
                DrawHPointLineLeft(center.x, y, d, length - (x_right - center.x));
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
                DrawHPointLineRight(center.x, y, d, length - (center.x - x_left));
                DrawHPointLineLeft(center.x, y, d, length - (x_right - center.x));
            }
        }
        else
        {
            break;
        }
    }

    d = 10 * scale;

    for (int i = 1; i < 3; i++)
    {
        DrawVPointLineUp(x_left + i, center.y, d, length - (BottomY() - center.y));
        DrawVPointLineDown(x_left + i, center.y, d, length - (center.y - TopY()));

        DrawHPointLineRight(center.x, BottomY() - i, d, length - (center.x - LeftX()));
        DrawHPointLineLeft(center.x, BottomY() - i, d, length - (RightX() - center.x));
    }

    for (auto *entity : entities)
    {
        entity->Draw(this);
    }

    Display::self->FillRectangle(0, 0, x_left - 1, Display::self->GetSize().y, *wxWHITE); //-V807
    Display::self->FillRectangle(x_left, 0, length, y_top - 1, *wxWHITE);
    Display::self->FillRectangle(x_right + 1, 0, Display::self->GetSize().x - x_right, Display::self->GetSize().y, *wxWHITE);
    Display::self->FillRectangle(x_left, y_bottom + 1, length, Display::self->GetSize().y - y_bottom, *wxWHITE);

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
            Text(rangeX.FullTitle()).DrawAboutCenterDown(center.x, BottomY() + 25);
        }

        for (int i = -100; i < 100; i++)
        {
            wxPoint coord = GetCoordPointAxisX(i);

            if (BottomY() < Display::self->GetSize().y)
            {
                if (Math::InRange(coord.x, LeftX() + 1, RightX()))
                {
                    Text(rangeX.GetValuePointAxis(i)).DrawAboutCenterDown(coord.x, coord.y + d);
                }
            }
            else
            {
                if (Math::InRange(coord.x, LeftX() + 1, RightX()))
                {
                    Text(rangeX.GetValuePointAxis(i)).DrawAboutCenterDown(coord.x, Display::self->GetSize().y - 25, true, *wxWHITE);
                }
            }
        }
    }

    {
        if (Math::InRange(center.y, TopY(), BottomY()))
        {
            Text(rangeY.FullTitle()).DrawAboutCenterLeft(LeftX() - 30, center.y);
        }

        for (int i = -100; i < 100; i++)
        {
            wxPoint coord = GetCoordPointAxisY(-i);

            if (LeftX() > 0)
            {
                if (Math::InRange(coord.y, TopY(), BottomY() - 1))
                {
                    Text(rangeY.GetValuePointAxis(i)).DrawAboutCenterLeft(coord.x - d, coord.y);
                }
            }
            else
            {
                if (Math::InRange(coord.y, TopY(), BottomY() - 1))
                {
                    Text(rangeY.GetValuePointAxis(i)).DrawAboutCenterRigth(0 + d, coord.y, true, *wxWHITE);
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

    rangeX += delta_x;

    units_on_pixel = UnitsInCellY() / SizeCell();

    double delta_y = delta.y * units_on_pixel;

    rangeY += delta_y;

    center += delta;
}


void Grid::ScaleMeasuresOnX(int delta)
{
    if (delta < 0)
    {
        rangeX *= 1.5;
    }
    else
    {
        rangeX *= 1 / 1.5;
    }

    CalculateCenter();

    Display::self->Draw();
}


void Grid::ScaleMeasuresOnY(int delta)
{
    if (delta < 0)
    {
        rangeY *= 1.5;
    }
    else
    {
        rangeY *= 1 / 1.5;
    }

    CalculateCenter();

    Display::self->Draw();
}


int Grid::SizeCell() const
{
    return 60 * scale;
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


void Grid::DrawVPointLineDown(int x, int y, int d, int height)
{
    for (int i = y; i < y + height; i += d)
    {
        Point().Draw(x, i);
    }
}


void Grid::DrawVPointLineUp(int x, int y, int d, int height)
{
    for (int i = y; i > y - height; i -= d)
    {
        Point().Draw(x, i);
    }
}


void Grid::DrawHPointLineRight(int x, int y, int d, int width)
{
    for (int i = x; i < x + width; i += d)
    {
        Point().Draw(i, y);
    }
}


void Grid::DrawHPointLineLeft(int x, int y, int d, int width)
{
    for (int i = x; i > x - width; i -= d)
    {
        Point().Draw(i, y);
    }
}


void Grid::DrawVPointLineDown2(int x, int y, int d, int height)
{
    for (int i = y; i < y + height; i += d)
    {
        Point().Draw(x, i);
        Point().Draw(x, i + 1);
        Point().Draw(x, i + 2);
    }
}


void Grid::DrawVPointLineUp2(int x, int y, int d, int height)
{
    for (int i = y; i > y - height; i -= d)
    {
        Point().Draw(x, i);
        Point().Draw(x, i - 1);
        Point().Draw(x, i - 2);
    }
}


void Grid::DrawHPointLineRight2(int x, int y, int d, int width)
{
    for (int i = x; i < x + width; i += d)
    {
        Point().Draw(i, y);
        Point().Draw(i + 1, y);
        Point().Draw(i + 2, y);
    }
}


void Grid::DrawHPointLineLeft2(int x, int y, int d, int width)
{
    for (int i = x; i > x - width; i -= d)
    {
        Point().Draw(i, y);
        Point().Draw(i - 1, y);
        Point().Draw(i - 2, y);
    }
}


double Grid::UnitsInCellX() const
{
    return rangeX.Amplitude() / num_cells;
}


double Grid::UnitsInCellY() const
{
    return rangeY.Amplitude() / num_cells;
}


wxPoint Grid::ValuesToCoord(double x, double y) const
{
    double cells_in_x = x / rangeX.Amplitude() * 10.0;

    double cells_in_y = y / rangeY.Amplitude() * 10.0;

    return { (int)(center.x + cells_in_x * SizeCell() + 0.5), (int)(center.y - cells_in_y * SizeCell() + 0.5) };
}


wxPoint2DDouble Grid::CoordToValues(const wxPoint &coord) const
{
    return {
        rangeX.Amplitude() * (coord.x - center.x) / (10.0 * SizeCell()),
        rangeY.Amplitude() * (coord.y - center.y) / (10.0 * SizeCell())
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


void Grid::OnEventCnangeMeasuredElement()
{

}


double Range::MaxAbs() const
{
    double _min = std::fabs(min);
    double _max = std::fabs(max);

    return (_min > _max) ? _min : _max;
}


double Range::Amplitude() const
{
    return max - min;
}


void Range::operator+=(const double &delta)
{
    min += delta;
    max += delta;
}


void Range::operator*=(const double &delta)
{
    double center = (max + min) / 2.0;

    double amplitude = Amplitude();

    amplitude *= delta;

    min = center - amplitude / 2.0;
    max = center + amplitude / 2.0;
}


wxString Range::FullTitle() const
{
    wxString prefix;

    if (MaxAbs() >= 1e3)
    {
        prefix = "k";
    }
    else if (MaxAbs() >= 1.0)
    {

    }
    else if (MaxAbs() >= 1e-3)
    {
        prefix = "m";
    }
    else if (MaxAbs() >= 1e-6)
    {
        prefix = "u";
    }
    else if (MaxAbs() >= 1e-9)
    {
        prefix = "n";
    }

    return title + "," + prefix + units;
}


wxString Range::GetValuePointAxis(int num) const
{
    double step = Amplitude() / 10.0;   // По горизонтали всегда 10 клеток

    if (MaxAbs() >= 1e3)
    {
        step /= 1e3;
    }
    else if (MaxAbs() >= 1)
    {

    }
    else if (MaxAbs() >= 1e-3)
    {
        step *= 1e3;
    }
    else if (MaxAbs() >= 1e-6)
    {
        step *= 1e6;
    }
    else
    {
        step *= 1e9;
    }

    return wxString::Format("%.1f", step * num);
}
