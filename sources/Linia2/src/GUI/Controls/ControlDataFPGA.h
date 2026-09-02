// 2026/04/01 14:21:40 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once
#include "GUI/Controls/Painter.h"
#include "GUI/Controls/Button.h"
#include "GUI/Controls/Panel.h"
#include <mutex>
#pragma warning(push, 0)
    #include <wx/stattext.h>
#pragma warning(pop)


class wxStatictext;


class PainterDataFPGA : public Painter
{
public:

    PainterDataFPGA(wxWindow *, const wxSize &);
};


class ControlDataFPGA : public Panel
{
public:

    const wxSize SIZE{ 185, 85 };

    // _for_value - сюда будет выводиться рассчитанное значение
    ControlDataFPGA(wxWindow *, wxStaticText *_for_value);

    void SetMax(int);

    void Draw();

    void SetData(int[POINTS_IN_SAMPLE_ADC]);

private:

    PainterDataFPGA *painter = nullptr;
    ToggleButton *btnScale = nullptr;
    int max = (1 << 18) - 1;
    int data[POINTS_IN_SAMPLE_ADC];
    wxStaticText *for_value = nullptr;

    void OnEventToggleButon(wxCommandEvent &);

    void OnEventPaint(wxPaintEvent &);
};
