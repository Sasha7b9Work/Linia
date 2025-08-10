// 2025/8/9 10:39:59 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls/RadioButtonICO.h"
#include "Controls/BmpButtonsCombo.h"


class PanelChannelC : public wxPanel
{
public:

    PanelChannelC(wxPanel *parent, int x, int w);

    static PanelChannelC *self;

    // Вызывается комбобоксом выбора развёртки
    void OnEventChangeComboScan();

private:

    wxComboBox *comboVoltage = nullptr;         // Измеритель U
    wxComboBox *comboCurrent = nullptr;         // Измеритель I
    wxComboBox *comboRange = nullptr;           // Источник U - Диапазон
    BmpButtonsCombo *comboScan = nullptr;       // Развёртка
    wxCheckBox *chbDutyCycle = nullptr;         // Развёртка - Скважность
    wxComboBox *comboNumberPoints = nullptr;    // Развёртка - Число точек
    wxStaticText *textLabelImpulse = nullptr;   // Развёртка - Длина импульса
    wxStaticText *textValueImpulse = nullptr;   // Развёртка - Длина импульса (значение)

    void Tune();
};