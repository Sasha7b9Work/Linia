// 2026/09/29 11:16:27 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "GUI/PageTests/Entities/OStT/FETS.h"
#include "GUI/PageTests/Entities/OStT/FET.h"


FETS::FETS(Test *test) : FET(test)
{

}


void FETS::Draw(wxAutoBufferedPaintDC &dc, const wxPoint &c)
{
    FET::DrawCommon(dc, c);

    FuncAfterDraw(dc);
}
