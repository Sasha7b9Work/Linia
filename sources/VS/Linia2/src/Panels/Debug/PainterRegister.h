// 2025/6/4 15:46:57 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls/Painter.h"


class PanelRegister;


class PainterRegister : public Painter
{
public:

    PainterRegister(wxWindow *parent, PanelRegister *panel, const wxPoint &position, const wxSize &size);

    virtual void OnPaint(wxPaintEvent &) override;

private:

    PanelRegister *panel = nullptr;
};
