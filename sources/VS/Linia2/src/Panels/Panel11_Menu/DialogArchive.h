// 2025/7/12 16:57:10 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Panels/Panel11_Menu/MenuDialog.h"


class DialogArchive : public MenuDialog
{
public:

    DialogArchive();

    static DialogArchive *self;

private:

    static void OnButtonClear();
    static void OnButtonDeleteFrom();
    static void OnButtonLoadFrom();
    static void OnButtonOpen();
    static void OnButtonClose();
};
