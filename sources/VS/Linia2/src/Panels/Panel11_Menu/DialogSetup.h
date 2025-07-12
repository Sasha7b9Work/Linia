// 2025/7/12 16:18:39 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Panels/Panel11_Menu/MenuDialog.h"


class DialogSetup : public MenuDialog
{
public:

    DialogSetup();

    static DialogSetup *self;

private:

    static void OnButtonUser();
    static void OnButtonStart();
    static void OnButtonAutoSave();
    static void OnButtonCorrectionZero();
    static void OnButtonCalibration();
    static void OnButtonTable();
    static void OnButtonPassword();
    static void OnButtonAddressIP();
    static void OnButtonMyComputer();
};
