// 2026/10/08 13:10:08 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once
#include "GUI/Controls/ButtonCombo.h"


// Специфичные органы управления для разных типов элементов (тип проводимости для биполярного транзистора, например)
struct SpecificControls
{
    void Create(wxWindow *);
    void Hide();
    void Tune();

private:

    ButtonsCombo *bcTypeBJT = nullptr;                  // Тип биполярного транзистора
    ButtonsCombo *bcTypeFET = nullptr;                  // Тип полевого транзистора
    ButtonsCombo *bcCommonElectrodeDIOD = nullptr;      // Для диода - общий электрод
    ButtonsCombo *bcControlElectrodeTHYR = nullptr;     // Дли тиристора - управляющий электрод

    wxPoint Coordinate() const;
};
