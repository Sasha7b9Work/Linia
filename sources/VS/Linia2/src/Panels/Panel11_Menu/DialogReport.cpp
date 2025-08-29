// 2025/7/12 18:36:24 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel11_Menu/DialogReport.h"


DialogReport *DialogReport::self = nullptr;


DialogReport::DialogReport() :
    MenuDialog("Отчёт", 100, { },
        "Новый", [](){},
        "Сохранить", []() {},
        "Удалить", []() {},
        "Развернуть", []() {},
        "Свернуть", []() {},
        "Закрыть", []() {}
    )
{
    self = this;
}
