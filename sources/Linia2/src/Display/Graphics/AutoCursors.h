// 2026/3/15 19:48:24 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Display/Graphics/GraphMeasure.h"


namespace AutoCursors
{
    void Draw(const std::vector<GraphMeasure *> &);

    // Открыто какое-то окно поверху. Не нужно отображать курсоры
    void Ban();

    // Нету открытых окон. Нужно отображать курсоры
    void Allow();
}
