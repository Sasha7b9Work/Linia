// 2026/09/04 12:17:30 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once
#include "Settings/Tests/Ranges.h"
#pragma warning(push, 0)
    #include <wx/dcclient.h>
#pragma warning(pop)


class ComboInput;


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

    MeasurerSourcer(Type::E _type, wxPaintDC &_dc, const wxPoint _center, Dir::E);

    void Draw(wxPaintDC &dc);

    int GetRadius() const
    {
        return radius;
    }

protected:

    Type::E type;
    Dir::E dir;                             // Расположение органов управления относительно УГО измерителя/источника
    const int radius = 12;
    wxPoint center;
    std::vector<ComboInput *> parametersU;
    std::vector<ComboInput *> parametersI;

private:

    // Нарисовать окантовку для измерителя или источника. x, y - центр измерителя
    // В x, y возвращаются координаты, с которых нужно выводить элементы управления
    void DrawBorder(wxPaintDC &dc, int &x, int &y, int radius, int num_controls);
};


// Измеритель напряжения
class Voltmeter : public MeasurerSourcer
{
public:

    Voltmeter(wxPaintDC &dc, const wxPoint _center, Dir::E _dir) :
        MeasurerSourcer(MeasurerSourcer::Type::MeasU, dc, _center, _dir)
    {}
};


// Имзеритель тока
class Ampermeter : public MeasurerSourcer
{
public:

    Ampermeter(wxPaintDC &dc, const wxPoint _center, Dir::E _dir) :
        MeasurerSourcer(MeasurerSourcer::Type::MeasI, dc, _center, _dir)
    {}
};


// Источник напряжения
class SourceVoltage : public MeasurerSourcer
{
public:

    SourceVoltage(wxPaintDC &dc, const wxPoint _center, Dir::E _dir) :
        MeasurerSourcer(MeasurerSourcer::Type::SourceU, dc, _center, _dir)
    {}
};


// Источник тока
class SourceCurrent : public MeasurerSourcer
{
public:

    SourceCurrent(wxPaintDC &dc, const wxPoint _center, Dir::E _dir) :
        MeasurerSourcer(MeasurerSourcer::Type::SourceI, dc, _center, _dir)
    {}
};


class SourceVoltageCurrent : public MeasurerSourcer
{
public:

    SourceVoltageCurrent(wxPaintDC &dc, const wxPoint _center, Dir::E _dir) :
        MeasurerSourcer(MeasurerSourcer::Type::SourceUI, dc, _center, _dir)
    {}
};