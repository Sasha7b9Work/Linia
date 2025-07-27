// 2025/7/12 15:43:24 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Panels/Panel11_Menu/MenuDialog.h"


class DialogTest : public MenuDialog
{
public:

    DialogTest();

    static DialogTest *self;
};


class DialogTestAppend : public MenuDialog
{
public:

    DialogTestAppend();

    static DialogTestAppend *self;
};
