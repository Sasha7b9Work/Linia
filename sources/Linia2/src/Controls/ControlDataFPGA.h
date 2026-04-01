// 2026/04/01 14:21:40 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls/Painter.h"


class PainterDataFPGA : public Painter
{
public:
    PainterDataFPGA(wxWindow *);
private:
};


class ControlDataFPGA : public wxPanel
{
public:

    const wxSize SIZE{ 200, 200 };

    ControlDataFPGA(wxWindow *, const wxPoint &);

private:

    PainterDataFPGA painter;
};
