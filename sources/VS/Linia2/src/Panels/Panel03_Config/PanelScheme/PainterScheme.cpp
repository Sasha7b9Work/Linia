// 2025/08/19 12:59:37 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel03_Config/PanelScheme/PainterScheme.h"
#include "Device/Tests/Tests.h"
#include "Controls/Bitmap.h"


void PainterScheme::Build()
{
    BeginPaint();

    DrawElement();

    EndPaint();
}


void PainterScheme::DrawElement()
{
    static const pchar files[Category::Count] =
    {
        "sch/diod2.bmp",
        "sch/GASK.bmp",
        "sch/BCE_N.bmp",
        "sch/BCE_P.bmp",
        "sch/GDS_N.bmp",
        "sch/GDS_P.bmp",
        "sch/BCSE_N.bmp",
        "sch/BCSE_P.bmp",
        "sch/GDBS_N.bmp",
        "sch/GDBS_P.bmp"
    };

    DrawBitmap(40, 30, Bitmap::Get(files[Category::Current()]).GetBitmap(), {241, 241, 241});
}
