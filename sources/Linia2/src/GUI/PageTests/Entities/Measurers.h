// 2026/09/04 12:17:30 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once
#include "Settings/Tests/Ranges.h"
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
    {}
};


// Общий класс для источника и измерителя
class MeasurerSourcer
{
public:

    static const int WIDTH_CONTROL = 120;

    struct Type
    {
        enum E
        {
            MeasU,
            MeasI,
            SourceU,
            SourceI,
            SourceUI,
            MeasUI,
            Count
        };
    };

    MeasurerSourcer(Type::E, Chan::E, const wxPoint _center, Dir::E);

    void Draw(wxPaintDC &dc);

    int GetRadius() const
    {
        return radius;
    }

protected:

    Type::E type;
    Chan::E chan;
    Dir::E dir;                                 // Расположение органов управления относительно УГО измерителя/источника
    const int radius = 12;
    wxPoint center;
    std::vector<ComboInput *> parametersU;
    std::vector<ComboInput *> parametersI;
    Button *btnDisable = nullptr;               // Кнопка отлючения измерителя/источника
    bool disabled = false;
    Button *btnModeSourceUI = nullptr;          // В каком режиме находится - источник тока или источник напряжения

private:

    // Нарисовать окантовку для измерителя или источника. x, y - центр измерителя
    // В x, y возвращаются координаты, с которых нужно выводить элементы управления
    // Возвращает прямоугльник окантовки
    wxRect DrawBorder(wxPaintDC &dc, int &x, int &y, int radius, int num_controls);

    void CreateControls(const wxRect &rect, int x, int y);

    void CreateButtonDisable(const wxRect &rect, const wxSize &size, wxPoint &pos);

    void CreateButtonModeSourceUI(const wxSize &size, const wxPoint &pos);
};


// Измеритель напряжения
class Voltmeter : public MeasurerSourcer
{
public:

    Voltmeter(Chan::E _chan, const wxPoint _center, Dir::E _dir) :
        MeasurerSourcer(MeasurerSourcer::Type::MeasU, _chan, _center, _dir)
    {}
};


// Имзеритель тока
class Ampermeter : public MeasurerSourcer
{
public:

    Ampermeter(Chan::E _chan, const wxPoint _center, Dir::E _dir) :
        MeasurerSourcer(MeasurerSourcer::Type::MeasI, _chan, _center, _dir)
    {}
};


// Измеритель и тока и напряжения
class MeasurerVoltageCurrent : public MeasurerSourcer
{
public:

    MeasurerVoltageCurrent(Chan::E _chan, const wxPoint _center, Dir::E dir) :
        MeasurerSourcer(MeasurerSourcer::Type::MeasUI, _chan, _center, dir)
    {}
};


// Источник напряжения
class SourceVoltage : public MeasurerSourcer
{
public:

    SourceVoltage(Chan::E _chan, const wxPoint _center, Dir::E _dir) :
        MeasurerSourcer(MeasurerSourcer::Type::SourceU, _chan, _center, _dir)
    {}
};


// Источник тока
class SourceCurrent : public MeasurerSourcer
{
public:

    SourceCurrent(Chan::E _chan, const wxPoint _center, Dir::E _dir) :
        MeasurerSourcer(MeasurerSourcer::Type::SourceI, _chan, _center, _dir)
    {}
};


class SourceVoltageCurrent : public MeasurerSourcer
{
public:

    SourceVoltageCurrent(Chan::E _chan, const wxPoint _center, Dir::E _dir) :
        MeasurerSourcer(MeasurerSourcer::Type::SourceUI, _chan, _center, _dir)
    {}
};