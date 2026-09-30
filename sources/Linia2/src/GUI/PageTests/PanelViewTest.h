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

    Test *test = nullptr;

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

    MeasurerVoltageCurrent *measurerBase = nullptr;
    SourceVoltageCurrent *sourceVoltageCurrentBase = nullptr;

    MeasurerVoltageCurrent *measurerSubstrate = nullptr;
    SourceVoltageCurrent *sourceVoltateCurrentSubstrate = nullptr;

    Ampermeter *ampermeterCollector = nullptr;
    Voltmeter *voltmeterCollector = nullptr;
    SourceVoltage *sourceVoltageCollector = nullptr;

    OStT *ostt = nullptr;

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

    // type == "npn", "pnp"
    // Биполярный транзистор с подложкой и без
    void CreateBJT(const wxPoint &, wxPoint &point_base, wxPoint &point_collector, wxPoint &point_substrate, wxPoint &point_emitter, AutoBufferedPaintDC &dc);

    int CalculateCombos(ComboInput **, ComboInput **, ComboInput ** = nullptr, ComboInput ** = nullptr);

    // Создать элементы управляения для данного теста
    void CreateControls();
    void ShowControls();
    void HideControls();

    void CreateButton(Button **, wxWindow *parent, const wxString &, const wxPoint &, const wxSize &, std::function<void(wxCommandEvent &)> onClick);

    wxPoint GetCenter() const;

    // Если true - находимся в режиме редактирования теста
    bool InModeEdit() const;

    void DrawScheme(AutoBufferedPaintDC &);

    // Нарисовать значок земли
    void DrawGround(AutoBufferedPaintDC &, const wxPoint &);
};
