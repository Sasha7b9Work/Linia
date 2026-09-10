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
            SourceUI,
            Count
        };
    };

    MeasurerSourcer(Type::E _type, wxPaintDC &_dc,
        std::vector<ComboInput **> *_parametersU, std::vector<ComboInput **> *_parametersI, const wxPoint _center) :
        type(_type), dc(_dc), center(_center), parametersU(_parametersU), parametersI(_parametersI)
    {}

    void Draw();

    int GetRadius() const
    {
        return radius;
    }

protected:

    Type::E type;
    wxPaintDC &dc;
    const int radius = 12;
    wxPoint center;
    std::vector<ComboInput **> *parametersU;
    std::vector<ComboInput **> *parametersI;
};


// Измеритель напряжения
class Voltmeter : public MeasurerSourcer
{
public:

    Voltmeter(wxPaintDC &dc, std::vector<ComboInput **> *_parameters, const wxPoint _center) :
        MeasurerSourcer(MeasurerSourcer::Type::MeasU, dc, _parameters, nullptr, _center)
    {}
};


// Имзеритель тока
class Ampermeter : public MeasurerSourcer
{
public:

    Ampermeter(wxPaintDC &dc, std::vector<ComboInput **> *_parameters, const wxPoint _center) :
        MeasurerSourcer(MeasurerSourcer::Type::MeasI, dc, nullptr, _parameters, _center)
    {}
};


// Источник напряжения
class SourceVoltage : public MeasurerSourcer
{
public:

    SourceVoltage(wxPaintDC &dc, std::vector<ComboInput **> *_parameters, const wxPoint _center) :
        MeasurerSourcer(MeasurerSourcer::Type::SourceU, dc, _parameters, nullptr, _center)
    {}
};


// Источник тока
class SourceCurrent : public MeasurerSourcer
{
public:

    SourceCurrent(wxPaintDC &dc, std::vector<ComboInput **> *_parameters, const wxPoint _center) :
        MeasurerSourcer(MeasurerSourcer::Type::SourceI, dc, nullptr, _parameters, _center)
    {}
};


class SourceVoltageCurrent : public MeasurerSourcer
{
public:

    SourceVoltageCurrent(wxPaintDC &dc, std::vector<ComboInput **> *_parametersU, std::vector<ComboInput **> *_parametersI, const wxPoint _center) :
        MeasurerSourcer(MeasurerSourcer::Type::SourceUI, dc, _parametersU, _parametersI, _center)
    {}
};