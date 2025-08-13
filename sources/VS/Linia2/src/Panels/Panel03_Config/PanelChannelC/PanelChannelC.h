// 2025/8/9 10:39:59 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls/RadioButtonICO.h"
#include "Controls/BmpButtonsCombo.h"
#include "Controls/ButtonsCombo.h"


class PanelChannelC : public wxPanel
{
public:

    PanelChannelC(wxPanel *parent, int x, int w);

    static PanelChannelC *self;

private:

    ButtonsCombo    *comboVoltage = nullptr;        // Измеритель U
    ButtonsCombo    *comboCurrent = nullptr;        // Измеритель I
    ButtonsCombo    *comboRange = nullptr;          // Источник U - Диапазон
    BmpButtonsCombo *comboScan = nullptr;           // Развёртка
    wxCheckBox      *chbDutyCycle = nullptr;        // Развёртка - Скважность
    wxComboBox      *comboNumberPoints = nullptr;   // Развёртка - Число точек
    wxStaticText    *textLabelImpulse = nullptr;    // Развёртка - Длина импульса
    wxStaticText    *textValueImpulse = nullptr;    // Развёртка - Длина импульса (значение)

    void Tune();

    void OnEventComboBox(wxCommandEvent &);
};