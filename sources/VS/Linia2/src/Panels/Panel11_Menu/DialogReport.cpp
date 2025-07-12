// 2025/7/12 18:36:24 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel11_Menu/DialogReport.h"


DialogReport *DialogReport::self = nullptr;


DialogReport::DialogReport() :
    MenuDialog(_("Отчёт"), 100,
        _L("Новый"), ID_BTN_MENU_REPORT_NEW, OnButtonNew,
        _L("Сохранить"), ID_BTN_MENU_REPORT_SAVE, OnButtonSave,
        _L("Удалить"), ID_BTN_MENU_REPORT_DELETE, OnButtonDelete,
        _L("Развернуть"), ID_BTN_MENU_REPORT_EXPAND, OnButtonExpand,
        _L("Свернуть"), ID_BTN_MENU_REPORT_COLLAPSE, OnButtonCollapse,
        _L("Закрыть"), ID_BTN_MENU_REPORT_CLOSE, OnButtonClose
    )
{
    self = this;
}


void DialogReport::OnButtonNew()
{

}


void DialogReport::OnButtonSave()
{

}


void DialogReport::OnButtonDelete()
{

}


void DialogReport::OnButtonExpand()
{

}


void DialogReport::OnButtonCollapse()
{

}


void DialogReport::OnButtonClose()
{

}
