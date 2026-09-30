// 2026/09/29 11:10:33 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "GUI/PageTests/Entities/OStT/DARL.h"


DARL::DARL(Test *test) : BJT(test)
{

}


void DARL::Draw(wxAutoBufferedPaintDC &dc, const wxPoint &c)
{
    DrawCase(dc, c);

    int dx = (int)(radius * 0.3);
    int dy = (int)(radius * 0.3);

    radius.Store();

    radius.value /= 4;

    DrawCommon(dc, { c.x - dx, c.y - dy }, false);

    DrawCommon(dc, { c.x + dx, c.y + dy }, false);

    radius.Restore();
}
