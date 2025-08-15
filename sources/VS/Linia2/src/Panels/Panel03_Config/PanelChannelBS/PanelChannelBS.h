// 2025/08/14 11:24:06 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls/ButtonsCombo.h"
#include "Controls/SpinBox.h"
#include "Controls/Slider.h"
#include "Controls/Buttons.h"


class PanelChannelBS : public wxPanel
{
    friend class PanelChannelB;
    friend class PanelChannelS;

private:

    PanelChannelBS(wxPanel *parent, int x, int w, int h);

    ButtonsCombo      *comboTypeGenerator = nullptr;    // Тип генерации - напряжение или ток
    ButtonsComboRange *comboStep = nullptr;             // Амплитуда ступени
    ButtonsComboRange *comboLimitRange = nullptr;       // Ограничение - Диапазон
    SliderFloatOffset *sliderOffset = nullptr;          // Смещение
    SliderFloatLimit  *sliderLimit = nullptr;           // Ограничение
    CheckButton       *btnAmpitudeDecrease = nullptr;   // Амплитуда / 10

    void Tune();

    void OnEventComboBox(wxCommandEvent &);
};


class PanelChannelB : public PanelChannelBS
{
public:

    PanelChannelB(wxPanel *parent, int x, int w, int h) :
        PanelChannelBS(parent, x, w, h)
    {
        self = this;
    }

    static PanelChannelB *self;
};


class PanelChannelS : public PanelChannelBS
{
public:

    PanelChannelS(wxPanel *parent, int x, int w, int h) :
        PanelChannelBS(parent, x, w, h)
    {
        self = this;
    }

    static PanelChannelS *self;
};

