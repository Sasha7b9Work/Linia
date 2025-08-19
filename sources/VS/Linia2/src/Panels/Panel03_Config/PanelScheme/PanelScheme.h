// 2025/8/9 09:54:32 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls/ButtonsCombo.h"
#include "Panels/Panel03_Config/PanelScheme/PainterScheme.h"
#include "Controls/BmpButtonsCombo.h"


class PanelScheme : public wxPanel
{
public:

    PanelScheme(wxPanel *parent, const int x, int w, int h);

private:

    ButtonsCombo *comboC = nullptr,
        *comboB = nullptr,
        *comboE = nullptr,
        *comboS = nullptr,
        *comboTest = nullptr;

    wxButton *btnLoad = nullptr;

    PainterBMP *painterJackB = nullptr;
    PainterBMP *painterJackC = nullptr;
    PainterBMP *painterJackS = nullptr;
    PainterBMP *painterJackE = nullptr;

    BmpButtonsCombo *comboCategory = nullptr;

    PainterScheme *painter = nullptr;                 // Здесь будет нарисована схема

    void OnEventButton(wxCommandEvent &);
    void OnEventComboBox(wxCommandEvent &);

    // Нарисовать схему в соответствии с установками
    void BuildPainter();
};
