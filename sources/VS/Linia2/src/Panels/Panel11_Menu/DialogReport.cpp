// 2025/7/12 18:36:24 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel11_Menu/DialogReport.h"


enum
{
    ID_MENU_NEW = wxID_HIGHEST + 1,
    ID_MENU_DELETE,
    ID_MENU_EXPAND,
    ID_MENU_COLLAPSE,
    ID_MENU_CLOSE
};


DialogReport *DialogReport::self = nullptr;


DialogReport::DialogReport() :
    MenuDialog(_("Отчёт"), 100,
        _L("Новый"), ID_MENU_NEW, [](){},
        _L("Сохранить"), wxID_SAVE, []() {},
        _L("Удалить"), ID_MENU_DELETE, []() {},
        _L("Развернуть"), ID_MENU_EXPAND, []() {},
        _L("Свернуть"), ID_MENU_COLLAPSE, []() {},
        _L("Закрыть"), ID_MENU_CLOSE, []() {}
    )
{
    self = this;
}
