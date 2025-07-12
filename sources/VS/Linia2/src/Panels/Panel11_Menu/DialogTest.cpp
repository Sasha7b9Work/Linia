// 2025/7/12 15:42:53 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel11_Menu/DialogTest.h"


DialogTest *DialogTest::self = nullptr;
DialogTestAppend *DialogTestAppend::self = nullptr;


DialogTest::DialogTest() :
    MenuDialog(_L("Тест"), 175,
        _L("Добавить"), ID_BTN_MENU_TEST_APPEND, OnButtonAppend,
        _L("Сохранить параметры"), ID_BTN_MENU_TEST_SAVE_PARAMETERS, OnButtonSaveParameters,
        _L("Удалить"), ID_BTN_MENU_TEST_DELETE, OnButtonDelete ,
        _L("Удалить все"), ID_BTN_MENU_TEST_DELETE_ALL, OnButtonDeleteAll,
        _L("Копировать"), ID_BTN_MENU_TEST_COPY, OnButtonCopy,
        _L("Вставить"), ID_BTN_MENU_TEST_INSERT, OnButtonInsert,
        _L("Переименовать"), ID_BTN_MENU_TEST_RENAME, OnButtonRename,
        _L("Библиотека тестов"), ID_BTN_MENU_TEST_LIBRARY_TESTS, OnButtonLibraryTests,
        _L("Сохранить в библиотеке"), ID_BTN_MENU_TEST_SAVE_TO_LIBRARY, OnButtonSaveToLibrary
    )
{
    self = this;
}


void DialogTest::OnButtonAppend()
{
    DialogTestAppend().ShowModal();
}


void DialogTest::OnButtonSaveParameters()
{

}


void DialogTest::OnButtonDelete()
{

}


void DialogTest::OnButtonDeleteAll()
{

}


void DialogTest::OnButtonCopy()
{

}


void DialogTest::OnButtonInsert()
{

}


void DialogTest::OnButtonRename()
{

}


void DialogTest::OnButtonLibraryTests()
{

}


void DialogTest::OnButtonSaveToLibrary()
{

}


DialogTestAppend::DialogTestAppend() :
    MenuDialog(_L("Добавить"), 150,
        _L("Активный"), ID_BTN_MENU_TEST_APPEND_ACTIVE, OnButtonActive,
        _L("Из библиотеки"), ID_BTN_MENU_TEST_APPEND_FROM_LIBRARY, OnButtonFromLibrary
    )
{
    self = this;
}


void DialogTestAppend::OnButtonActive()
{

}


void DialogTestAppend::OnButtonFromLibrary()
{

}
