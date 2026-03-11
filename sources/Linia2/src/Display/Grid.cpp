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


void Grid::ResetCenter()
{
    wxSize display_size = Display::self->GetSize();

    center = { display_size.x / 2, display_size.y / 2 };
}


void Grid::Reset()
{
    scale = 1;

    ResetCenter();
}


int Grid::BottomY() const
{
    return TopY() + LengthAxis();
}


int Grid::TopY() const
{
    return center.y - (int)(rangeY.max / UnitsInCellY() * size_cell);
}


int Grid::LengthAxis() const
{
    return size_cell * NumCells();
}


int Grid::LeftX() const
{
    return center.x - (int)(rangeX.max / UnitsInCellX() * size_cell);
}


int Grid::RightX() const
{
    return LeftX() + LengthAxis();
}


void Grid::Draw(const std::vector<GraphEntity *> &entities)
{
    const int length = LengthAxis();

    const int x_left = LeftX();
    const int x_right = RightX();
    const int y_top = TopY();
    const int y_bottom = BottomY();

    int d = 5;

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

    d = size_cell / 5;

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

    wxColor background{ 230, 230, 230 };

    Text::SetFont();

    int d = 2;

    wxSize size = Display::self->GetSize();

    {
        // Подписываем горизонтульную ось

        wxPoint last_pos{ 0, 0 };   // Здесь отрисовано последнее значение

        // Значения
        for (int i = -100; i < 100; i++)
        {
            wxPoint coord = GetCoordPointAxisX(i);

            // Если влазит под сеткой
            if (BottomY() + 20 < size.y)
            {
                if (Math::InRange(coord.x, LeftX() + 1, RightX()))
                {
                    last_pos = { coord.x, coord.y + d };
                    Text(rangeX.GetValuePointAxis(i, NumCells())).DrawAboutCenterDown(last_pos.x, last_pos.y, true, background);
                }
            }
            else
            {
                if (Math::InRange(coord.x, LeftX() + 1, RightX()))
                {
                    last_pos = { coord.x, size.y - 25 };
                    Text(rangeX.GetValuePointAxis(i, NumCells())).DrawAboutCenterDown(last_pos.x, last_pos.y, true, background);
                }
            }
        }

        Text(rangeX.FullTitle()).DrawAboutCenterDown(last_pos.x < size.x ? last_pos.x : size.x - 20, last_pos.y, true, background);
    }

    {
        // Подписываем вертикальную ось

        wxPoint last_pos{ 0, 0 };   // Здесь отрисовано последнее значение

        for (int i = -100; i < 100; i++)
        {
            wxPoint coord = GetCoordPointAxisY(-i);

            // Если влазит слева от сетки
            if (LeftX() - 20 > 0)
            {
                if (Math::InRange(coord.y, TopY(), BottomY() - 1))
                {
                    last_pos = { coord.x - d, coord.y };
                    Text(rangeY.GetValuePointAxis(i, NumCells())).DrawAboutCenterLeft(last_pos.x, last_pos.y, true, background);
                }
            }
            else
            {
                if (Math::InRange(coord.y, TopY(), BottomY() - 1))
                {
                    last_pos = { d, coord.y };
                    Text(rangeY.GetValuePointAxis(i, NumCells())).DrawAboutCenterRigth(last_pos.x, last_pos.y, true, background);
                }
            }
        }

        d = 7;

        // Единицы измерения
        if (LeftX() - 20 > 0)
        {
            Text(rangeY.FullTitle()).DrawAboutCenterLeft(last_pos.x, last_pos.y < d ? d : last_pos.y, true, background);
        }
        else
        {
            Text(rangeY.FullTitle()).DrawAboutCenterRigth(last_pos.x, last_pos.y < d ? d : last_pos.y, true, background);
        }
    }
}


wxPoint Grid::GetCoordPointAxisX(int num) const
{
    return { center.x + size_cell * num, BottomY() };
}


wxPoint Grid::GetCoordPointAxisY(int num) const
{
    return { LeftX(), center.y + size_cell * num };
}


void Grid::MoveGridOn(const wxPoint &delta)
{
    if (scale == 1)
    {
        return;
    }

    center += delta;

    wxSize size = Display::self->GetSize();

    if (LengthAxis() < size.x)
    {
        if (LeftX() < 5)
        {
            center.x = 5 + LengthAxis() / 2;
        }
        if (RightX() > size.x - 8)
        {
            center.x = size.x - 8 - LengthAxis() / 2;
        }
    }

    if (LengthAxis() < size.y)
    {
        if (TopY() < 5)
        {
            center.y = 5 + LengthAxis() / 2;
        }
        if (BottomY() > size.y - 8)
        {
            center.y = size.y - 8 - LengthAxis() / 2;
        }
    }
}


void Grid::ScaleGridOn(const wxPoint &pos, int delta)
{
    wxPoint delta_center = center - pos;

    if (delta > 0 && scale < 8)
    {
        scale *= 2;

        center += delta_center;
    }
    else if (delta < 0 && scale > 1)
    {
        scale /= 2;

        center -= delta_center / 2;
    }

    if (scale == 1)
    {
        ResetCenter();
    }
}


void Grid::ScaleGridOnX(int delta)
{
    if (delta < 0)
    {
        rangeX *= 1.5;
    }
    else
    {
        rangeX *= 1 / 1.5;
    }

    ResetCenter();

    Display::self->Draw();
}


void Grid::ScaleGridOnY(int delta)
{
    if (delta < 0)
    {
        rangeY *= 1.5;
    }
    else
    {
        rangeY *= 1 / 1.5;
    }

    ResetCenter();

    Display::self->Draw();
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
    return rangeX.Amplitude() / NumCells();
}


double Grid::UnitsInCellY() const
{
    return rangeY.Amplitude() / NumCells();
}


int Grid::NumCells() const
{
    return 10 * scale;
}


wxPoint Grid::ValuesToCoord(double x, double y) const
{
    double cells_in_x = x / rangeX.Amplitude() * NumCells();

    double cells_in_y = y / rangeY.Amplitude() * NumCells();

    return { (int)(center.x + cells_in_x * size_cell + 0.5), (int)(center.y - cells_in_y * size_cell + 0.5) };
}


wxPoint2DDouble Grid::CoordToValues(const wxPoint &coord) const
{
    return {
        rangeX.Amplitude() * (coord.x - center.x) / (NumCells() * size_cell),
        rangeY.Amplitude() * (coord.y - center.y) / (NumCells() * size_cell)
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


double Range::Amplitude() const
{
    return 2.0 * max;
}


void Range::operator+=(const double &delta)
{
    max += delta;
}


void Range::operator*=(const double &delta)
{
    double center = 0.0;

    double amplitude = Amplitude();

    amplitude *= delta;

    max = center + amplitude / 2.0;
}


wxString Range::FullTitle() const
{
    wxString prefix;

    if (max >= 1e3)
    {
        prefix = "k";
    }
    else if (max >= 1.0)
    {
        prefix = "";
    }
    else if (max >= 1e-3)
    {
        prefix = "m";
    }
    else if (max >= 1e-6)
    {
        prefix = "u";
    }
    else if (max >= 1e-9)
    {
        prefix = "n";
    }

    return title + "," + prefix + units;
}


wxString Range::GetValuePointAxis(int num, int cells_in_axis) const
{
    double step = Amplitude() / cells_in_axis;

    if (max >= 1e3)
    {
        step /= 1e3;
    }
    else if (max >= 1)
    {
        step *= 1.0;
    }
    else if (max >= 1e-3)
    {
        step *= 1e3;
    }
    else if (max >= 1e-6)
    {
        step *= 1e6;
    }
    else
    {
        step *= 1e9;
    }

    return wxString::Format("%.1f", step * num);
}
