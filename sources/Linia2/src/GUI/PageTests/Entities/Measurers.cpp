// 2026/09/04 12:20:07 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "GUI/PageTests/Entities/Measurers.h"
#include "Utils/GlobalFunctions.h"


#define CREATE_BUTTONS_COMBO_RANGE(name, title, _x, _y)             \
    name = new ComboInput(this, title, WIDTH_CONTROL, titles, tooltips, #name); \
    name->SetPosition({ _x, _y });


MeasurerSourcer::MeasurerSourcer(Type::E _type, wxPaintDC &_dc, const wxPoint _center) :
    type(_type), center(_center)
{
    Draw(_dc);
}


void MeasurerSourcer::Draw(wxPaintDC &dc)
{
    dc.DrawCircle(center, radius);

    dc.SetFont(wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_EXTRALIGHT));

    static const pchar labels[Type::Count] =
    {
        "V",
        "I",
        "",
        ""
    };

    GF::DrawTextInCenter(dc, labels[type], wxRect(wxPoint{center.x - radius, center.y - radius}, wxPoint{center.x + radius, center.y + radius}));

    const int dY = 3;

    if (type == Type::SourceI)
    {
        const int ddY = 3;

        dc.DrawLine(center.x, center.y - dY - ddY, center.x, center.y - radius);
        dc.DrawLine(center.x, center.y + dY - ddY, center.x, center.y + radius);

        const int l = 5;

        dc.DrawLine(center.x, center.y - dY - ddY, center.x - l, center.y - dY + l - ddY);
        dc.DrawLine(center.x, center.y - dY + dY - ddY, center.x - l, center.y - dY + l + dY - ddY);

        dc.DrawLine(center.x, center.y - dY - ddY, center.x + l, center.y - dY + l - ddY);
        dc.DrawLine(center.x, center.y - dY + dY - ddY, center.x + l, center.y - dY + l + dY - ddY);
    }
    else if (type == Type::SourceU)
    {
        dc.DrawLine(center.x, center.y - radius + dY, center.x + dY, center.y + dY);
        dc.DrawLine(center.x, center.y - radius + dY, center.x - dY, center.y + dY);

        dc.DrawLine(center.x, center.y + radius - 5, center.x, center.y - radius + 6);
    }
}


void MeasurerSourcer::DrawBorder(wxPaintDC &dc, int &x, int &y, int r, Dir::E dir, int num_controls)
{
    const int d = 5;

    PaintDC paint(dc);

    paint.StorePenBrush();

    dc.SetPen({ *wxBLACK, 1, wxPENSTYLE_SHORT_DASH });
    dc.SetBrush(*wxTRANSPARENT_BRUSH);

    int width = WIDTH_CONTROL + d * 2;
    int height = (num_controls * (ButtonsCombo::HEIGHT + d)) + d;

    if (dir == Dir::Left)
    {
        x -= WIDTH_CONTROL + 2 * d + r;
        width += 2 * r + d;
        y -= height / 2;
        dc.DrawRectangle(x, y, width, height);
        x += d;
        y += d;
    }
    else if (dir == Dir::Up)
    {
        x = x - WIDTH_CONTROL / 2 - d;
        y -= d + r + (ButtonsCombo::HEIGHT + d) * num_controls;
        height += d + r * 2;

        dc.DrawRectangle(x, y, width, height);

        x += d;
        y += d;
    }
    else if (dir == Dir::Right)
    {
        x -= r + d;
        width += 2 * r + d;
        y -= height / 2;

        dc.DrawRectangle(x, y, width, height);

        y += d;
        x += d * 2 + r * 2;
    }
    if (dir == Dir::Down)
    {
        x = x - WIDTH_CONTROL / 2 - d;
        y = y - d - r;
        height += d + r * 2;

        dc.DrawRectangle(x, y, width, height);

        x += d;
        y += d * 2 + r * 2;
    }

    paint.RestorePenBrush();

}
