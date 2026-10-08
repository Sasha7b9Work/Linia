// 2026/04/29 16:01:42 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once
#include "GUI/Controls/Panel.h"
#include "Settings/Tests/Library/Library.h"
#include "GUI/Controls/ButtonCombo.h"
#include "GUI/PageTests/Entities/OStT/OStT.h"
#include "GUI/PageTests/MeasurersSourcers.h"
#include "GUI/Controls/StaticText.h"
#include "GUI/Controls/StaticBox.h"
#include "GUI/PageTests/CommonControls.h"


class PanelViewTest : public Panel
{
public:

    PanelViewTest(wxWindow *);

    static PanelViewTest *self;

    void SetTest(Test *);

private:

    static const int radius_trans = 50;                     // От этого значения и Center() идёт всё построение изображения
    static const int d_combos = ButtonsCombo::HEIGHT + 5;   // Расстояние между элементами ввода по вертикали
    static const int Y_GROUND = 700;

    ComboInput *bcBaseNumMeasures = nullptr;            // Количество измерений

    ButtonsCombo *bcTypeBJT = nullptr;                  // Тип биполярного транзистора
    ButtonsCombo *bcTypeFET = nullptr;                  // Тип полевого транзистора
    ButtonsCombo *bcCommonElectrodeDIOD = nullptr;      // Для диода - общий электрод
    ButtonsCombo *bcControlElectrodeTHYR = nullptr;     // Дли тиристора - управляющий электрод

    MeasurersSourcers meas_src;
    CommonControls com_controls;

    void OnEventPaint(wxPaintEvent &);

    void OnChangedTypeSemiconductor(wxCommandEvent &);
    void OnChangedBaseModeControl(wxCommandEvent &);
    void OnChangedBaseStartValueI(wxCommandEvent &);
    void OnChangedBaseDeltaValueI(wxCommandEvent &);
    void OnChangedBaseNumMeasures(wxCommandEvent &);
    void OnChangedBaseMeasureRangeU(wxCommandEvent &);
    void OnChangedBaseMeasureLimitU(wxCommandEvent &);
    void OnChangedCollectorModeSource(wxCommandEvent &);
    void OnChangedCollectorValueStart(wxCommandEvent &);
    void OnChangedCollectorValueFinish(wxCommandEvent &);
    void OnChangedCollectorMeasureRangeI(wxCommandEvent &);
    void OnChangedCollectorMeasureLimitI(wxCommandEvent &);
    void OnChangedCollectorMeasureRangeU(wxCommandEvent &);
    void OnChangedCollectorMeasureLimitU(wxCommandEvent &);

    // Нарисовать испытуемый элемент
    OStT *CreateOStT();

    int CalculateCombos(ComboInput **, ComboInput **, ComboInput ** = nullptr, ComboInput ** = nullptr);

    // Специфичные органы управления для разных типов элементов (тип проводимости для биполярного транзистора, например)
    void CreateSpecificControls();
    void HideSpecificControls();
    void TuneSpecificControls();

    wxPoint GetCenter() const;

    void DrawScheme(AutoBufferedPaintDC &);

    // Нарисовать значок земли
    void DrawGround(AutoBufferedPaintDC &);

    // Возвращает координаты точки, из которой выходит вертикальная линия коллектора
    wxPoint CoordinateCollector() const;

    wxPoint CoordinateSpecificControl() const;
};
