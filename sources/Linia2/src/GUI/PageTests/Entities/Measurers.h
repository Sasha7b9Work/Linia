// 2026/09/04 12:17:30 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once
#pragma warning(push, 0)
    #include <wx/dcclient.h>
#pragma warning(pop)


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

    MeasurerSourcer(Type::E _type, wxPaintDC &_dc) : type(_type), dc(_dc) { }

    void Draw(const wxPoint &);

    int GetRadius() const
    {
        return radius;
    }

protected:

    Type::E type;
    wxPaintDC &dc;
    const int radius = 12;
    wxPoint center;
};


class Voltmeter : public MeasurerSourcer
{
public:

    Voltmeter(wxPaintDC &dc) : MeasurerSourcer(MeasurerSourcer::Type::MeasU, dc) { }
};


class Ampermeter : public MeasurerSourcer
{
public:

    Ampermeter(wxPaintDC &dc) : MeasurerSourcer(MeasurerSourcer::Type::MeasI, dc) { }
};


class SourceVoltage : public MeasurerSourcer
{
public:

    SourceVoltage(wxPaintDC &dc) : MeasurerSourcer(MeasurerSourcer::Type::SourceU, dc) { }
};


class SourceCurrent : public MeasurerSourcer
{
public:

    SourceCurrent(wxPaintDC &dc) : MeasurerSourcer(MeasurerSourcer::Type::SourceI, dc) { }
};
