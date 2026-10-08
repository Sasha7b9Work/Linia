// 2026/04/29 16:01:42 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once
#include "GUI/Controls/Panel.h"
#include "Settings/Tests/Library/Library.h"
#include "GUI/Controls/ButtonCombo.h"
#include "GUI/PageTests/Entities/OStT/OStT.h"
#include "GUI/PageTests/MeasurersSourcers.h"
#include "GUI/PageTests/CommonControls.h"
#include "GUI/PageTests/SpecificControls.h"


class PanelViewTest : public Panel
{
    friend class SpecificControls;

public:

    PanelViewTest(wxWindow *);

    static PanelViewTest *self;

    void SetTest(Test *);

private:

    static const int radius_trans = 50;                     // От этого значения и Center() идёт всё построение изображения
    static const int d_combos = ButtonsCombo::HEIGHT + 5;   // Расстояние между элементами ввода по вертикали
    static const int Y_GROUND = 700;

    ComboInput *bcBaseNumMeasures = nullptr;            // Количество измерений

    MeasurersSourcers meas_src;
    CommonControls com_controls;
    SpecificControls spec_controls;

    void OnEventPaint(wxPaintEvent &);

    // Нарисовать испытуемый элемент
    OStT *CreateOStT();

    int CalculateCombos(ComboInput **, ComboInput **, ComboInput ** = nullptr, ComboInput ** = nullptr);

    wxPoint GetCenter() const;

    void DrawScheme(AutoBufferedPaintDC &);

    // Нарисовать значок земли
    void DrawGround(AutoBufferedPaintDC &);

    // Возвращает координаты точки, из которой выходит вертикальная линия коллектора
    wxPoint CoordinateCollector() const;
};
