// 2025/03/19 15:25:42 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Panels/Dialogs/AutoMovedDialog.h"
#include "Settings/Settings.h"
#include "Controls/TextControls.h"


// Ключи Mifare Plus - Ввод значений
class EnterKeysSL0Dialog : public AutoMovedDialog
{
public:

    static EnterKeysSL0Dialog *self;

    static void Create(Key128 *key9000, Key128 *key9001, Key128 *key9002, Key128 *key9003, Key128 *key9004);

    static void Delete();

private:

    EnterKeysSL0Dialog(Key128 *key9000, Key128 *key9001, Key128 *key9002, Key128 *key9003, Key128 *key9004);

    TextCtrlKeyHex128 *controls[5][2];

    Key128 *keys[5];        // Здесь храним указатели на ключи, чтобы сохранить новые значения при выходе

    pchar names[5] =
    {
        "Card Master Key",
        "Card Configuration Key",
        "Level 2 Switch Key",
        "Level 3 Switch Key",
        "SL1 Card Authentication Key"
    };

    void OnEventButton(wxCommandEvent &);

    void OnEventKeyHook(wxKeyEvent &);

    // Младший полубайт:
    //      0 - всё ОК
    //      1 - пароли не совпадают
    //      2 - не полностью заполнено поле пароля.
    // При ошибке 1 или 2 в старшем полубайте номер неправильного пароляпароля.
    int CheckInputData();

    // Ключи становятся текущими
    void SetKeysAsCurrent();
};
