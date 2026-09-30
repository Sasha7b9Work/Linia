// 2026/09/29 11:21:53 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "GUI/PageTests/Entities/OStT/RES.h"


RES::RES(Test *test) : OStT(test)
{

}


void RES::Draw(AutoBufferedPaintDC &dc, const wxPoint &c)
{
    const wxPoint d{ (int)(radius * 0.3), (int)(radius * 0.7) };

    dc.MoveTo({ c.x - d.x, c.y - d.y });

    dc.LineTo({ c.x + d.x, c.y - d.y });

    dc.LineTo({ c.x + d.x, c.y + d.y });

    dc.LineTo({ c.x - d.x, c.y + d.y });

    dc.LineTo({ c.x - d.x, c.y - d.y });

    {
        // Вывод коллектора

        dc.MoveTo({ c.x, c.y - d.y });

        dc.LineOnDY(-dr);

        point_C = dc.GetCoord();
    }

    {
        // Вывод эмиттера

        dc.MoveTo({ c.x, c.y + d.y });

        dc.LineOnDY(dr);

        point_E = dc.GetCoord();
    }
}
