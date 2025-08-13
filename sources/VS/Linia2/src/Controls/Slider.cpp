// 2025/08/13 14:19:26 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Controls/Slider.h"


Slider::Slider(wxWindow *parent, const wxPoint &position, int width) :
    wxPanel(parent, wxID_ANY, position, { width, TEXTCNTRL_HEIGHT + 5 }, wxBORDER_SIMPLE)
{
    new wxSlider(this, wxID_ANY, 0, 0, 100, { 0, 0 }, { width - 5, TEXTCNTRL_HEIGHT });
}
