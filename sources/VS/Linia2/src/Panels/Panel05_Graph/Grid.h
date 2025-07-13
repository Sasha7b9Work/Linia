// 2025/7/13 20:38:50 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class Grid
{
public:

    Grid();

    void Draw();

private:

    wxPoint center;

    int size_cell = 50;

    int num_cells = 10;
};
