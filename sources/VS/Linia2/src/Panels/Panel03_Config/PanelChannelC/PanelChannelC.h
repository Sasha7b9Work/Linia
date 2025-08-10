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

    BmpButtonsCombo *comboScan = nullptr;   // Развёртка
    wxStaticBox *boxImpulse = nullptr;      // Развёртка - Импульс
    wxCheckBox *chbDutyCycle = nullptr;     // Развёртка - Скважность

    void Tune();
};