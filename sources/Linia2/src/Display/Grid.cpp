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
    wxSize display_size = Display::self->GetDrawingSize();

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
    return center.y - (int)(rangeY.MaxAbs() / UnitsInCellY() * size_cell);
}


int Grid::LengthAxis() const
{
    return size_cell * NumCells();
}


int Grid::LeftX() const
{
    return center.x - (int)(rangeX.MaxAbs() / UnitsInCellX() * size_cell);
}


int Grid::RightX() const
{
    return LeftX() + LengthAxis();
}


void Grid::DrawArea() const
{
    if (scale == 1)
    {
        return;
    }

    wxSize size = Display::self->GetDrawingSize();

    int size_x = size.x * size.x / LengthAxis();
    int size_y = size.y * size.y / LengthAxis();

    int d_x = -LeftX() * size.x / LengthAxis();
    int d_y = -TopY() * size.y / LengthAxis();

    wxColor color{ 127, 127, 127 };

    Rect(size_x, 2).Fill(d_x, 0, color);
    Rect(2, size_y).Fill(size.x - 4, d_y, color);
}


void Grid::DrawNavigationWindow() const
{
    if (scale == 1 || !wxGetMouseState().LeftIsDown())
    {
        return;
    }

    wxSize size_window { 150, 150 };

    Display::self->FillRectangle(0, 0, size_window.x, size_window.y, { 240, 240, 240 });

    {
        wxSize size = Display::self->GetDrawingSize();

        int size_x = size.x * size_window.x / LengthAxis();
        int size_y = size.y * size_window.y / LengthAxis();

        int dx = -LeftX() * size_window.x / LengthAxis();
        int dy = -TopY() * size_window.y / LengthAxis();

        Rect(size_x, size_y).Fill(dx, dy, *wxWHITE);
    }

    Rect(size_window.x, size_window.y).Draw(0, 0, *wxBLACK);
}


void Grid::Draw(const std::vector<GraphEntity *> &entities)
{
    wxSize size = Display::self->GetDrawingSize();

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
            DrawHPointLineRight2(center.x, center.y, size.x, d);
            DrawHPointLineLeft2(center.x, center.y, 0, d);
        }

        Line(x_left, BottomY(), RightX(), BottomY()).Draw();

        // Вертикальные линии
        Line(x_left, y_top, x_left, BottomY()).Draw();

        if (Math::InRange(center.x, x_left, x_right))
        {
            DrawVPointLineDown2(center.x, center.y, size.y, d);
            DrawVPointLineUp2(center.x, center.y, 0, d);
        }

        Line(RightX(), y_top, RightX(), BottomY()).Draw();
    }

    // Рисуем вертикальные линии справа от нуля
    for (int i = 1; i < 100; i++)
    {
        int x = center.x + i * size_cell;

        if (x > 0 && x < size.x)
        {
            DrawVPointLineDown(x, center.y, size.y, d);
            DrawVPointLineUp(x, center.y, 0, d);
        }
    }

    // Рисуем вертикальные линии слева от нуля
    for (int i = 1; i < 100; i++)
    {
        int x = center.x - i * size_cell;

        if (x > 0 && x < size.x)
        {
            DrawVPointLineDown(x, center.y, size.y, d);
            DrawVPointLineUp(x, center.y, 0, d);
        }
    }

    // Рисуем горизонтальные линии сверху от нуля
    for (int i = 1; i < 100; i++)
    {
        int y = center.y - i * size_cell;

        if (y > 0 && y < size.y)
        {
            DrawHPointLineRight(center.x, y, size.x, d);
            DrawHPointLineLeft(center.x, y, 0, d);
        }
    }

    // Рисуем горизонтальные линии снизу от нуля
    for (int i = 1; i < 100; i++)
    {
        int y = center.y + i * size_cell;

        if (y > 0 && y < size.y)
        {
            DrawHPointLineRight(center.x, y, size.x, d);
            DrawHPointLineLeft(center.x, y, 0, d);
        }
    }

    d = size_cell / 5;

    for (int i = 1; i < 3; i++)
    {
        DrawVPointLineUp(x_left + i, center.y, 0, d);
        DrawVPointLineDown(x_left + i, center.y, size.y, d);

        DrawHPointLineRight(center.x, BottomY() - i, size.x, d);
        DrawHPointLineLeft(center.x, BottomY() - i, 0, d);
    }

    for (auto *entity : entities)
    {
        entity->Draw(this);
    }

    DrawArea();

    if (scale == 1)
    {
        Display::self->FillRectangle(0, 0, x_left - 1, Display::self->GetDrawingSize().y, *wxWHITE); //-V807
        Display::self->FillRectangle(x_left, 0, length, y_top - 1, *wxWHITE);
        Display::self->FillRectangle(x_right + 1, 0, Display::self->GetDrawingSize().x - x_right, Display::self->GetDrawingSize().y, *wxWHITE);
        Display::self->FillRectangle(x_left, y_bottom + 1, length, Display::self->GetDrawingSize().y - y_bottom, *wxWHITE);
    }

    DrawLabelsOnAxis();

    DrawMouseMarkers();

    DrawNavigationWindow();
}


void Grid::DrawLabelsOnAxis() const
{
    Display::self->SetColor(*wxBLACK);

    wxColor background{ 230, 230, 230 };

    Text::SetFont();

    int d = 2;

    wxSize size = Display::self->GetDrawingSize();

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

    center += delta * scale;

    FitIntoDisplay();
}


void Grid::FitIntoDisplay()
{
    if (scale == 1)
    {
        return;
    }

    wxSize size = Display::self->GetDrawingSize();

    if (LeftX() > 5)
    {
        center.x = 5 + LengthAxis() / 2;
    }

    if (TopY() > 5)
    {
        center.y = 5 + LengthAxis() / 2;
    }

    int d = 5;

    if (RightX() < size.x - d)
    {
        center.x = size.x - d - LengthAxis() / 2;
    }

    if (BottomY() < size.y - d)
    {
        center.y = size.y - d - LengthAxis() / 2;
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

    FitIntoDisplay();
}


void Grid::RangeGridOnX(int delta)
{
    if (delta < 0)
    {
        rangeX.Increase();
    }
    else
    {
        rangeX.Decrease();
    }

    Display::self->Draw();
}


void Grid::RangeGridOnY(int delta)
{
    if (delta < 0)
    {
        rangeY.Increase();
    }
    else
    {
        rangeY.Decrease();
    }

    Display::self->Draw();
}


void Grid::DrawVPointLineDown(int x, int y0, int y_low, int d)
{
    for (int i = y0; i < y_low; i += d)
    {
        Point().Draw(x, i);
    }
}


void Grid::DrawVPointLineUp(int x, int y0, int y_hi, int d)
{
    for (int i = y0; i > y_hi; i -= d)
    {
        Point().Draw(x, i);
    }
}


void Grid::DrawHPointLineRight(int x, int y, int x_right, int d)
{
    for (int i = x; i < x_right; i += d)
    {
        Point().Draw(i, y);
    }
}


void Grid::DrawHPointLineLeft(int x, int y, int x_left, int d)
{
    for (int i = x; i > x_left; i -= d)
    {
        Point().Draw(i, y);
    }
}


void Grid::DrawVPointLineDown2(int x, int y0, int y_low, int d)
{
    for (int i = y0; i < y_low; i += d)
    {
        Point().Draw(x, i);
        Point().Draw(x, i + 1);
        Point().Draw(x, i + 2);
    }
}


void Grid::DrawVPointLineUp2(int x, int y0, int y_hi, int d)
{
    for (int i = y0; i > y_hi; i -= d)
    {
        Point().Draw(x, i);
        Point().Draw(x, i - 1);
        Point().Draw(x, i - 2);
    }
}


void Grid::DrawHPointLineRight2(int x, int y, int x_right, int d)
{
    for (int i = x; i < x_right; i += d)
    {
        Point().Draw(i, y);
        Point().Draw(i + 1, y);
        Point().Draw(i + 2, y);
    }
}


void Grid::DrawHPointLineLeft2(int x, int y, int x_left, int d)
{
    for (int i = x; i > x_left; i -= d)
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


double Range::Amplitude() const
{
    return 2.0 * MaxAbs();
}


void Range::Increase()
{
    max.Increase();
}


void Range::Decrease()
{
    max.Decrease();
}


double Range::MaxAbs() const
{
    return max.MaxAbs();
}


double Range::Value::MaxAbs() const
{
    static double values[Type::Count] =
    {
        1.0,
        2.0,
        5.0
    };

    if (order == 0)
    {
        return values[type];
    }
    else if (order > 0)
    {
        double result = 1.0;
        for (int i = 0; i < order; i++)
        {
            result *= 10.0;
        }
        return result * values[type];
    }
    else
    {
        double result = 1.0;
        for (int i = order; i < 0; i++)
        {
            result *= 0.1;
        }
        return result * values[type];
    }
}


void Range::Value::Increase()
{
    if (MaxAbs() > 3e3)
    {
        return;
    }

    type = (Type)((int)type + 1);
    if (type == Count)
    {
        type = (Type)0;
        order++;
    }
}


void Range::Value::Decrease()
{
    if (MaxAbs() < 1e-11)
    {
        return;
    }

    int new_type = (int)type - 1;

    if (new_type < 0)
    {
        new_type = Count - 1;
        order--;
    }

    type = (Type)new_type;
}


wxString Range::FullTitle() const
{
    wxString prefix;

    double value = MaxAbs();

    if (value > 1e3)
    {
        prefix = "k";
    }
    else if (value > 1.0)
    {
        prefix = "";
    }
    else if((int64)(value * 1000) > 1)
    {
        prefix = "m";
    }
    else if ((int64)(value * 1000000) > 1)
    {
        prefix = "u";
    }
    else if ((int64)(value * 1e9) > 1)
    {
        prefix = "n";
    }
    else if ((int64)(value * 1e12) > 1)
    {
        prefix = "p";
    }

    return title + "," + prefix + units;
}


wxString Range::GetValuePointAxis(int num, int cells_in_axis) const
{
    double step = Amplitude() / cells_in_axis;

    double value = MaxAbs();

    if (value > 1e3)
    {
        step /= 1e3;
    }
    else if (value > 1)
    {
        step *= 1.0;
    }
    else if ((int64)(value * 1000) > 1)
    {
        step *= 1e3;
    }
    else if ((int64)(value * 1e6) > 1)
    {
        step *= 1e6;
    }
    else if ((int64)(value * 1e9) > 1)
    {
        step *= 1e9;
    }
    else if ((int64)(value * 1e12) > 1)
    {
        step *= 1e12;
    }

    return wxString::Format("%.1f", step * num);
}
