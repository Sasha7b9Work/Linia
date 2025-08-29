// 2025/7/12 15:42:53 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel11_Menu/DialogTest.h"


DialogTest *DialogTest::self = nullptr;
DialogTestAppend *DialogTestAppend::self = nullptr;


DialogTest::DialogTest() :
    MenuDialog(_L("Тест"), 175, { 0, 1, 3, 6, 7 },
        _L("Добавить"), []()
        {
            DialogTestAppend().ShowModal();
        },
        _L("Сохранить параметры"), []() {},
        _L("Удалить"), []() {},
        _L("Удалить все"), []() {},
        _L("Копировать"), []() {},
        _L("Вставить"), []() {},
        _L("Переименовать"), []() {},
        _L("Библиотека тестов"), []() {},
        _L("Сохранить в библиотеке"), []() {}
    )
{
    self = this;
}


DialogTestAppend::DialogTestAppend() :
    MenuDialog(_L("Добавить"), 150, { },
        _L("Активный"), []() {},
        _L("Из библиотеки"), []() {}
    )
{
    self = this;
}
