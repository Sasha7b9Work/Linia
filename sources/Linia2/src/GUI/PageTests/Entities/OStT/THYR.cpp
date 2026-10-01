// 2026/09/29 11:23:32 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "GUI/PageTests/Entities/OStT/THYR.h"


THYR::THYR() : DIOD()
{

}


void THYR::Draw(AutoBufferedPaintDC &dc, const wxPoint &c)
{
    DIOD::DrawCommon(dc, c);

    DrawControlElectrode(dc, c);
}


void THYR::DrawControlElectrode(AutoBufferedPaintDC &dc, const wxPoint &c)
{
    const wxPoint d = Delta();

    if ((commonElectrode.IsAnode() && controlElectrode.IsAnode()) ||
        (commonElectrode.IsCatode() && controlElectrode.IsCatode()))
    {
            std::vector<wxPoint> points = IntersectLineCircle({ c.x, c.y - d.y }, { c.x - d.x, c.y + d.y }, c, radius);

            dc.MoveTo({ c.x - d.x, c.y + d.y });

            dc.LineTo(points[1]);
    }
    else if ((commonElectrode.IsAnode() && controlElectrode.IsCatode()) ||
        (commonElectrode.IsCatode() && controlElectrode.IsAnode()))
    {
        std::vector<wxPoint> points = IntersectLineCircle({ c.x, c.y + d.y }, { c.x - d.x, c.y - d.y }, c, radius);

        dc.MoveTo({ c.x - d.x, c.y - d.y });

        dc.LineTo(points[1]);
    }

    dc.LineOnDX(-dr);

    point_B = dc.GetCoord();
}
