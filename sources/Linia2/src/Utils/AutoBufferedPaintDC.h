// 2026/09/30 15:11:13 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once
#pragma warning(push, 0)
    #include <wx/dcbuffer.h>
#pragma warning(pop)


class AutoBufferedPaintDC : public wxAutoBufferedPaintDC
{
public:

    AutoBufferedPaintDC(wxWindow *wnd) : wxAutoBufferedPaintDC(wnd) { }

    void MoveTo(const wxPoint &);

    void MoveToY(int);

    void MoveOnDY(int);

    void LineTo(const wxPoint &);

    void LineToX(int);

    void LineOnDY(int);

    void LineOnDX(int);

    void LineOn(const wxPoint &);

    wxPoint GetCoord() const;

private:

    wxPoint coord{ 0, 0 };
};
