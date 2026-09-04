// 2026/04/29 16:01:42 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once
#include "GUI/Controls/Panel.h"
#include "Settings/Tests/Library/Library.h"
#include "GUI/Controls/ButtonCombo.h"
#pragma warning(push, 0)
    #include <wx/dcclient.h>
#pragma warning(pop)


class PanelViewTest : public Panel
{
public:

    PanelViewTest(wxWindow *, PanelViewTest *&);

    void SetTest(Test *);

private:

    static const int radius = 50;       // От этого значения и Center() идёт всё построение изображения
    static const int WIDTH_CONTROL = 150;

    Test *test = nullptr;

    ButtonsCombo *bcModeScan = nullptr;                 // Режим развёртки
    ButtonsCombo *bcTypeSemiconductor = nullptr;        // npn или pnp

    ButtonsCombo *bcBaseModeControl = nullptr;          // Режим управления базой - током или напряжением
    ButtonsComboRange *bcBaseStartValue = nullptr;      // Стартовое значение задаваемой величины
    ButtonsComboRange *bcBaseDeltaValue = nullptr;      // Шаг изменения задаваемой величины
    ButtonsCombo *bcBaseNumMeasures = nullptr;          // Количество измерений
    ButtonsComboRange *bcBaseMeasureRange = nullptr;    // Диапазон - максимальное доступное значение
    ButtonsComboRange *bcBaseMeasureLimit = nullptr;      // Максимальное значение, при котором происходит завершение измерения

    ButtonsCombo *bcSubstrateModeControl = nullptr;
    ButtonsComboRange *bcSubstrateStartValue = nullptr;
    ButtonsComboRange *bcSubstrateDeltaValue = nullptr;
    ButtonsCombo *bcSubstrateNumMeasures = nullptr;
    ButtonsComboRange *bcSubstrateRangeMeasure = nullptr;
    ButtonsComboRange *bcSubstrateRangeLimit = nullptr;

    ButtonsCombo *bcCollectorModeSource = nullptr;          // Режим работы источника в коллекторе - высокое напряжение или большой ток
    ButtonsComboRange *bcCollectorValueStart = nullptr;     // Начальное значение испытательного напряжения Uк
    ButtonsComboRange *bcCollectorValueFinish = nullptr;    // Конечное значение испытательного напряжения Uк
    ButtonsComboRange *bcCollectorMeasureRange = nullptr;
    ButtonsComboRange *bcCollectorMeasureLimit = nullptr;

    void OnEventPaint(wxPaintEvent &);

    void OnEventComboBoxModeScan(wxCommandEvent &);
    void OnEventComboBoxTypeSemiconductor(wxCommandEvent &);
    void OnEventComboBoxBaseModeControl(wxCommandEvent &);
    void OnEventComboBoxBaseStartValue(wxCommandEvent &);
    void OnEventComboBoxBaseDeltaValue(wxCommandEvent &);
    void OnEventComboBoxBaseNumMeasures(wxCommandEvent &);
    void OnEventComboBoxBaseMeasureRange(wxCommandEvent &);
    void OnEventComboBoxBaseMeasureLimit(wxCommandEvent &);
    void OnEventComboBoxCollectorModeSource(wxCommandEvent &);
    void OnEventComboBoxCollectorValueStart(wxCommandEvent &);
    void OnEventComboBoxCollectorValueFinish(wxCommandEvent &);
    void OnEventComboBoxCollectorMeasureRange(wxCommandEvent &);
    void OnEventComboBoxCollectorMeasureLimit(wxCommandEvent &);

    // Нарисовать испытуемый элемент
    void DrawElement(wxPaintDC &);

    // type == "npn", "pnp"
    void DrawBJT(wxPaintDC &, const wxString &type, const wxPoint &);
    void DrawBJTS(wxPaintDC &, const wxString &type, const wxPoint &);

    // Нарисовать блок измерителя тока коллектора
    // x, y - центр измерителя
    void DrawMeasurerCollectorI(wxPaintDC &, int x, int y, ButtonsComboRange **cbRange, ButtonsComboRange **cbLimit);

    // Создать элементы управляения для данного теста
    void CreateControls();

    // Рисует линию длиной length под углом angleDeg
    void DrawLineWithAngle(wxPaintDC &dc, const wxPoint &start, double length, double angleDeg);

    wxPoint GetCenter() const;
};
