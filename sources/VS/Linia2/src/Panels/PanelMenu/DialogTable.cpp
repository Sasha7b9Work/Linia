// 2025/7/12 17:38:54 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PanelMenu/DialogTable.h"


DialogTable *DialogTable::self = nullptr;


const pchar DialogTable::BTN_SAVE = "Сохранить";
const pchar DialogTable::BTN_DELETE = "Удалить";
const pchar DialogTable::BTN_EXPAND = "Развернуть";
const pchar DialogTable::BTN_COLLAPSE = "Свернуть";


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
