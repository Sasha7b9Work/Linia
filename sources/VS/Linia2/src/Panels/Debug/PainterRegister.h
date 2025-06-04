// 2025/6/4 15:46:57 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls/Painter.h"


class PainterRegister : public Painter
{
public:
    PainterRegister(wxWindow *parent, const wxPoint &position, const wxSize &size);
};
