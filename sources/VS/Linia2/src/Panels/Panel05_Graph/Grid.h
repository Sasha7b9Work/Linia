// 2025/7/13 20:38:50 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class Grid
{
public:

    Grid();

    void Draw();

    void MoveOn(const wxPoint &);

    void ScaleOn(const wxPoint &, int);

private:

    wxPoint center;
    int scale = 1;      // 2 - увеличено в два раза, 3 - увелично в три и так далее
    int num_cells = 10;

    wxString titleX{ "Uc" };
    wxString titleY{ "Ic" };

    wxString unitsX{ "V" };
    wxString unitsY{ "A" };

    double rangeX = 5.0;        // Размах в одну сторону от нуля
    double rangeY = 1.0;        // Размах в одну сторону от нуля

    void DrawVPointLine(int x, int y, int d, int height);
    void DrawHPointLine(int x, int y, int d, int width);

    int SizeCell() const;

    // Координаты точки оси x для подписи значения
    wxPoint GetCoordPointAxisX(int) const;
    wxPoint GetCoordPointAxisY(int) const;

    wxString GetValuePointAxisX(int) const;
    wxString GetValuePointAxisY(int) const;

    int BottomY() const;
    int TopY() const;
    int LeftX() const;
    int RightX() const;

    int LengthAxis() const;

    void DrawLabelsOnAxis() const;

    wxString FullTitleX() const;
};
