// 2025/7/23 14:20:52 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Controls/SpinBox.h"


SpinBox::SpinBox(wxWindow *parent, int id, pchar text, const wxPoint &position, const wxSize &size) :
    wxPanel(parent, wxID_ANY, position, size)
{
    new wxSpinCtrl(this, id, text, { 0, 0 }, size);
}
