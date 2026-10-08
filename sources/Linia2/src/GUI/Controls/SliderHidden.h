// 2026/10/06 16:28:42 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once
#include "GUI/Controls/ButtonCombo.h"


// Представляет собой слайдер, спрятанный под кнопкой


class WindowSlider;


class SliderHidden : public DrawingButton
{
public:

    SliderHidden(wxWindow *, int width, const wxString &title, const wxString &name_file);

    void SetRange(double, double);

private:

    WindowSlider *wndSlider = nullptr;      // Окошко, которое будет открываться по нажатию кнопки

    double min = 0.0;
    double max = 0.0;
};
