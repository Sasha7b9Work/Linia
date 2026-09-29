// 2026/09/29 11:10:33 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "GUI/PageTests/Entities/OStT/DARL.h"


DARL::DARL(Test *test) : OStT3(test)
{

}


void DARL::Draw(wxAutoBufferedPaintDC &dc, const wxPoint &c)
{
    DrawCase(dc, c);
}
