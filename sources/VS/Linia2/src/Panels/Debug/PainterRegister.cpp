// 2025/6/4 15:50:26 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Debug/PainterRegister.h"
#include "Panels/Debug/PanelRegister.h"


PainterRegister::PainterRegister(wxWindow *parent, PanelRegister *_panel, const wxPoint &position, const wxSize &size) :
    Painter::Painter(parent, position, size),
    panel(_panel)
{

}


void PainterRegister::OnPaint(wxPaintEvent &)
{
    wxPaintDC _dc(this);

    wxGraphicsContext *gc = wxGraphicsContext::Create(_dc);

    gc->SetPen(*wxBLACK_PEN);

    int num_bits = panel->bit_depth;

    int w = 20;
    int h = w;

    bool pen_is_white = false;           // Признак того, что заполнение идёт белым

    for (int i = 0; i < num_bits; i++)
    {
        if ((i % 8) == 0)
        {
            gc->SetBrush(pen_is_white ? *wxGREY_BRUSH : *wxWHITE_BRUSH);
            pen_is_white = !pen_is_white;
        }

        wxPoint coord = CoordBit(i);

        gc->DrawRectangle(coord.x, coord.y, w, h);
    }

    for (int i = 0; i < num_bits; i++)
    {
        DrawTitleBit(i, panel->names_bits[(uint)i], gc);
    }

    delete gc;
}


wxPoint PainterRegister::CoordBit(int num_bit)
{
    if (!panel->reverse_bits)
    {
        num_bit = panel->bit_depth - num_bit - 1;
    }

    return { 36 + num_bit * 20, 39 };
}


void PainterRegister::DrawTitleBit(int num_bit, const wxString &title, wxGraphicsContext *gc)
{
    wxFont font(7, wxFONTFAMILY_DEFAULT,
        wxFONTSTYLE_NORMAL,
        wxFONTWEIGHT_BOLD);

    gc->SetFont(font, *wxBLACK);

    wxPoint coord = CoordBit(num_bit);

    int d = (num_bit % 2) ? 2 : 7;

    // Получаем размеры текста
    wxDouble textWidth, textHeight;
    gc->GetTextExtent(title, &textWidth, &textHeight);

    if (textWidth > 20)
    {
        wxFont f(5, wxFONTFAMILY_DEFAULT,
            wxFONTSTYLE_NORMAL,
            wxFONTWEIGHT_BOLD);

        gc->SetFont(f, *wxBLACK);

        // Рассчитываем позицию для центрирования
        wxDouble x = coord.x + 15 - textWidth / 2;

        gc->DrawText(title, x, coord.y + d);
    }
    else
    {
        // Рассчитываем позицию для центрирования
        wxDouble x = coord.x + 11 - textWidth / 2;

        gc->DrawText(title, x, coord.y + d);
    }
}