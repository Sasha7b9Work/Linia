// 2025/7/25 10:02:28 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls/Dialog.h"


class WindowScale : public Dialog
{
public:

    static const int WIDTH = 500;
    static const int HEIGHT = 500;

    WindowScale();

    struct Range
    {
        double min;
        double max;

        // Возвращает максимальное отклонение от нуля в минимальную или максимальную сторону
        double MaxAbs() const;

        // Разница между максимальным и минимальным значениями - амплитуда
        double Amplitude() const;
    };

    static Range rangeX;
    static Range rangeY;

    static wxString titleX;
    static wxString unitsX;

    static wxString FullTitleX();

    static wxString GetValuePointAxisX(int);

    static wxString GetValuePointAxisY(int);

private:

    wxStaticBox *CreateBox(int x, int y, int w, int h, pchar axe, pchar units, int id_min, int id_max);
};
