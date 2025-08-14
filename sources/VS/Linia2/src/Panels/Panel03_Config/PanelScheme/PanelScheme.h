// 2025/8/9 09:54:32 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls/PainterBMP.h"
#include "Controls/ButtonsCombo.h"


class PanelScheme : public wxPanel
{
public:

    PanelScheme(wxPanel *parent, int x);

private:

    ButtonsCombo *comboC = nullptr,
        *comboB = nullptr,
        *comboE = nullptr,
        *comboTest = nullptr;

    wxButton *btnLoad = nullptr;

    void OnEventButton(wxCommandEvent &);
    void OnEventComboBox(wxCommandEvent &);
};
