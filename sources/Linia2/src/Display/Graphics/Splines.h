// 2026/03/14 01:12:30 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


// Класс для интерполяции кубическими сплайнами
class CubicSpline {
private:
    std::vector<double> x;      // исходные x-координаты
    std::vector<double> y;      // исходные y-координаты
    std::vector<double> a, b, vec_c, vec_d; // коэффициенты сплайна
    bool valid;                 // флаг успешного построения

public:
    CubicSpline();

    // Инициализация сплайна по точкам
    // boundary: 0 - натуральный (вторая производная = 0), 
    //           1 - закрепленный (нужно указать производные на концах)
    bool Initialize(const std::vector<wxPoint> &points,
        int boundary = 0,
        double leftDerivative = 0,
        double rightDerivative = 0);

    bool Initialize(const std::vector<wxRealPoint> &points,
        int boundary = 0,
        double leftDerivative = 0,
        double rightDerivative = 0);

    // Вычисление значения сплайна в произвольной точке t
    double Evaluate(double t) const;

    // Получение всех точек для отрисовки с заданным шагом
    std::vector<wxPoint> GetPoints(double step = 1.0) const;

    bool IsValid() const
    {
        return valid;
    }

private:
    // Решение трехдиагональной системы методом прогонки
    bool SolveTridiagonal(const std::vector<double> &subDiagonal,
        const std::vector<double> &mainDiagonal,
        const std::vector<double> &superDiagonal,
        const std::vector<double> &rightHandSide,
        std::vector<double> &solution);
};

// Класс для отрисовки сплайнов на wxDC
class SplineRenderer {
public:
    // Отрисовка сплайна по точкам
    static void DrawSpline(wxDC &dc, const std::vector<wxPoint> &points,
        double step = 1.0, int boundary = 0);

    // Отрисовка сплайна с закраской области под кривой
    static void DrawFilledSpline(wxDC &dc, const std::vector<wxPoint> &points,
        int baselineY = 0, double step = 1.0);

    // Отрисовка сплайна с маркерами в исходных точках
    static void DrawSplineWithMarkers(wxDC &dc, const std::vector<wxPoint> &points,
        double step = 1.0);
};
