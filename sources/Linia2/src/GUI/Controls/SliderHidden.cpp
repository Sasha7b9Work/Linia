// 2026/10/06 16:24:13 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "GUI/Controls/SliderHidden.h"
#pragma warning(push, 0)
    #include <wx/popupwin.h>
#pragma warning(pop)


class WindowSlider : public wxPopupTransientWindow
{
public:
    WindowSlider(SliderHidden *parent) : wxPopupTransientWindow(parent)
    {

    }
private:
};


SliderHidden::SliderHidden(wxWindow *parent, int width, const wxString &title, const wxString &name_file) :
    DrawingButton(parent, title, { width, ButtonsCombo::HEIGHT }, name_file)
{
    Bind(wxEVT_BUTTON, [this](wxCommandEvent &)
        {
            if (!wndSlider)
            {
                wndSlider = new WindowSlider(this);
            }

            wndSlider->Show(this);
        });
}


void SliderHidden::SetRange(double _min, double _max)
{
    min = _min;
    max = _max;
}
