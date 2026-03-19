// 2025/7/12 17:38:54 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PanelMenu/DialogTable.h"
#include "Panels/PanelTable.h"


DialogTable *DialogTable::self = nullptr;


const pchar DialogTable::BTN_SAVE = _("Сохранить");
const pchar DialogTable::BTN_DELETE = _("Удалить");
const pchar DialogTable::BTN_EXPAND = _("Показать");
const pchar DialogTable::BTN_COLLAPSE = _("Скрыть");


DialogTable::DialogTable() :
    MenuDialog(_("Таблица"), 100, { },
        BTN_SAVE, []()
        {
        },
        BTN_DELETE, []()
        {
        },
        BTN_EXPAND, []()
        {
            PanelTable::self->Show();

            DialogTable::self->Close();
        },
        BTN_COLLAPSE, []()
        {
            PanelTable::self->Hide();

            DialogTable::self->Close();
        }
    )
{
    self = this;

    FindButton(BTN_EXPAND)->Enable(!PanelTable::self->IsShown());

    FindButton(BTN_COLLAPSE)->Enable(PanelTable::self->IsShown());
}
