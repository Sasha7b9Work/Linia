// 2026/09/04 12:17:30 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once
#pragma warning(push, 0)
    #include <wx/dcclient.h>
#pragma warning(pop)


class Measurer
{
public:

    Measurer(wxPaintDC &_dc, int r, bool vertical) : dc(_dc), is_vertical(vertical), radius(r) { }

    virtual void Draw(const wxPoint &center) = 0;

protected:

    wxPaintDC &dc;
    wxPoint center;
    bool is_vertical;
    int radius;
};


class Voltmeter : public Measurer
{
public:

    Voltmeter(wxPaintDC &dc, int r, bool vertical) : Measurer(dc, r, vertical) { }

    virtual void Draw(const wxPoint &) override;
};


class Ampermeter : public Measurer
{
public:

    Ampermeter(wxPaintDC &dc, int r, bool vertical) : Measurer(dc, r, vertical) { }

    virtual void Draw(const wxPoint &) override;
};
