// 2025/8/9 09:54:32 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls/PainterBMP.h"


class PanelScheme : public wxPanel
{
public:

    PanelScheme(wxPanel *parent, int x);

private:

    wxComboBox *comboC = nullptr,
        *comboB = nullptr,
        *comboE = nullptr,
        *comboTest = nullptr;

    wxButton *btnLoad = nullptr;

    PainterBMP *bmpCategory[10];                // Категория

    void OnEventCategoryBmpClick(wxMouseEvent &);
    void OnEventButton(wxCommandEvent &);
    void OnEventComboBox(wxCommandEvent &);
};
