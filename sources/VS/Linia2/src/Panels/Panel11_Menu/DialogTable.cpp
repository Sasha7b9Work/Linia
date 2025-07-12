// 2025/7/12 17:38:54 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel11_Menu/DialogTable.h"


DialogTable *DialogTable::self = nullptr;


DialogTable::DialogTable() :
    MenuDialog(_L("Таблица"), 100,
        _L("Сохранить"), ID_BTN_MENU_TABLE_SAVE, OnButtonSave,
        _L("Удалить"), ID_BTN_MENU_TABLE_DELETE, OnButtonDelete,
        _L("Развернуть"), ID_BTN_MENU_TABLE_EXPAND, OnButtonExpand,
        _L("Свернуть"), ID_BTN_MENU_TABLE_COLLAPSE, OnButtonCollapse
    )
{
    self = this;
}


void DialogTable::OnButtonSave()
{

}


void DialogTable::OnButtonDelete()
{

}


void DialogTable::OnButtonExpand()
{

}


void DialogTable::OnButtonCollapse()
{

}
