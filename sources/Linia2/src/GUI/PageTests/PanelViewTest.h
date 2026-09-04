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
    static const int WIDTH_CONTROL = 120;

    Test *test = nullptr;

    wxPaintDC *dc = nullptr;

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
    ButtonsComboRange *bcCollectorMeasureRangeI = nullptr;
    ButtonsComboRange *bcCollectorMeasureLimitI = nullptr;
    ButtonsComboRange *bcCollectorMeasureRangeU = nullptr;
    ButtonsComboRange *bcCollectorMeasureLimitU = nullptr;

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
    void OnEventComboBoxCollectorMeasureRangeI(wxCommandEvent &);
    void OnEventComboBoxCollectorMeasureLimitI(wxCommandEvent &);
    void OnEventComboBoxCollectorMeasureRangeU(wxCommandEvent &);
    void OnEventComboBoxCollectorMeasureLimitU(wxCommandEvent &);

    // Нарисовать испытуемый элемент
    void DrawElement();

    // type == "npn", "pnp"
    void DrawBJT(const wxString &type, const wxPoint &);
    void DrawBJTS(const wxString &type, const wxPoint &);

    // Нарисовать блок измерителя тока коллектора
    // x, y - центр измерителя
    void DrawMeasurerCollectorI(int x, int y, Dir::E,
        ButtonsComboRange **cbRange, void (PanelViewTest::*)(wxCommandEvent &),
        ButtonsComboRange **cbLimit, void (PanelViewTest::*)(wxCommandEvent &));

    void DrawMeasurerCollectorU(int x, int y, Dir::E,
        ButtonsComboRange **cbRange, void (PanelViewTest:: *)(wxCommandEvent &),
        ButtonsComboRange **cbLimit, void (PanelViewTest:: *)(wxCommandEvent &));

    void DrawSourceCollectorU(int x, int y, Dir::E,
        ButtonsComboRange **cbValueStart, void (PanelViewTest:: *)(wxCommandEvent &),
        ButtonsComboRange **cbValueFinish, void (PanelViewTest:: *)(wxCommandEvent &));

    // Нарисовать значок земли
    void DrawGround(int x, int y);

    // Нарисовать окантовку для измерителя или источника. x, y - центр измерителя
    void DrawBorder(int &x, int &y, int radius, Dir::E, int num_controls);

    // Создать элементы управляения для данного теста
    void CreateControls();

    // Рисует линию длиной length под углом angleDeg
    void DrawLineWithAngle(const wxPoint &start, double length, double angleDeg);

    wxPoint GetCenter() const;
};
