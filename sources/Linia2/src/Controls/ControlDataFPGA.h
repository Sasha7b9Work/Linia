// 2026/04/01 14:21:40 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls/Painter.h"


class PainterDataFPGA : public Painter
{
public:

    PainterDataFPGA(wxWindow *, const wxSize &);

private:

    virtual void OnPaint(wxPaintEvent &) override;
};


class ControlDataFPGA : public wxPanel
{
public:

    const wxSize SIZE{ 185, 85 };

    ControlDataFPGA(wxWindow *, const wxPoint &);

private:

    PainterDataFPGA *painter = nullptr;

    wxToggleButton *btnScale = nullptr;
};
