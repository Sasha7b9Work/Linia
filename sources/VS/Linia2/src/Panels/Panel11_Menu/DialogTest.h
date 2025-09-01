// 2025/7/12 15:43:24 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Panels/Panel11_Menu/MenuDialog.h"


class DialogTest : public MenuDialog
{
public:

    DialogTest();

    static DialogTest *self;

private:

    const pchar BTN_APPEND = "Добавить";
    const pchar BTN_SAVE_PARAMETERS = "Сохранить параметры";
    const pchar BTN_DELETE = "Удалить";
    const pchar BTN_DELETE_ALL = "Удалить все";
    const pchar BTN_COPY = "Копировать";
    const pchar BTN_INSERT = "Вставить";
    const pchar BTN_RENAME = "Переименовать";
    const pchar BTN_LIBRARY = "Библиотека тестов";
    const pchar BTN_SAVE_TO_LIBRARY = "Сохранить в библиотеке";
};


class DialogTestAppend : public MenuDialog
{
public:

    DialogTestAppend();

    static DialogTestAppend *self;

private:

    const pchar BTN_ACTIVE = "Активный";
    const pchar BTN_FROM_LIBRARY = "Из библиотеки";
};
