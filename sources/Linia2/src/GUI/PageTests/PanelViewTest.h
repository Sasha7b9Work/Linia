// 2026/04/29 16:01:42 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once
#include "GUI/Controls/Panel.h"
#include "Settings/Tests/Library/Library.h"
#include "GUI/Controls/ButtonCombo.h"
#include "GUI/PageTests/Entities/Measurers.h"
#pragma warning(push, 0)
    #include <wx/dcclient.h>
#pragma warning(pop)


class PanelViewTest : public Panel
{
public:

    PanelViewTest(wxWindow *, PanelViewTest *&);

    void SetTest(Test *);

private:

    static const int radius_trans = 50;                     // От этого значения и Center() идёт всё построение изображения
    static const int d_combos = ButtonsCombo::HEIGHT + 5;   // Расстояние между элементами ввода по вертикали

    Test *test = nullptr;

    wxPaintDC *dc = nullptr;

    ButtonsCombo *bcModeScan = nullptr;                 // Режим развёртки
    ButtonsCombo *bcTypeSemiconductor = nullptr;        // npn или pnp
    ButtonsCombo *bcBaseModeControl = nullptr;          // Режим управления базой - током или напряжением
    ComboInput *bcBaseNumMeasures = nullptr;            // Количество измерений
    ButtonsCombo *bcSubstrateModeControl = nullptr;
    ButtonsCombo *bcCollectorModeSource = nullptr;      // Режим работы источника в коллекторе - высокое напряжение или большой ток

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
    void CreateElement();

    // type == "npn", "pnp"
    void CreateBJT(const wxString &type, const wxPoint &);
    void CreateBJTS(const wxString &type, const wxPoint &);

    int CalculateCombos(ComboInput **, ComboInput **, ComboInput ** = nullptr, ComboInput ** = nullptr);

    // Нарисовать значок земли
    void DrawGround(int x, int y);

    // Создать элементы управляения для данного теста
    void CreateControls();

    // Рисует линию длиной length под углом angleDeg
    void DrawLineWithAngle(const wxPoint &start, double length, double angleDeg);

    wxPoint GetCenter() const;

    Ampermeter *amptermeterBase = nullptr;
    Voltmeter *voltmeterBase = nullptr;
    SourceVoltageCurrent *sourceVoltageCurrentBase = nullptr;

    Ampermeter *ampermeterSubstrate = nullptr;
    Voltmeter *voltmeterSubstrate = nullptr;
    SourceVoltageCurrent *sourceVoltateCurrentSubstrate = nullptr;

    Ampermeter *ampermeterCollector = nullptr;
    Voltmeter *voltmeterCollector = nullptr;
    SourceVoltage *sourceVoltageCollector = nullptr;

    bool created = false;
};
