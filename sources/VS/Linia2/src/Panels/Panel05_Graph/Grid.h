// 2025/7/13 20:38:50 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class Grid
{
public:

    Grid();

    void Draw();

    void MoveOn(const wxPoint &);

private:

    wxPoint center;

    int size_cell = 40;
    int num_cells = 10;

    void DrawVPointLine(int x, int y, int d, int height);
    void DrawHPointLine(int x, int y, int d, int width);
};
