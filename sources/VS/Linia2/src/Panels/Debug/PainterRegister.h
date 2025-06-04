// 2025/6/4 15:46:57 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls/Painter.h"


class PanelRegister;


class PainterRegister : public Painter
{
public:

    // Ширина бита
    static const int W_B = 20;

    PainterRegister(wxWindow *parent, PanelRegister *panel, const wxPoint &position, const wxSize &size);

    virtual void OnPaint(wxPaintEvent &) override;

    wxFont GetDefaultFont(int size = -1);

private:

    PanelRegister *panel = nullptr;

    // Возвращает координаты верхнего левого угла отображения бита
    wxPoint CoordBit(int num_bit);

    void DrawTitleBit(int num_bit, const wxString &title, wxGraphicsContext *);

    void DrawDescriptions(int index, wxGraphicsContext *);
};
