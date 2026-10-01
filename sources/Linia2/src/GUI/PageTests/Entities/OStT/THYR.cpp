// 2026/09/29 11:23:32 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "GUI/PageTests/Entities/OStT/THYR.h"


THYR::THYR(Test *test) : DIOD(test)
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

    if ((DIOD::type == CommonElectrode::Anode_P && type == ControlElectrode::Anode_P) ||
        (DIOD::type == CommonElectrode::Catode_N && type == ControlElectrode::Catode_N))
    {
            std::vector<wxPoint> points = IntersectLineCircle({ c.x, c.y - d.y }, { c.x - d.x, c.y + d.y }, c, radius);

            dc.MoveTo({ c.x - d.x, c.y + d.y });

            dc.LineTo(points[1]);
    }
    else if ((DIOD::type == CommonElectrode::Anode_P && type == ControlElectrode::Catode_N) ||
        (DIOD::type == CommonElectrode::Catode_N && type == ControlElectrode::Anode_P))
    {
        std::vector<wxPoint> points = IntersectLineCircle({ c.x, c.y + d.y }, { c.x - d.x, c.y - d.y }, c, radius);

        dc.MoveTo({ c.x - d.x, c.y - d.y });

        dc.LineTo(points[1]);
    }

    dc.LineOnDX(-dr);

    point_B = dc.GetCoord();
}
