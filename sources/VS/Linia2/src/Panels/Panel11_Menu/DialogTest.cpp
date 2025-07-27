// 2025/7/12 15:42:53 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel11_Menu/DialogTest.h"


enum
{
    ID_BTN_MENU_TEST_APPEND = wxID_HIGHEST + 1,
    ID_BTN_MENU_TEST_APPEND_ACTIVE,
    ID_BTN_MENU_TEST_APPEND_FROM_LIBRARY,
    ID_BTN_MENU_TEST_SAVE_PARAMETERS,
    ID_BTN_MENU_TEST_DELETE,
    ID_BTN_MENU_TEST_DELETE_ALL,
    ID_BTN_MENU_TEST_COPY,
    ID_BTN_MENU_TEST_INSERT,
    ID_BTN_MENU_TEST_RENAME,
    ID_BTN_MENU_TEST_LIBRARY_TESTS,
    ID_BTN_MENU_TEST_SAVE_TO_LIBRARY
};


DialogTest *DialogTest::self = nullptr;
DialogTestAppend *DialogTestAppend::self = nullptr;


DialogTest::DialogTest() :
    MenuDialog(_L("Тест"), 175,
        _L("Добавить"), ID_BTN_MENU_TEST_APPEND, []()
        {
            DialogTestAppend().ShowModal();
        },
        _L("Сохранить параметры"), ID_BTN_MENU_TEST_SAVE_PARAMETERS, []() {},
        _L("Удалить"), ID_BTN_MENU_TEST_DELETE, []() {},
        _L("Удалить все"), ID_BTN_MENU_TEST_DELETE_ALL, []() {},
        _L("Копировать"), ID_BTN_MENU_TEST_COPY, []() {},
        _L("Вставить"), ID_BTN_MENU_TEST_INSERT, []() {},
        _L("Переименовать"), ID_BTN_MENU_TEST_RENAME, []() {},
        _L("Библиотека тестов"), ID_BTN_MENU_TEST_LIBRARY_TESTS, []() {},
        _L("Сохранить в библиотеке"), ID_BTN_MENU_TEST_SAVE_TO_LIBRARY, []() {}
    )
{
    self = this;
}


DialogTestAppend::DialogTestAppend() :
    MenuDialog(_L("Добавить"), 150,
        _L("Активный"), ID_BTN_MENU_TEST_APPEND_ACTIVE, []() {},
        _L("Из библиотеки"), ID_BTN_MENU_TEST_APPEND_FROM_LIBRARY, []() {}
    )
{
    self = this;
}
