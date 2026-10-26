// 2026/09/04 12:17:30 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once
#include "Settings/Tests/Ranges.h"
#include "GUI/Controls/ButtonCombo.h"
#include "Utils/AutoBufferedPaintDC.h"


// \todo Элемент предназначен для ввода числового значения.
class ComboInput : public ButtonsComboRange
{
public:
    ComboInput(wxWindow *parent, const wxString &title, int width,
        const wxArrayString &labels,
        const wxArrayString &tooltips,
        const wxString &name,
        int buttons_in_row = 3) :
        ButtonsComboRange(parent, title, width, labels, tooltips, name, buttons_in_row)
    {}
};


// Группа параметров - это может быть источник, измеритель или сканнер (формирователь развёртки)
class GroupParameters
{
public:

    static const int WIDTH_CONTROL = 150;

    struct Type
    {
        enum E
        {
            MeasU,      // Измеритель напряжения
            MeasI,      // Измеритель тока
            SourceU,    // Источник напряжения
            SourceI,    // Источник тока
            SourceUI,   // Источник напряжения и тока
            MeasUI,     // Измеритель напряжения и тока
            Scanner,    // Развёртка
            Count
        };

        E value;

        bool IsMeasU() const
        {
            return value == MeasU;
        }

        bool IsMeasI() const
        {
            return value == MeasI;
        }

        bool IsMeasUandI() const
        {
            return value == MeasUI;
        }

        bool IsSourceU() const
        {
            return value == SourceU;
        }

        bool IsSourceI() const
        {
            return value == SourceI;
        }

        bool IsSourceUandI() const
        {
            return value == SourceUI;
        }

        bool IsScanner() const
        {
            return value == Scanner;
        }
    };

    GroupParameters(Type::E, Chan::E, Dir::E);

    void Draw(AutoBufferedPaintDC &);

    int GetRadius() const
    {
        return radius;
    }

    void Draw(AutoBufferedPaintDC &dc, const wxPoint &pos)
    {
        if (btnDisable)
        {
            btnDisable->Show();
        }

        if (is_enabled)
        {
            if (btnModeUI)
            {
                btnModeUI->Show();
            }
        }

        SetPosition(pos);

        is_showing = true;

        Draw(dc);
    }

    void Hide()
    {
        is_showing = false;

        if (btnDisable)
        {
            btnDisable->Hide();
        }

        if (btnModeUI)
        {
            btnModeUI->Hide();
        }

        for (auto wnd : parametersU)
        {
            wnd->Hide();
        }

        for (auto wnd : parametersI)
        {
            wnd->Hide();
        }
    }

    void SetPosition(const wxPoint &pos)
    {
        center = pos;
    }

    // Показать параметры в соотвествии с выбранным режимом - така или напряжения
    void ShowNeedParameters();

    template<class T>
    T *FindByName(const wxString &);
    wxWindow *FindWindowByName(const wxString &);

protected:

    Type type;
    Chan chan;
    Dir::E dir;                                 // Расположение органов управления относительно УГО измерителя/источника
    const int radius = 12;
    wxPoint center{ 0, 0 };
    std::vector<wxWindow *> parametersU;
    std::vector<wxWindow *> parametersI;
    std::vector<wxWindow *> parametersScanner;
    Button *btnDisable = nullptr;               // Кнопка отлючения измерителя/источника
    bool is_enabled = true;
    bool is_showing = false;
    Button *btnModeUI = nullptr;                // В измерителе переключение между вольтметром и амперметров, в источнике - между источником тока и источником напряжения
    wxPoint coord_controls;                     // Координаты комбобоксов

    // Нужно вызывать при изменении положения переключателя коммутатора
    virtual void OnEventChangeCommutator() { }

    virtual void OnEventChangeScanner() { }

private:

    // Нарисовать окантовку для измерителя или источника. x, y - центр измерителя
    // В x, y возвращаются координаты, с которых нужно выводить элементы управления
    // Возвращает прямоугльник окантовки
    wxRect DrawBorder(AutoBufferedPaintDC &, int radius);

    void CreateControls(const wxRect &);

    void CreateButtonDisable(const wxRect &, const wxSize &, wxPoint &pos);

    void CreateButtonModeUI(const wxRect &, const wxSize &, const wxPoint &pos);

    // Создаёт токовые параметры
    void CreateParametersI();
    void CreateParametersBaseI();
    void CreateParametersSubstrateI();
    void CreateParametersCollectorI();

    // Создаёт напряжённые параметры
    void CreateParametersU();
    void CreateParametersBaseU();
    void CreateParametersSubstrateU();
    void CreateParametersCollectorU();

    void CreateParametersScanner();

    // Возвращает true, если выбран режим источника или измерителя напряжения
    bool IsSetModeU() const;

    void DrawUGO(AutoBufferedPaintDC &);

    wxString SymbolUGO();

    int CalculateNumControls() const;

    void OnChangedScanMode(wxCommandEvent &);
    void OnChangedScanNumberPoints(wxCommandEvent &);
    void OnChangedScanDurationImpulse(wxCommandEvent &);
};


// Измеритель напряжения
class Voltmeter : public GroupParameters
{
public:

    Voltmeter(Chan::E _chan, Dir::E _dir) :
        GroupParameters(GroupParameters::Type::MeasU, _chan, _dir)
    {}
};


// Имзеритель тока
class Ampermeter : public GroupParameters
{
public:

    Ampermeter(Chan::E _chan, Dir::E _dir) :
        GroupParameters(GroupParameters::Type::MeasI, _chan, _dir)
    {}

    // Нужно вызывать при изменении положения переключателя коммутатора
    virtual void OnEventChangeCommutator() override;

    virtual void OnEventChangeScanner() override;

private:

    void FillChoices(wxArrayString &choices, RangeI::E begin, RangeI::E end, RowRange::E);

    // Рассчитывает минимальный и максимальный ток, измеряемый в коллекторе, для текущих установленных настроек
    std::pair<RangeI::E, RangeI::E> GetMinMaxCurrentMeasCollector();
};


// Измеритель и тока и напряжения
class MeasurerVoltageCurrent : public GroupParameters
{
public:

    MeasurerVoltageCurrent(Chan::E _chan, Dir::E dir) :
        GroupParameters(GroupParameters::Type::MeasUI, _chan, dir)
    {}
};


// Источник напряжения
class SourceVoltage : public GroupParameters
{
public:

    SourceVoltage(Chan::E _chan, Dir::E _dir) :
        GroupParameters(GroupParameters::Type::SourceU, _chan, _dir)
    {}
};


// Источник тока
class SourceCurrent : public GroupParameters
{
public:

    SourceCurrent(Chan::E _chan, Dir::E _dir) :
        GroupParameters(GroupParameters::Type::SourceI, _chan, _dir)
    {}
};


class SourceVoltageCurrent : public GroupParameters
{
public:

    SourceVoltageCurrent(Chan::E _chan, Dir::E _dir) :
        GroupParameters(GroupParameters::Type::SourceUI, _chan, _dir)
    {}
};


// Настройки развёрток
class Scanner : public GroupParameters
{
public:

    Scanner(Chan::E _chan, Dir::E _dir) : GroupParameters(GroupParameters::Type::Scanner, _chan, _dir) { }

    // Нужно вызывать при изменении положения переключателя коммутатора
    virtual void OnEventChangeCommutator() override;
};
