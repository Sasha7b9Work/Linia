// 2025/7/12 18:36:24 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PanelMenu/DialogReport.h"


DialogReport *DialogReport::self = nullptr;


const pchar DialogReport::BTN_NEW = "Новый";
const pchar DialogReport::BTN_SAVE = "Сохранить";
const pchar DialogReport::BTN_DELETE = "Удалить";
const pchar DialogReport::BTN_EXPAND = "Развернуть";
const pchar DialogReport::BTN_COLLAPSE = "Свернуть";
const pchar DialogReport::BTN_CLOSE = "Закрыть";


DialogReport::DialogReport() :
    MenuDialog(_("Отчёт"), 100, { },
        BTN_NEW, []()
        {
        },
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
        },
        BTN_CLOSE, []()
        {
        }
    )
{
    self = this;
}
