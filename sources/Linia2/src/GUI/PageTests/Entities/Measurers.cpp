// 2026/09/04 12:20:07 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "GUI/PageTests/Entities/Measurers.h"
#include "Utils/GlobalFunctions.h"


void MeasurerSourcer::Draw(const wxPoint &_center, pchar label)
{
    center = _center;

    dc.DrawCircle(center, radius);

    dc.SetFont(wxFont(18, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_EXTRALIGHT));

    GF::DrawTextInCenter(dc, label, wxRect(wxPoint{ center.x - radius, center.y - radius }, wxPoint{ center.x + radius, center.y + radius }));
}


void Voltmeter::Draw(const wxPoint &_center)
{
    MeasurerSourcer::Draw(_center, "V");
}


void Ampermeter::Draw(const wxPoint &_center)
{
    MeasurerSourcer::Draw(_center, "A");
}
