// 2025/7/27 22:01:47 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel11_Menu/Setup/TableValues.h"
#include "Device/SettingsDevice.h"


TableValues::TableValues(wxWindow *parent) :
    wxPanel(parent, wxID_ANY, wxDefaultPosition, {450, 418})
{
    CreateFields();
}


void TableValues::CreateFields()
{
    CreateFields_U();
    CreateFields_I();
}


void TableValues::CreateFields_U()
{
    const int d = 10;

    wxSize size1{ 90, TEXTCNTRL_HEIGHT };
    wxSize size2{ 130, TEXTCNTRL_HEIGHT };

    int dh = 1;

    wxSize size{ (size1.x + size2.x) * 2 + d, (size1.y + dh) * ((int)(RangeU::Count + 1) / 2 + 1) };

    SetSizeHints(size);

    new wxStaticText(this, wxID_ANY, "Диапазон", { 0, 0 }, size1, wxALIGN_CENTER);
    new wxStaticText(this, wxID_ANY, "Значение", { size1.x, 0 }, size2, wxALIGN_CENTER);
    new wxStaticText(this, wxID_ANY, "Диапазон", { size1.x + size2.x + d, 0 }, size1, wxALIGN_CENTER);
    new wxStaticText(this, wxID_ANY, "Значение", { d + (size1.x * 2) + size2.x, 0}, size2, wxALIGN_CENTER);

    const int num_rows = (int)(RangeU::Count + 1) / 2;

    const int num_cols = 2;

    RangeU range = RangeU((RangeU::E)0);

    for (int col = 0; col < num_cols; col++)
    {
        for (int row = 0; row < num_rows; row++)
        {
            if (range.value < RangeU::Count)
            {
                int x = col * (size1.x + size2.x + d);
                int y = size1.y + row * (size1.y + dh);

                new wxStaticText(this, wxID_ANY, range.Name(), { x, y }, size1, wxALIGN_CENTER);

                x += size1.x;

                new wxTextCtrl(this, wxID_ANY, wxString::Format("%.15f", 0.0), { x, y }, size2);

                range++;
            }
        }
    }
}


void TableValues::CreateFields_I()
{

}
