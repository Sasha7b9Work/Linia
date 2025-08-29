// 2025/7/12 15:42:53 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel11_Menu/DialogTest.h"


DialogTest *DialogTest::self = nullptr;
DialogTestAppend *DialogTestAppend::self = nullptr;


DialogTest::DialogTest() :
    MenuDialog("Тест", 175, { 0, 1, 3, 6, 7 },
        "Добавить", []()
        {
            DialogTestAppend().ShowModal();
        },
        "Сохранить параметры", []() {},
        "Удалить", []() {},
        "Удалить все", []() {},
        "Копировать", []() {},
        "Вставить", []() {},
        "Переименовать", []() {},
        "Библиотека тестов", []() {},
        "Сохранить в библиотеке", []() {}
    )
{
    self = this;
}


DialogTestAppend::DialogTestAppend() :
    MenuDialog("Добавить", 150, { },
        "Активный", []() {},
        "Из библиотеки", []() {}
    )
{
    self = this;
}
