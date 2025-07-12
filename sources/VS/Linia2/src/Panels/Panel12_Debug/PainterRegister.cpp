// 2025/6/4 15:50:26 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel12_Debug/PainterRegister.h"
#include "Panels/Panel12_Debug/PanelRegister.h"


PainterRegister::PainterRegister(wxWindow *parent, PanelRegister *_panel, const wxPoint &position) :
    Painter::Painter(parent, position, { 750, 150 }),
    panel(_panel)
{
    wxSize size(20, 20);

    chbox.resize((uint)panel->bit_depth);

    for (int i = 0; i < panel->bit_depth; i++)
    {
        wxCheckBox *chb = new wxCheckBox(this, wxID_ANY, "", { 40 + i * size.x, W_B + 1 }, size);

        if (panel->reverse_bits)
        {
            chbox[(uint)i] = chb;
        }
        else
        {
            chbox[(uint)(panel->bit_depth - i - 1)] = chb;
        }
    }
}


void PainterRegister::OnPaint(wxPaintEvent &)
{
    if (first_paint)
    {
        first_paint = false;

        for (int i = 0; i < panel->bit_depth; i++)
        {
            SetHintCheckBox(i);
        }
    }

    wxPaintDC _dc(this);

    wxGraphicsContext *gc = wxGraphicsContext::Create(_dc);

    gc->SetPen(*wxGREEN_PEN);

    gc->SetBrush(*wxWHITE_BRUSH);

    gc->DrawRectangle(0, 0, GetSize().x - 1, GetSize().y - 1);

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

    gc->SetBrush(*wxWHITE_BRUSH);

    for (int i = 0; i < 2; i++)
    {
        DrawDescriptions(i, gc);
    }

    gc->SetFont(GetDefaultFont(8), *wxBLACK);

    if (panel->reverse_bits)
    {
        int y = 24;
        gc->DrawText("DB0", 5, y);
        gc->DrawText(wxString::Format("DB%d", panel->bit_depth - 1), W_B * panel->bit_depth + 45, y);
    }
    else
    {
        int y = 24;
        gc->DrawText("DB0", W_B * panel->bit_depth + 45, y);
        gc->DrawText(wxString::Format("DB%d", panel->bit_depth - 1), 5, y);
    }

    delete gc;
}


void PainterRegister::SetHintCheckBox(int num_bit)
{
    wxString hint = wxString("Name : ") + panel->names_bits[(uint)num_bit];

    wxString desc0 = GetDescription(0, num_bit);

    if (!desc0.empty())
    {
        hint += "\n";
        hint += "Descriptin 1 : ";
        hint += desc0;
    }

    wxString desc1 = GetDescription(1, num_bit);

    if (!desc1.empty())
    {
        hint += "\n";
        hint += "Descriptin 2 : ";
        hint += desc1;
    }

    chbox[(uint)num_bit]->SetToolTip(hint);
}


wxPoint PainterRegister::CoordBit(int num_bit)
{
    if (!panel->reverse_bits)
    {
        num_bit = panel->bit_depth - num_bit - 1;
    }

    return { 36 + num_bit * 20, 0 };
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


wxString PainterRegister::GetDescription(int index_desc, int num_bit)
{
    if (panel->desc[index_desc].empty())
    {
        return "";
    }

    std::vector<StructDescription> &desc = panel->desc[index_desc];

    for (uint i = 0; i < desc.size(); i++)
    {
        if (num_bit >= desc[i].first_bit)
        {
            if (num_bit < desc[i].first_bit + desc[i].num_bits)
            {
                return desc[i].desc;
            }
        }
    }

    return "";
}
