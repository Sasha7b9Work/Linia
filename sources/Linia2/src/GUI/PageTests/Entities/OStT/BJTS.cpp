// 2026/09/29 10:56:18 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "GUI/PageTests/Entities/OStT/BJTS.h"
#include "GUI/PageTests/Entities/OStT/BJT.h"
#include "Utils/LineDrawer.h"


BJTS::BJTS(Test *test) : BJT(test)
{

}


void BJTS::Draw(wxAutoBufferedPaintDC &dc, const wxPoint &c)
{
    BJT::DrawCommon(dc, c);

    FuncAfterDraw(dc);
}
