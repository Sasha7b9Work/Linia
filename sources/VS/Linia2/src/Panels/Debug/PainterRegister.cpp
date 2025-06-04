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
            wxBrush brush(wxColor(0xD0, 0xD0, 0xD0));

            gc->SetBrush(pen_is_white ? brush : *wxWHITE_BRUSH);
            pen_is_white = !pen_is_white;
        }

        wxPoint coord = CoordBit(i);

        gc->DrawRectangle(coord.x, coord.y, w, h);
    }

    for (int i = 0; i < num_bits; i++)
    {
        DrawTitleBit(i, panel->names_bits[(uint)i], gc);
    }

    for (int i = 0; i < 2; i++)
    {
        DrawDescriptions(i, gc);
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


void PainterRegister::DrawDescriptions(int index, wxGraphicsContext *gc)
{
    std::vector<StructDescription> &desc = panel->desc[index];

    for (uint i = 0; i < desc.size(); i++)
    {
        StructDescription &d = desc[i];

        wxPoint coord = CoordBit(d.first_bit);

        gc->DrawRectangle(coord.x - (d.num_bits - 1) * W_B, coord.y + 41 + index * W_B, W_B * d.num_bits, W_B);
    }
}


wxFont PainterRegister::GetDefaultFont(int size)
{
    wxFont font(7, wxFONTFAMILY_DEFAULT,
        wxFONTSTYLE_NORMAL,
        wxFONTWEIGHT_BOLD);

    if (size != -1)
    {
        font.SetPointSize(size);
    }

    return font;
}


void PainterRegister::DrawTitleBit(int num_bit, const wxString &title, wxGraphicsContext *gc)
{
    gc->SetFont(GetDefaultFont(), *wxBLACK);

    wxPoint coord = CoordBit(num_bit);

    int d = (num_bit % 2) ? 2 : 7;

    // Получаем размеры текста
    wxDouble textWidth, textHeight;
    gc->GetTextExtent(title, &textWidth, &textHeight);

    if (textWidth > W_B)
    {
        gc->SetFont(GetDefaultFont(5), *wxBLACK);

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