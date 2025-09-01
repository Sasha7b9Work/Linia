// 2025/7/12 15:42:53 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel11_Menu/DialogTest.h"


DialogTest *DialogTest::self = nullptr;
DialogTestAppend *DialogTestAppend::self = nullptr;


DialogTest::DialogTest() :
    MenuDialog("Тест", 175, { 0, 1, 3, 6, 7 },
        BTN_APPEND, []()
        {
            DialogTestAppend().ShowModal();
        },
        BTN_SAVE_PARAMETERS, []()
        {
        },
        BTN_DELETE, []()
        {
        },
        BTN_DELETE_ALL, []()
        {
        },
        BTN_COPY, []()
        {
        },
        BTN_INSERT, []()
        {
        },
        BTN_RENAME, []()
        {
        },
        BTN_LIBRARY, []()
        {
        },
        BTN_SAVE_TO_LIBRARY, []()
        {
        }
    )
{
    self = this;

//    FindButton()
}


DialogTestAppend::DialogTestAppend() :
    MenuDialog("Добавить", 150, { },
        BTN_ACTIVE, []()
        {
        },
        BTN_FROM_LIBRARY, []()
        {
        }
    )
{
    self = this;

//    FindButton()
}
