// 2025/7/12 18:36:24 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel11_Menu/DialogReport.h"


DialogReport *DialogReport::self = nullptr;


DialogReport::DialogReport() :
    MenuDialog(_("Отчёт"), 100,
        _L("Новый"), ID_REPORT_MENU_NEW, [](){},
        _L("Сохранить"), ID_REPORT_MENU_SAVE, []() {},
        _L("Удалить"), ID_REPORT_MENU_DELETE, []() {},
        _L("Развернуть"), ID_REPORT_MENU_EXPAND, []() {},
        _L("Свернуть"), ID_REPORT_MENU_COLLAPSE, []() {},
        _L("Закрыть"), ID_REPORT_MENU_CLOSE, []() {}
    )
{
    self = this;
}
