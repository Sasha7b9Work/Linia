// 2026/03/19 16:45:31 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls/Painter.h"
#include "Utils/Timer.h"


class AnimatedImpulse : public PainterAnimated
{
public:

    static const int WIDTH = 150;
    static const int HEIGHT = 75;

    AnimatedImpulse(wxWindow *, const wxPoint &);

private:

    virtual void FuncDraw() override;

    TimeMeterMS meter;
    int x = 0;
};
