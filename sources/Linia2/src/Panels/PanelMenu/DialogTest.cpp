// 2025/7/12 15:42:53 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PanelMenu/DialogTest.h"


DialogTest *DialogTest::self = nullptr;
DialogTestAppend *DialogTestAppend::self = nullptr;
DialogTestLibrary *DialogTestLibrary::self = nullptr;


const pchar DialogTest::BTN_APPEND = "Добавить";
const pchar DialogTest::BTN_SAVE_PARAMETERS = "Сохранить параметры";
const pchar DialogTest::BTN_DELETE = "Удалить";
const pchar DialogTest::BTN_DELETE_ALL = "Удалить все";
const pchar DialogTest::BTN_COPY = "Копировать";
const pchar DialogTest::BTN_INSERT = "Вставить";
const pchar DialogTest::BTN_RENAME = "Переименовать";
const pchar DialogTest::BTN_LIBRARY = "Библиотека тестов";
const pchar DialogTest::BTN_SAVE_TO_LIBRARY = "Сохранить в библиотеке";

const pchar DialogTestAppend::BTN_ACTIVE = "Активный";
const pchar DialogTestAppend::BTN_FROM_LIBRARY = "Из библиотеки";

const pchar DialogTestLibrary::BTN_STANDARD = "Стандартных";
const pchar DialogTestLibrary::BTN_USER = "Пользовательских";


DialogTest::DialogTest() :
    MenuDialog("Тесты", 175, { 0, 1, 3, 6, 7 },
        BTN_APPEND, []()
        {
            DialogTestAppend().ShowOnWindow(DialogTest::self->FindButton(BTN_APPEND));
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
            DialogTestLibrary().ShowOnWindow(DialogTest::self->FindButton(BTN_LIBRARY));
        },
        BTN_SAVE_TO_LIBRARY, []()
        {
        }
    )
{
    self = this;

    MenuDialog::Update();
}


void DialogTest::Update()
{
    MenuDialog::Update();

    FindButton(BTN_SAVE_PARAMETERS)->Enable(false);
    FindButton(BTN_DELETE)->Enable(false);
    FindButton(BTN_DELETE_ALL)->Enable(false);
    FindButton(BTN_COPY)->Enable(false);
    FindButton(BTN_INSERT)->Enable(false);
    FindButton(BTN_RENAME)->Enable(false);
}


DialogTestAppend::DialogTestAppend() :
    MenuDialog(DialogTest::BTN_APPEND, 150, { },
        BTN_ACTIVE, []()
        {
        },
        BTN_FROM_LIBRARY, []()
        {
        }
    )
{
    self = this;
}


DialogTestLibrary::DialogTestLibrary() :
    MenuDialog(DialogTest::BTN_LIBRARY, 150, { },
        BTN_STANDARD, []()
        {
        },
        BTN_USER, []()
        {
        }
    )
{
    self = this;
}
