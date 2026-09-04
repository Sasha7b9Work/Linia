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

    MeasurerSourcer(Type::E _type, wxPaintDC &_dc, bool vertical) : type(_type), dc(_dc), is_vertical(vertical) { }

    void Draw(const wxPoint &);

    int GetRadius() const
    {
        return radius;
    }

protected:

    Type::E type;
    wxPaintDC &dc;
    bool is_vertical;
    const int radius = 20;
    wxPoint center;
};


class Voltmeter : public MeasurerSourcer
{
public:

    Voltmeter(wxPaintDC &dc, bool vertical) : MeasurerSourcer(MeasurerSourcer::Type::MeasU, dc, vertical) { }
};


class Ampermeter : public MeasurerSourcer
{
public:

    Ampermeter(wxPaintDC &dc, bool vertical) : MeasurerSourcer(MeasurerSourcer::Type::MeasI, dc, vertical) { }
};


class SourceVoltage : public MeasurerSourcer
{
public:

    SourceVoltage(wxPaintDC &dc, bool vertical) : MeasurerSourcer(MeasurerSourcer::Type::SourceU, dc, vertical) { }
};


class SourceCurrent : public MeasurerSourcer
{
public:

    SourceCurrent(wxPaintDC &dc, bool vertical) : MeasurerSourcer(MeasurerSourcer::Type::SourceI, dc, vertical) { }
};
