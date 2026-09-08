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

    struct Type
    {
        enum E
        {
            MeasU,
            MeasI,
            SourceU,
            SourceI,
            Count
        };
    };

    MeasurerSourcer(Type::E _type, wxPaintDC &_dc, std::vector<ComboInput *> &_parameters,
        std::vector<double> mins,
        std::vector<RowRange::E> ranges,
        std::vector<double> maxs;
        const wxPoint _center) :
        type(_type), dc(_dc), center(_center), parameters(_parameters)
    {
    }

    void Draw();

    int GetRadius() const
    {
        return radius;
    }

    static void FillValues(std::vector<std::vector<double>> &values);

protected:

    Type::E type;
    wxPaintDC &dc;
    const int radius = 12;
    wxPoint center;
    std::vector<ComboInput *> parameters;
    std::vector<std::vector<double>> values;
};


class Voltmeter : public MeasurerSourcer
{
public:

    Voltmeter(wxPaintDC &dc, std::vector<ComboInput *> _parameters, std::vector<std::vector<double>>_values, const wxPoint _center) :
        MeasurerSourcer(MeasurerSourcer::Type::MeasU, dc, _parameters, _values, _center)
    {
    }
};


class Ampermeter : public MeasurerSourcer
{
public:

    Ampermeter(wxPaintDC &dc, std::vector<ComboInput *> _parameters, std::vector<std::vector<double>>_values, const wxPoint _center) :
        MeasurerSourcer(MeasurerSourcer::Type::MeasI, dc, _parameters, _values, _center)
    {
    }
};


class SourceVoltage : public MeasurerSourcer
{
public:

    SourceVoltage(wxPaintDC &dc, std::vector<ComboInput *> _parameters, std::vector<std::vector<double>>_values, const wxPoint _center) :
        MeasurerSourcer(MeasurerSourcer::Type::SourceU, dc, _parameters, _values, _center)
    {
    }
};


class SourceCurrent : public MeasurerSourcer
{
public:

    SourceCurrent(wxPaintDC &dc, std::vector<ComboInput *> _parameters, std::vector<std::vector<double>>_values, const wxPoint _center) :
        MeasurerSourcer(MeasurerSourcer::Type::SourceI, dc, _parameters, _values, _center)
    {
    }
};
