// 2026/09/04 12:17:30 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once
#include "Settings/Tests/Ranges.h"
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
            Count
        };
    };

    MeasurerSourcer(Type::E _type, const wxPoint _center, Dir::E);

    void Draw(wxPaintDC &dc);

    int GetRadius() const
    {
        return radius;
    }

protected:

    Type::E type;
    Dir::E dir;                                 // Расположение органов управления относительно УГО измерителя/источника
    const int radius = 12;
    wxPoint center;
    std::vector<ComboInput *> parametersU;
    std::vector<ComboInput *> parametersI;
    Button *btnDisable = nullptr;               // Кнопка отлючения измерителя/источника

private:

    // Нарисовать окантовку для измерителя или источника. x, y - центр измерителя
    // В x, y возвращаются координаты, с которых нужно выводить элементы управления
    // Возвращает прямоугльник окантовки
    wxRect DrawBorder(wxPaintDC &dc, int &x, int &y, int radius, int num_controls);
};


// Измеритель напряжения
class Voltmeter : public MeasurerSourcer
{
public:

    Voltmeter(const wxPoint _center, Dir::E _dir) :
        MeasurerSourcer(MeasurerSourcer::Type::MeasU, _center, _dir)
    {}
};


// Имзеритель тока
class Ampermeter : public MeasurerSourcer
{
public:

    Ampermeter(const wxPoint _center, Dir::E _dir) :
        MeasurerSourcer(MeasurerSourcer::Type::MeasI, _center, _dir)
    {}
};


// Источник напряжения
class SourceVoltage : public MeasurerSourcer
{
public:

    SourceVoltage(const wxPoint _center, Dir::E _dir) :
        MeasurerSourcer(MeasurerSourcer::Type::SourceU, _center, _dir)
    {}
};


// Источник тока
class SourceCurrent : public MeasurerSourcer
{
public:

    SourceCurrent(const wxPoint _center, Dir::E _dir) :
        MeasurerSourcer(MeasurerSourcer::Type::SourceI, _center, _dir)
    {}
};


class SourceVoltageCurrent : public MeasurerSourcer
{
public:

    SourceVoltageCurrent(const wxPoint _center, Dir::E _dir) :
        MeasurerSourcer(MeasurerSourcer::Type::SourceUI, _center, _dir)
    {}
};