// 2026/09/29 11:10:33 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "GUI/PageTests/Entities/OStT/DARL.h"


DARL::DARL(Test *test) : BJT(test)
{

}


void DARL::Draw(AutoBufferedPaintDC &dc, const wxPoint &c)
{
    DrawCase(dc, c);

    int dx = (int)(radius * 0.3);
    int dy = (int)(radius * 0.3);

    radius.Store();
    dr.Store();
    length_arrow.Store();

    radius.value = 19;
    dr.value = 1;
    length_arrow.value = 10;

    DrawCommon(dc, { c.x - dx, c.y - dy + 5 }, false);          // Рисуем первый транзистор

    wxPoint pb1 = point_B;
    wxPoint pc1 = point_C;
    wxPoint pe1 = point_E;

    DrawCommon(dc, { c.x + dx, c.y + dy + 5 }, false);          // Рисуем второй транзистор

    wxPoint pb2 = point_B;
    wxPoint pc2 = point_C;
    wxPoint pe2 = point_E;

    radius.Restore();
    dr.Restore();
    length_arrow.Restore();

    {
        // Вывод базы

        std::vector<wxPoint> points = IntersectLineCircle(pb1, { pb1.x - 100, pb1.y }, c, (double)radius);

        dc.DrawLine(pb1, points[1]);

        dc.MoveTo(points[1]);
        dc.LineOnDX(-dr);

        point_B = dc.GetCoord();
    }

    {
        // Соединение между транзисторами

        dc.MoveTo(pe1);
        dc.LineOnDY(pb2.y - pe1.y);
        dc.LineOnDX(pb2.x - pe1.x);
    }

    {
        // Коллектор

        dc.MoveTo(pc1);
        dc.LineOnDX(pc2.x - pc1.x);

        std::vector<wxPoint> points = IntersectLineCircle(pc2, dc.GetCoord(), c, (double)radius);

        dc.MoveTo(pc2);
        dc.LineTo(points[1]);
        dc.LineOnDY(-dr);

        point_C = dc.GetCoord();
    }

    {
        // Эмиттер

        dc.MoveTo(pe2);

        std::vector<wxPoint> points = IntersectLineCircle(pe2, { pe2.x, pe2.y + 10 }, c, (double)radius);

        dc.LineTo(points[1]);
        dc.LineOnDY(dr);

        point_E = dc.GetCoord();
    }

    FuncAfterDraw(dc);
}
