// 2025/7/13 20:38:50 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class GraphEntity;


struct Offset
{
    void Reset()
    {
        x = 0;
        y = 0;
    }

    // При нажимании/отпускании мышки вызываем эту функцию, чтобы обнулить накопительный счётчик смещения
    void ResetDelta()
    {
        dx = 0;
        dy = 0;
    }

    // При перемещении мышки вызываем эту функцию
    void MoveOn(const wxPoint &delta)
    {
        dx += delta.x;
        dy += delta.y;

        Process(x, dx);
        Process(y, dy);
    }

    int x = 0;
    int y = 0;

private:

    int dx = 0;
    int dy = 0;

    void Process(int &_x, int &_delta)
    {
        while (_delta >= 10)
        {
            _delta -= 10;
            _x++;
        }

        while (_delta <= -10)
        {
            _delta += 10;
            _x--;
        }
    }
};


struct Range
{
    Range(const wxString &_title, const wxString &_units, int &_offset) : title(_title), units(_units), max(_offset) { }

    wxString title;
    wxString units;

    // Разница между максимальным и минимальным значениями - амплитуда
    double Amplitude() const;

    // cells_in_axis - количество клеток по любой оси. Оно всегда одинаковое
    wxString GetValuePointAxis(int, int cells_in_axis) const;

    wxString FullTitle() const;

    void Increase();
    void Decrease();

    // Возвращает абсолютное значение - амперы, вольты
    double MaxAbs() const;

private:

    struct Value
    {
        Value(int &_offset) : offset(_offset) { }

        // Возвращает значение в абсолютных значениях - амперы, вольты
        double MaxAbs() const;
        void Increase();
        void Decrease();

    private:
        // Чему кратно значение - единице, 2, 4(5)
        enum Type
        {
            _1,
            _2,
            _4_5,
            Count
        };

        Type type = _1;
        int order = 0;
        int &offset;        // Смещение 0 относительно центра графика. Измеряется в клетках графика
    };

    Value max;
};


class Grid
{
public:

    Grid();

    void Draw(const std::vector<GraphEntity *> &);

    // Переместить изоражение на экране целиком
    void MoveImageOn(const wxPoint &);

    // Переместить центр графика (избражение измерений в графике)
    void MoveCenterOn(const wxPoint &);

    void OnMouseMove(const wxPoint &);

    void OnMouseDown();
    void OnMouseUp();

    void ScaleGridOn(const wxPoint &, int);
    void RangeGridOnX(int);
    void RangeGridOnY(int);

    // Преобразует точку графика в координаты на холсте
    wxPoint ValuesToCoord(double x, double y) const;

    wxPoint2DDouble CoordToValues(const wxPoint &) const;

    void ResetCenter();

    void Reset();

private:

    wxPoint center_about_screen;    // В этом месте относительно центра экрана находится центр сетки
    int     scale = 1;              // 2 - увеличено в два раза, 3 - увелично в три и так далее
    wxPoint pos_mouse;
    Offset  offset;
    Range   rangeX{ "Uc", "V", offset.x };
    Range   rangeY{ "Ic", "A", offset.y };
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

    // Отобразить окошко навигации (когда сетка целиком не умещается в окне)
    void DrawNavigationWindow() const;

    // Вписать в дисплей таким образом, чтобы не было полей по краям
    void FitIntoDisplay();
};
