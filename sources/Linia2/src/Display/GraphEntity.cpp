// 2025/7/14 17:22:25 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/GraphEntity.h"
#include "Display/Display.h"


void GraphLine::Draw(const Grid *grid) const
{
    wxPoint s = grid->ValuesToCoord(start.x.val, start.y.val);
    wxPoint e = grid->ValuesToCoord(end.x.val, end.y.val);

    Line(s.x, s.y, e.x, e.y).Draw(color);
}


void GraphMeasures::AppendPoint(const wxPoint2DDouble &point)
{
    points.push_back(point);
}


void GraphMeasures::Draw(const Grid *grid) const
{
    Display::self->SetColor(color);

    Spline spline;

    for (auto &coord : points)
    {
        wxPoint point = grid->ValuesToCoord(coord.m_x, coord.m_y);

        spline.AppendPoint({ (double)point.x, (double)point.y });
    }

    spline.Draw(true, true);
}
