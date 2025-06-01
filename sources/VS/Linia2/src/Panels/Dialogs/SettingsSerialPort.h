// 2024/11/01 14:17:09 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Panels/Dialogs/AutoMovedDialog.h"


class SettingsSerialPort : public AutoMovedDialog
{
public:

    static SettingsSerialPort *self;

    static void Create();
    static void Delete();

    virtual int ShowModal() override;

private:

    SettingsSerialPort();

    void OnEventToggleButton(wxCommandEvent &);

    void OnEventButton(wxCommandEvent &);

    void OnEventComboBox(wxCommandEvent &);

    // Сохранить номер порта в настройках
    void SaveNumberPort();

    std::vector<bool> ports;    // Список портов. 0-й элемент - COM1
};