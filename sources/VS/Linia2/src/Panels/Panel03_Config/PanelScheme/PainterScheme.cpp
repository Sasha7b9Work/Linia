// 2025/08/19 12:59:37 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel03_Config/PanelScheme/PainterScheme.h"
#include "Device/Tests/Tests.h"
#include "Controls/Bitmap.h"


void PainterScheme::Build()
{
    BeginPaint();

    if (Category::Current() == Category::Diod)
    {
        DrawDiode();
    }

    EndPaint();
}


void PainterScheme::DrawDiode()
{
    DrawBitmap(10, 10, Bitmap::Get("sch/diod2.bmp").GetBitmap(), { 241, 241, 241 });
}
