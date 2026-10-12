// 2026/09/30 15:11:22 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "Utils/AutoBufferedPaintDC.h"


void AutoBufferedPaintDC::MoveTo(const wxPoint &point)
{
    coord = point;
}


void AutoBufferedPaintDC::MoveToY(int y)
{
    coord.y = y;
}


void AutoBufferedPaintDC::MoveOnDY(int dy)
{
    coord.y += dy;
}


void AutoBufferedPaintDC::LineTo(const wxPoint &point)
{
    DrawLine(coord, point);

    coord = point;
}


void AutoBufferedPaintDC::LineToX(int x)
{
    DrawLine(coord, { x, coord.y });

    coord.x = x;
}


void AutoBufferedPaintDC::LineOnDY(int dy)
{
    DrawLine(coord, { coord.x, coord.y + dy });

    coord.y += dy;
}


void AutoBufferedPaintDC::LineOnDX(int dx)
{
    DrawLine(coord, { coord.x + dx, coord.y });

    coord.x += dx;
}


void AutoBufferedPaintDC::LineOn(const wxPoint &d)
{
    DrawLine(coord, { coord.x + d.x, coord.y + d.y });

    coord += d;
}


wxPoint AutoBufferedPaintDC::GetCoord() const
{
    return coord;
}

void AutoBufferedPaintDC::MoveOnDX(int dx)
{
    DrawLine(coord, { coord.x + dx, coord.y });

    coord.x += dx;
}


void AutoBufferedPaintDC::LineToY(int y)
{
    DrawLine(coord, { coord.x, y });

    coord.y = y;
}


void AutoBufferedPaintDC::StorePenBrush()
{
    stored_pen = GetPen();
    stored_brush = GetBrush();
}


void AutoBufferedPaintDC::RestorePenBrush()
{
    SetPen(stored_pen);
    SetBrush(stored_brush);
}


void AutoBufferedPaintDC::StoreBrushAndSetNew(const wxBrush &brush)
{
    stored_brush = GetBrush();

    SetBrush(brush);
}


void AutoBufferedPaintDC::RestoreBrush()
{
    SetBrush(stored_brush);
}


void AutoBufferedPaintDC::StorePenAndSetNew(const wxPen &pen)
{
    stored_pen = GetPen();

    SetPen(pen);
}


void AutoBufferedPaintDC::RestorePen()
{
    SetPen(stored_pen);
}


void AutoBufferedPaintDC::DrawTextInCenter(const wxString &text, const wxRect &rect)
{
    wxCoord textWidth, textHeight;
    GetTextExtent(text, &textWidth, &textHeight);

    int x = rect.x + (rect.width - textWidth) / 2;
    int y = rect.y + (rect.height - textHeight) / 2;

    DrawText(text, x, y);
}
