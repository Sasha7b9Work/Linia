// 2025/7/12 17:38:54 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel11_Menu/DialogTable.h"


DialogTable *DialogTable::self = nullptr;


DialogTable::DialogTable() :
    MenuDialog(_L("Таблица"), 100,
        _L("Сохранить"), [](){},
        _L("Удалить"), []() {},
        _L("Развернуть"), []() {},
        _L("Свернуть"), []() {}
    )
{
    self = this;
}
