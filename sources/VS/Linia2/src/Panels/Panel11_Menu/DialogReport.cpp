// 2025/7/12 18:36:24 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel11_Menu/DialogReport.h"


DialogReport *DialogReport::self = nullptr;


DialogReport::DialogReport() :
    MenuDialog(_("Отчёт"), 100,
        _L("Новый"), [](){},
        _L("Сохранить"), []() {},
        _L("Удалить"), []() {},
        _L("Развернуть"), []() {},
        _L("Свернуть"), []() {},
        _L("Закрыть"), []() {}
    )
{
    self = this;
}
