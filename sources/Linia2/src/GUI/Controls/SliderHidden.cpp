// 2026/10/06 16:24:13 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "GUI/Controls/SliderHidden.h"


SliderHidden::SliderHidden(wxWindow *parent, int width, const wxString &title) :
    DrawingButton(parent, title, { width, ButtonsCombo::HEIGHT })
{

}


void SliderHidden::SetRange(double _min, double _max)
{
    min = _min;
    max = _max;
}
