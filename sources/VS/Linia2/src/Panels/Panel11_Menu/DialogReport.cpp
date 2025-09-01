// 2025/7/12 18:36:24 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel11_Menu/DialogReport.h"


DialogReport *DialogReport::self = nullptr;


DialogReport::DialogReport() :
    MenuDialog("Отчёт", 100, { },
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
