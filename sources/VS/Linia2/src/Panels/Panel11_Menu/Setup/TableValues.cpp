// 2025/7/27 22:01:47 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel11_Menu/Setup/TableValues.h"


TableValues::TableValues(wxWindow *parent) :
    wxPanel(parent, wxID_ANY, wxDefaultPosition, {450, 399})
{
    SetSizeHints({ 450, 399 });
}


void TableValues::SetAll(wxVector<TableStruct> &_values)
{
    values = _values;

    const int d = 10;

    wxSize size1{ 90, TEXTCNTRL_HEIGHT };
    wxSize size2{ 130, TEXTCNTRL_HEIGHT };

    wxSize size{ (size1.x + size2.x) * 2 + d, size1.y * ((int)(values.size() + 1) / 2 + 1) };

    SetSizeHints(size);

    new wxStaticText(this, wxID_ANY, "Диапазон", { 0, 0 }, size1, wxALIGN_CENTER);
    new wxStaticText(this, wxID_ANY, "Значение", { size1.x, 0 }, size2, wxALIGN_CENTER);
    new wxStaticText(this, wxID_ANY, "Диапазон", { size1.x + size2.x + d, 0 }, size1, wxALIGN_CENTER);
    new wxStaticText(this, wxID_ANY, "Значение", { d + (size1.x * 2) + size2.x, 0}, size2, wxALIGN_CENTER);

    const int num_rows = (int)(values.size() + 1) / 2;

    const int num_cols = 2;

    auto it = values.begin();

    for (int col = 0; col < num_cols; col++)
    {
        for (int row = 0; row < num_rows; row++)
        {
            if (it != values.end())
            {
                int x = col * (size1.x + size2.x + d);
                int y = size1.y + row * size1.y;

                new wxStaticText(this, wxID_ANY, it->name, { x, y }, size1, wxALIGN_CENTER);

                x += size1.x;

                new wxTextCtrl(this, wxID_ANY, wxString::Format("%.15f", it->value), { x, y }, size2);

                it++;
            }
        }
    }
}
