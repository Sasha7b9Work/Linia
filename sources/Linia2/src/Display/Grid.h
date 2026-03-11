// 2025/7/13 20:38:50 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class GraphEntity;


struct Range
{
    double max;                 // Размах от нуля. Т.е. полный размах будет [-max, +max]

    wxString title;
    wxString units;

    // Разница между максимальным и минимальным значениями - амплитуда
    double Amplitude() const;

    wxString GetValuePointAxis(int) const;

    wxString FullTitle() const;

    void operator+=(const double &);
    void operator*=(const double &);
};


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

    void OnEventCnangeMeasuredElement();

    void ScaleGridOn(const wxPoint &, int);
    void ScaleGridOnX(int);
    void ScaleGridOnY(int);

    // Преобразует точку графика в координаты на холсте
    wxPoint ValuesToCoord(double x, double y) const;

    wxPoint2DDouble CoordToValues(const wxPoint &) const;

    void CalculateCenter();

    void Reset();

private:

    wxPoint center;         // В этом месте относительно центра экрана находится центр сетки
    int     scale = 1;      // 2 - увеличено в два раза, 3 - увелично в три и так далее
    wxPoint pos_mouse;
    Range   rangeX{ 20, "Uc", "V" };
    Range   rangeY{ 5, "Ic", "A" };
    // Количество клеток в осях X и Y
    int num_cells = 10;

    // d - расстояние между точками
    void DrawVPointLineDown(int x, int y, int d, int height);
    void DrawVPointLineUp(int x, int y, int d, int height);
    void DrawHPointLineRight(int x, int y, int d, int width);
    void DrawHPointLineLeft(int x, int y, int d, int width);

    void DrawVPointLineDown2(int x, int y, int d, int height);
    void DrawVPointLineUp2(int x, int y, int d, int height);
    void DrawHPointLineRight2(int x, int y, int d, int width);
    void DrawHPointLineLeft2(int x, int y, int d, int width);

    double UnitsInCellX() const;
    double UnitsInCellY() const;

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
