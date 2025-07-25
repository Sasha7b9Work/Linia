// 2025/7/13 20:38:50 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class GraphEntity;


class Grid
{
public:

    Grid();

    void Draw(const std::vector<GraphEntity *> &);

    // Переместить сетку целиком
    void MoveGridOn(const wxPoint &);

    // Переместить графики внутри сетки
    void MoveMeasuresOn(const wxPoint &);

    void OnMouseMove(const wxPoint &);

    void ScaleOn(const wxPoint &, int);

    // Преобразует точку графика в координаты на холсте
    wxPoint ValuesToCoord(double x, double y) const;

    wxPoint2DDouble CoordToValues(const wxPoint &) const;

    void CalculateCenter();

private:

    wxPoint center;
    int scale = 1;      // 2 - увеличено в два раза, 3 - увелично в три и так далее

    wxPoint pos_mouse;

    void DrawVPointLine(int x, int y, int d, int height);
    void DrawHPointLine(int x, int y, int d, int width);

    double UnitsInCellX() const;
    double UnitsInCellY() const;

    // Количество клеток в осях X и Y
    int num_cells = 10;

    // Размер клетки в пикселях
    int SizeCell() const;

    // Координаты точки оси для подписи значения
    wxPoint GetCoordPointAxisX(int) const;
    wxPoint GetCoordPointAxisY(int) const;

    int BottomY() const;
    int TopY() const;
    int LeftX() const;
    int RightX() const;

    // Длина оси в пикселях
    int LengthAxis() const;

    void DrawLabelsOnAxis() const;

    void DrawMouseMarkers() const;
};
