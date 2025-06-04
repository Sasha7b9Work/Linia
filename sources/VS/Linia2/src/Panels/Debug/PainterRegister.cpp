// 2025/6/4 15:50:26 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Debug/PainterRegister.h"
#include "Panels/Debug/PanelRegister.h"


PainterRegister::PainterRegister(wxWindow *parent, PanelRegister *_panel, const wxPoint &position) :
    Painter::Painter(parent, position, { 750, 150 }),
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

        int x = coord.x;
        if (!panel->reverse_bits)
        {
            x -= (d.num_bits - 1) * W_B;
        }
        int y = coord.y + 41 + index * W_B;
        int w = W_B * d.num_bits;
        int h = W_B;

        gc->DrawRectangle(x, y, w, h);

        DrawTextInCenter(x, y + 4, w, d.desc, 8, gc);
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
    wxPoint coord = CoordBit(num_bit);

    int d = (num_bit % 2) ? 2 : 6;

    DrawTextInCenter(coord.x, coord.y + d, W_B, title, 8, gc);
}


void PainterRegister::DrawTextInCenter(int x, int y, int width, const wxString &text, int size, wxGraphicsContext *gc)
{
    gc->SetFont(GetDefaultFont(size), *wxBLACK);

    wxDouble textWidth = 10000;
    wxDouble textHeight = 0;

    while (textWidth >= width)
    {
        gc->GetTextExtent(text, &textWidth, &textHeight);

        if (textWidth < width - 2)
        {
            break;
        }

        size--;
        gc->SetFont(GetDefaultFont(size), *wxBLACK);
    }

    gc->DrawText(text, x + width / 2 - textWidth / 2 + 1, y);
}
