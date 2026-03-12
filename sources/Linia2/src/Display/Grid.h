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

    // cells_in_axis - количество клеток по любой оси. Оно всегда одинаковое
    wxString GetValuePointAxis(int, int cells_in_axis) const;

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

    void OnMouseMove(const wxPoint &);

    void OnEventCnangeMeasuredElement();

    void ScaleGridOn(const wxPoint &, int);
    void ScaleGridOnX(int);
    void ScaleGridOnY(int);

    // Преобразует точку графика в координаты на холсте
    wxPoint ValuesToCoord(double x, double y) const;

    wxPoint2DDouble CoordToValues(const wxPoint &) const;

    void ResetCenter();

    void Reset();

    static Grid *self;

    uint8 *pixels = nullptr;     // Здесь производим отрисовку

    uint8 *GetPixel(int x, int y);

private:

    wxPoint center;         // В этом месте относительно центра экрана находится центр сетки
    int     scale = 1;      // 2 - увеличено в два раза, 3 - увелично в три и так далее
    wxPoint pos_mouse;
    Range   rangeX{ 1.0, "Uc", "V" };
    Range   rangeY{ 1.0, "Ic", "A" };
    const int size_cell = 60;       // Столько клетка всегда занимает на экране

    // d - расстояние между точками
    void DrawVPointLineDown(int x, int y0, int y_low, int d);
    void DrawVPointLineUp(int x, int y0, int y_hi, int d);
    void DrawHPointLineRight(int x, int y, int x_right, int d);
    void DrawHPointLineLeft(int x, int y, int x_left, int d);

    void DrawVPointLineDown2(int x, int y0, int y_low, int d);
    void DrawVPointLineUp2(int x, int y0, int y_hi, int d);
    void DrawHPointLineRight2(int x, int y, int x_right, int d);
    void DrawHPointLineLeft2(int x, int y, int x_left, int d);

    double UnitsInCellX() const;
    double UnitsInCellY() const;

    // Координаты точки оси для подписи значения
    wxPoint GetCoordPointAxisX(int) const;
    wxPoint GetCoordPointAxisY(int) const;

    int BottomY() const;            // В этой позиции экрана находится нижняя сторона экрана
    int TopY() const;               // В этой позиции экрана находится верхняя сторона экрана
    int LeftX() const;              // В этой позиции экрана находится левая сторона сетки
    int RightX() const;             // В этой позиции экрана находится правая сторона сетки

    // Длина оси в пикселях
    int LengthAxis() const;

    void DrawLabelsOnAxis() const;

    void DrawMouseMarkers() const;

    // Количество клеток по осям X и Y
    int NumCells() const;

    // Отрисовать область сетки, отображаемую на дисплее
    void DrawArea() const;

    // Вписать в дисплей таким образом, чтобы не было полей по краям
    void FitIntoDisplay();

    struct Line
    {
        Line(int _x1, int _y1, int _x2, int _y2) : x1(_x1), y1(_y1), x2(_x2), y2(_y2) { }
        void Draw() const;
        void Draw(const wxColor &) const;
    private:
        int x1, y1, x2, y2;
    };
};
