// 2025/7/12 17:38:54 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel11_Menu/DialogTable.h"


DialogTable *DialogTable::self = nullptr;


DialogTable::DialogTable() :
    MenuDialog("Таблица", 100, { },
        BTN_SAVE, []()
        {
        },
        BTN_DELETE, []()
        {
        },
        BTN_EXPAND, []()
        {
        },
        BTN_COLLAPSE, []()
        {
        }
    )
{
    self = this;
}
