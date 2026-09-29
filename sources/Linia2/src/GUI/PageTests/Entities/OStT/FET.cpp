// 2026/09/29 11:15:14 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "GUI/PageTests/Entities/OStT/FET.h"


FET::FET(Test *test) : OStT3(test)
{

}


void FET::Draw(wxAutoBufferedPaintDC &dc, const wxPoint &c)
{
    DrawCommon(dc, c, *this);
}


void FET::DrawCommon(wxAutoBufferedPaintDC &dc, const wxPoint &c, OStT &self)
{
    self.DrawCase(dc, c);
}
