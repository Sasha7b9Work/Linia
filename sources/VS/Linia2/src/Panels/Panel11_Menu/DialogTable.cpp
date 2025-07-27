// 2025/7/12 17:38:54 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel11_Menu/DialogTable.h"


enum
{
    ID_MENU_SAVE = wxID_HIGHEST + 1,
    ID_MENU_DELETE,
    ID_MENU_EXPAND,
    ID_MENU_COLLAPSE
};


DialogTable *DialogTable::self = nullptr;


DialogTable::DialogTable() :
    MenuDialog(_L("Таблица"), 100,
        _L("Сохранить"), ID_MENU_SAVE, OnButtonSave,
        _L("Удалить"), ID_MENU_DELETE, OnButtonDelete,
        _L("Развернуть"), ID_MENU_EXPAND, OnButtonExpand,
        _L("Свернуть"), ID_MENU_COLLAPSE, OnButtonCollapse
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
