// 2026/09/04 12:17:30 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once
#pragma warning(push, 0)
    #include <wx/dcclient.h>
#pragma warning(pop)


// Общий класс для источника и измерителя
class MeasurerSourcer
{
public:

    MeasurerSourcer(wxPaintDC &_dc, bool vertical, const wxString &_label) : label(_label), dc(_dc), is_vertical(vertical) { }

    void Draw(const wxPoint &);

    int GetRadius() const
    {
        return radius;
    }

protected:

    wxString label;
    wxPaintDC &dc;
    bool is_vertical;
    const int radius = 20;
    wxPoint center;
};


class Voltmeter : public MeasurerSourcer
{
public:

    Voltmeter(wxPaintDC &dc, bool vertical) : MeasurerSourcer(dc, vertical, "V") { }
};


class Ampermeter : public MeasurerSourcer
{
public:

    Ampermeter(wxPaintDC &dc, bool vertical) : MeasurerSourcer(dc, vertical, "A") { }
};


class SourceVoltage : public MeasurerSourcer
{
public:

    SourceVoltage(wxPaintDC &dc, bool vertical) : MeasurerSourcer(dc, vertical, "E") { }
};


class SourceCurrent : public MeasurerSourcer
{
public:

    SourceCurrent(wxPaintDC &dc, bool vertical) : MeasurerSourcer(dc, vertical, "J") { }
};
