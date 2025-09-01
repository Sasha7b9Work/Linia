// 2025/7/12 15:43:24 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Panels/Panel11_Menu/MenuDialog.h"


class DialogTest : public MenuDialog
{
public:

    DialogTest();

    static DialogTest *self;

    virtual void Update() override;

private:

    static const pchar BTN_APPEND;
    static const pchar BTN_SAVE_PARAMETERS;
    static const pchar BTN_DELETE;
    static const pchar BTN_DELETE_ALL;
    static const pchar BTN_COPY;
    static const pchar BTN_INSERT;
    static const pchar BTN_RENAME;
    static const pchar BTN_LIBRARY;
    static const pchar BTN_SAVE_TO_LIBRARY;
};


class DialogTestAppend : public MenuDialog
{
public:

    DialogTestAppend();

    static DialogTestAppend *self;

private:

    static const pchar BTN_ACTIVE;
    static const pchar BTN_FROM_LIBRARY;
};
