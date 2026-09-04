// 2026/09/04 10:11:28 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once
#pragma warning(push, 0)
    #include <wx/dcclient.h>
#pragma warning(pop)


class LineDriwer
{
public:

    LineDriwer(wxPaintDC &, int x, int y);

    void MoveTo(int x, int y);

    void MoveOnDY(int dy)
    {
        coord.y += dy;
    }

    // Рисует линию вверх или вниз на величину dy
    int LineOnDY(int dy);

    int LineToY(int y);

    int LineOnDX(int dx);

    int GetX() const
    {
        return coord.x;
    }

    int GetY() const
    {
        return coord.y;
    }

    wxPoint GetCoord() const
    {
        return coord;
    }

private:

    wxPaintDC &dc;
    wxPoint coord;
};
