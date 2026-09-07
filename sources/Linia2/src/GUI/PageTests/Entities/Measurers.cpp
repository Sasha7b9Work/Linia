// 2026/09/04 12:20:07 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "GUI/PageTests/Entities/Measurers.h"
#include "Utils/GlobalFunctions.h"


void MeasurerSourcer::Draw(const wxPoint &_center)
{
    center = _center;

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
        const int ddY = 1;

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
    }
}
