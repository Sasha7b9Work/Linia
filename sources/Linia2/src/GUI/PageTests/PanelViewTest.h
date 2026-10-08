// 2026/04/29 16:01:42 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once
#include "GUI/Controls/Panel.h"
#include "Settings/Tests/Library/Library.h"
#include "GUI/Controls/ButtonCombo.h"
#include "GUI/PageTests/Entities/Measurers.h"
#include "GUI/PageTests/Entities/Commutator.h"
#include "GUI/PageTests/Entities/OStT/OStT.h"
#pragma warning(push, 0)
    #include <wx/dcclient.h>
#pragma warning(pop)


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

    ButtonsCombo *bcScanMode = nullptr;                 // Режим развёртки
    ButtonsCombo *bcScanNumberPoints = nullptr;         // Количество точек в одной ВАХ
    ComboInput *bcBaseNumMeasures = nullptr;            // Количество измерений
    Commutator *commutator = nullptr;                   // Управление коммутатором
    StaticText *txtCover = nullptr;                     // Индикатор состояния крышки
    bool cover_is_opened = false;
    Button *btnEditSave = nullptr;                      // Сохранить результат редактирования
    Button *btnEditExit = nullptr;                      // Выйти из режима редактирования
    StaticBox *boxScan = nullptr;                       // "Развёртка"
    StaticBox *boxCover = nullptr;                      // "Крышка"

    MeasurerVoltageCurrent *measBase = nullptr;
    SourceVoltageCurrent *srcVoltageCurrentBase = nullptr;

    MeasurerVoltageCurrent *measSubstrate = nullptr;
    SourceVoltageCurrent *srcVoltateCurrentSubstrate = nullptr;

    Ampermeter *ampCollector = nullptr;
    Voltmeter *voltCollector = nullptr;
    SourceVoltage *srcVoltageCollector = nullptr;

    ButtonsCombo *bcTypeBJT = nullptr;                  // Тип биполярного транзистора
    ButtonsCombo *bcTypeFET = nullptr;                  // Тип полевого транзистора
    ButtonsCombo *bcCommonElectrodeDIOD = nullptr;      // Для диода - общий электрод
    ButtonsCombo *bcControlElectrodeTHYR = nullptr;     // Дли тиристора - управляющий электрод

    void OnEventPaint(wxPaintEvent &);

    void OnChangedScanMode(wxCommandEvent &);
    void OnChangedScanNumberPoints(wxCommandEvent &);
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

    // Открыта крышка
    void OpenCover();

    // Закрыта крышка
    void CloseCover();

    // Нарисовать испытуемый элемент
    OStT *CreateOStT();

    int CalculateCombos(ComboInput **, ComboInput **, ComboInput ** = nullptr, ComboInput ** = nullptr);

    // Общие для всех типов элементов органы управления (тип развёртки, например)
    void CreateCommonControls();
    void ShowCommonControls();
    void HideCommonControls();

    // Специфичные органы управления для разных типов элементов (тип проводимости для биполярного транзистора, например)
    void CreateSpecificControls();
    void HideSpecificControls();
    void TuneSpecificControls();

    // Источники и измерители
    void CreateMeasurersSourcers();
    void HideMeasurersSourcers();

    void CreateButton(Button **, wxWindow *parent, const wxString &, const wxPoint &, const wxSize &, std::function<void(wxCommandEvent &)> onClick);

    wxPoint GetCenter() const;

    // Если true - находимся в режиме редактирования теста
    bool InModeEdit() const;

    void DrawScheme(AutoBufferedPaintDC &);

    // Нарисовать значок земли
    void DrawGround(AutoBufferedPaintDC &);

    // Возвращает позицию по Y источника либо измерителя
    int PosMeasurerSourcerY(int);

    // Возвращает координаты точки, из которой выходит вертикальная линия коллектора
    wxPoint CoordinateCollector() const;

    wxPoint CoordinateSpecificControl() const;
};
