// 2026/09/29 11:23:32 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "GUI/PageTests/Entities/OStT/THYR.h"
#include "Utils/LineDrawer.h"


THYR::THYR(Test *test) : DIOD(test)
{

}


void THYR::Draw(wxAutoBufferedPaintDC &dc, const wxPoint &c)
{
    DIOD::DrawCommon(dc, c);

    DrawControlElectrode(dc, c);

    FuncAfterDraw(dc);
}


void THYR::DrawControlElectrode(wxAutoBufferedPaintDC &dc, const wxPoint &center) const
{
    const wxPoint d = Delta();

    if (DIOD::type == TypeDIOD::Common_Anode_P)
    {
        if (type == TypeTHYR::Control_Anode)
        {

        }
        else if (type == TypeTHYR::Control_Catode)
        {

        }
    }
    else if (DIOD::type == TypeDIOD::Common_Catode_N)
    {

    }
}
