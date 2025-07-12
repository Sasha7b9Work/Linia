// 2025/7/12 11:08:55 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Panels/Panel11_Menu/MenuDialog.h"


class DialogFile : public MenuDialog
{
public:

    DialogFile();

    static DialogFile *self;

private:

    static void OnButtonNew();
    static void OnButtonOpen();
    static void OnButtonClose();
    static void OnButtonSave();
    static void OnButtonSaveAs();
    static void OnButtonDelete();
};
