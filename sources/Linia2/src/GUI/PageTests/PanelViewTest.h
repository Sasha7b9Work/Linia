// 2026/04/29 16:01:42 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once
#include "GUI/Controls/Panel.h"
#include "Settings/Tests/Library/Library.h"
#include "GUI/Controls/ButtonCombo.h"
#pragma warning(push, 0)
    #include <wx/dcclient.h>
#pragma warning(pop)


// \todo Элемент предназначен для ввода числового значения.
class ComboInput : public ButtonsComboRange
{
public:
    ComboInput(wxWindow *parent, const wxString &title, int width,
        const wxArrayString &labels,
        const wxArrayString &tooltips,
        const wxString &name) :
        ButtonsComboRange(parent, title, width, labels, tooltips, name)
    {
    }
};


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

    ButtonsCombo *bcModeScan = nullptr;                  // Режим развёртки
    ButtonsCombo *bcTypeSemiconductor = nullptr;         // npn или pnp

    ButtonsCombo *bcBaseModeControl = nullptr;           // Режим управления базой - током или напряжением
    ComboInput *bcBaseStartValueI = nullptr;      // Стартовое значение задаваемой величины
    ComboInput *bcBaseDeltaValueI = nullptr;      // Шаг изменения задаваемой величины
    ButtonsCombo *bcBaseNumMeasures = nullptr;           // Количество измерений
    ComboInput *bcBaseMeasureRangeU = nullptr;    // Диапазон - максимальное доступное значение
    ComboInput *bcBaseMeasureLimitU = nullptr;    // Максимальное значение, при котором происходит завершение измерения

    ButtonsCombo *bcSubstrateModeControl = nullptr;
    ComboInput *bcSubstrateStartValue = nullptr;
    ComboInput *bcSubstrateDeltaValue = nullptr;
    ButtonsCombo *bcSubstrateNumMeasures = nullptr;
    ComboInput *bcSubstrateRangeMeasure = nullptr;
    ComboInput *bcSubstrateRangeLimit = nullptr;

    ButtonsCombo *bcCollectorModeSource = nullptr;          // Режим работы источника в коллекторе - высокое напряжение или большой ток
    ComboInput *bcCollectorValueStart = nullptr;     // Начальное значение испытательного напряжения Uк
    ComboInput *bcCollectorValueFinish = nullptr;    // Конечное значение испытательного напряжения Uк
    ComboInput *bcCollectorMeasureRangeI = nullptr;
    ComboInput *bcCollectorMeasureLimitI = nullptr;
    ComboInput *bcCollectorMeasureRangeU = nullptr;
    ComboInput *bcCollectorMeasureLimitU = nullptr;

    void OnEventPaint(wxPaintEvent &);

    void OnChangedModeScan(wxCommandEvent &);
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
    void DrawElement();

    // type == "npn", "pnp"
    void DrawBJT(const wxString &type, const wxPoint &);
    void DrawBJTS(const wxString &type, const wxPoint &);

    // Нарисовать блок измерителя тока коллектора
    // x, y - центр измерителя
    void DrawMeasurerCollectorI(int x, int y, Dir::E,
        ComboInput **cbRange, const wxString &, void (PanelViewTest:: *)(wxCommandEvent &),
        ComboInput **cbLimit, const wxString &, void (PanelViewTest::*)(wxCommandEvent &));

    void DrawMeasurerCollectorU(int x, int y, Dir::E,
        ComboInput **cbRange, const wxString &, void (PanelViewTest:: *)(wxCommandEvent &),
        ComboInput **cbLimit, const wxString &, void (PanelViewTest:: *)(wxCommandEvent &));

    void DrawSourceCollectorU(int x, int y, Dir::E,
        ComboInput **cbValueStart, const wxString &, void (PanelViewTest:: *)(wxCommandEvent &),
        ComboInput **cbValueFinish, const wxString &, void (PanelViewTest:: *)(wxCommandEvent &));

    // Создаёт группу элементов управления источником базы или подложки
    void CreateSourceBaseSubstrate(ComboInput **start, ComboInput **step, ComboInput **num_curves);

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
