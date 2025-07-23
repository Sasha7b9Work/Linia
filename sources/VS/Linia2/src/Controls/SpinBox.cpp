// 2025/7/23 14:20:52 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Controls/SpinBox.h"


SpinBox::SpinBox(wxWindow *parent, int id, pchar text, const wxPoint &position, const wxSize &size) :
    wxPanel(parent, wxID_ANY, position, size)
{
    const int width_btn = 10;

    wxSize size_text = size;
    size_text.x -= width_btn;

    new wxTextCtrl(this, wxID_ANY, text, { 0, 0 }, size_text);

    wxSize size_btn = { width_btn, size.y / 2 };

    new wxButton(this, wxID_ANY, ".", { size_text.x, 0 }, size_btn);

    new wxButton(this, wxID_ANY, ".", { size_text.x, size_btn.y }, size_btn);
}
