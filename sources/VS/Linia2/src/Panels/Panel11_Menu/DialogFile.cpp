// 2025/7/12 10:59:56 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel11_Menu/DialogFile.h"


DialogFile::DialogFile() :
    MenuDialog(_L("Файл"),
        _L("Новый..."), ID_BTN_MENU_FILE_NEW, OnButtonNew,
        _L("Открыть..."), ID_BTN_MENU_FILE_OPEN, OnButtonOpen,
        _L("Закрыть"), ID_BTN_MENU_FILE_CLOSE, OnButtonClose,
        _L("Cохранить"), ID_BTN_MENU_FILE_SAVE, OnButtonSave,
        _L("Сохранить как..."), ID_BTN_MENU_FILE_SAVE_AS, OnButtonSaveAs,
        _L("Удалить"), ID_BTN_MENU_FILE_DELETE, OnButtonDelete
    )
{

}


void DialogFile::OnButtonNew()
{

}


void DialogFile::OnButtonOpen()
{

}


void DialogFile::OnButtonClose()
{

}


void DialogFile::OnButtonSave()
{

}


void DialogFile::OnButtonSaveAs()
{

}


void DialogFile::OnButtonDelete()
{

}
