// 2025/03/19 10:04:32 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Panels/Dialogs/AutoMovedDialog.h"


class KeysSL0Dialog : public AutoMovedDialog
{
public:

    static KeysSL0Dialog *self;

    static void Create();

    static void Delete();

    void SetStateAllButtons(bool state);

    void ShowInfoDialog(bool success);

private:

    KeysSL0Dialog();

    BitSet128 old_keys[5];                  // Здесь храним предыдущие ключи для сравнения
    bool keys_is_saved = true;              // Признак того, что ключи сохранены в файле. Нужно для подтверждения выхода из диалога без сохранения ключей

    void OnEventButton(wxCommandEvent &);

    void OnEventKeyHook(wxKeyEvent &);

    wxBoxSizer *CreateBoxButton(int id, const wxString &);

    bool LoadFromFile(const wxString &);

    bool SaveToFile(const wxString &);

    // Сохранить текущие ключи в keys
    void SaveCurrentKeys(BitSet128 keys[5]);

    // Значения ключей одинаковы с keys
    bool KeyValuesAreTheSame(BitSet128 keys[5]) const;
};
