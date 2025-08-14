// 2025/08/14 11:24:06 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls/ButtonsCombo.h"
#include "Controls/SpinBox.h"
#include "Controls/Slider.h"


class PanelChannelB : public wxPanel
{
public:

    PanelChannelB(wxPanel *parent, int x, int w);

    static PanelChannelB *self;

private:

    ButtonsCombo *comboTypeGenerator = nullptr;     // Тип генерации - напряжение или ток
    ButtonsCombo *comboStep = nullptr;              // Амплитуда ступени

    void Tune();

    void OnEventComboBox(wxCommandEvent &);
};
