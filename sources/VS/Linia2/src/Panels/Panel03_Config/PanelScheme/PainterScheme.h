// 2025/08/19 12:59:34 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls/Painter.h"


class PainterScheme : public Painter
{
public:
    PainterScheme(wxWindow *parent, const wxPoint &pos, const wxSize &size) :
        Painter(parent, pos, size)
    {
    }

    // Нарисовать схему в соответствии с установленными настройками
    void Build();

private:

    void DrawElement();
};
