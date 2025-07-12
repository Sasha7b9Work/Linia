// 2025/7/12 16:56:22 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel11_Menu/DialogArchive.h"


DialogArchive *DialogArchive::self = nullptr;


DialogArchive::DialogArchive() :
    MenuDialog(_("Архив"), 150,
        _L("Очистить архив"), ID_BTN_MENU_ARCHIVE_CLEAR, OnButtonClear,
        _L("Удалить из архива"), ID_BTN_MENU_ARCHIVE_DELETE_FROM, OnButtonDeleteFrom,
        _L("Загрузить из архива"), ID_BTN_MENU_ARCHIVE_LOAD_FROM, OnButtonLoadFrom,
        _L("Открыть архив"), ID_BTN_MENU_ARCHIVE_OPEN, OnButtonOpen,
        _L("Закрыть архив"), ID_BTN_MENU_ARCHIVE_CLOSE, OnButtonClose
    )
{
    self = this;
}


void DialogArchive::OnButtonClear()
{

}


void DialogArchive::OnButtonDeleteFrom()
{

}


void DialogArchive::OnButtonLoadFrom()
{

}


void DialogArchive::OnButtonOpen()
{

}


void DialogArchive::OnButtonClose()
{

}
