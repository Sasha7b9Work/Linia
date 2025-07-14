// 2025/7/14 17:22:25 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel05_Graph/GraphEntity.h"
#include "Panels/Panel05_Graph/Panel05_Graph.h"


void GraphLine::Draw(const Grid *grid) const
{
    wxPoint start = grid->ValuesToCoord(x1, y1);
    wxPoint end = grid->ValuesToCoord(x2, y2);

    Line(start.x, start.y, end.x, end.y).Draw(color);
}


void GraphMeasures::AppendPoint(const wxPoint2DDouble &point)
{
    points.push_back(point);
}


void GraphMeasures::Draw(const Grid *grid) const
{
    PanelGraph::self->SetColor(color);

    Spline spline;

    for (auto &coord : points)
    {
        wxPoint point = grid->ValuesToCoord(coord.m_x, coord.m_y);

        spline.AppendPoint({ (double)point.x, (double)point.y });
    }

    spline.Draw(true, true);
}
