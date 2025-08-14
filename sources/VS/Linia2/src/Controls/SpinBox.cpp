// 2025/7/23 14:20:52 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Controls/SpinBox.h"


SpinBox::SpinBox(wxWindow *parent, const wxPoint &position, const wxSize &size, int _min, int _max) :
    wxPanel(parent, wxID_ANY, position, size),
    min(_min),
    max(_max)
{
    const int width_btn = 10;

    wxSize size_text = size;
    size_text.x -= width_btn;

    text = new wxTextCtrl(this, wxID_ANY, "", { 0, 0 }, size_text);

    text->SetEditable(false);

    text->SetValue(wxString::Format("%d", min));

    wxSize size_btn = { width_btn, size.y / 2 };

    new wxButton(this, wxID_ANY, ".", { size_text.x, 0 }, size_btn);

    new wxButton(this, wxID_ANY, ".", { size_text.x, size_btn.y }, size_btn);
}
