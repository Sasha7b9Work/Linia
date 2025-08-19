// 2025/8/9 09:54:32 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls/ButtonsCombo.h"


class PainterScheme;


class PanelScheme : public wxPanel
{
public:

    PanelScheme(wxPanel *parent, const int x, int w, int h);

private:

    ButtonsCombo *comboC = nullptr,
        *comboB = nullptr,
        *comboE = nullptr,
        *comboTest = nullptr;

    wxButton *btnLoad = nullptr;

    PainterBMP *painterJackB = nullptr;
    PainterBMP *painterJackC = nullptr;
    PainterBMP *painterJackS = nullptr;
    PainterBMP *painterJackE = nullptr;

    PainterScheme *painter = nullptr;                 // Здесь будет нарисована схема

    void OnEventButton(wxCommandEvent &);
    void OnEventComboBox(wxCommandEvent &);
};
