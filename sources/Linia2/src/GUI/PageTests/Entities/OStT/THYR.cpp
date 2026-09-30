// 2026/09/29 11:23:32 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "GUI/PageTests/Entities/OStT/THYR.h"
#include "Utils/LineDrawer.h"


THYR::THYR(Test *test) : DIOD(test)
{

}


void THYR::Draw(wxAutoBufferedPaintDC &dc, const wxPoint &c)
{
    DrawCase(dc, c);

    DIOD::DrawCommon(dc, c);

    FuncAfterDraw(dc);
}
