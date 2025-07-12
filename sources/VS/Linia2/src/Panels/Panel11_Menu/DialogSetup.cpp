// 2025/7/12 16:18:43 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel11_Menu/DialogSetup.h"


DialogSetup *DialogSetup::self = nullptr;


DialogSetup::DialogSetup() :
    MenuDialog(_L("Настройка"), 275,
        _L("Настройки пользователя"), ID_BTN_MENU_SETUP_USER, OnButtonUser,
        _L("Запуск"), ID_BTN_MENU_SETUP_START, OnButtonStart,
        _L("Автосохранение"), ID_BTN_MENU_SETUP_AUTOSAVE, OnButtonAutoSave,
        _L("Коррекция смещения нуля"), ID_BTN_MENU_SETUP_CORRECTION_ZERO, OnButtonCorrectionZero,
        _L("Калибровка"), ID_BTN_MENU_SETUP_CALIBRATION, OnButtonCalibration,
        _L("Таблица смещений и коэффициентов"), ID_BTN_MENU_SETUP_TABLE, OnButtonTable,
        _L("Изменить пароль"), ID_BTN_MENU_SETUP_PASSWORD, OnButtonPassword,
        _L("IP-адрес"), ID_BTN_MENU_SETUP_ADDRESS_IP, OnButtonAddressIP,
        _L("Мой компьютер"), ID_BTN_MENU_SETUP_MY_COMPUTER, OnButtonMyComputer
    )
{
    self = this;
}


void DialogSetup::OnButtonUser()
{

}


void DialogSetup::OnButtonStart()
{

}


void DialogSetup::OnButtonAutoSave()
{

}


void DialogSetup::OnButtonCorrectionZero()
{

}


void DialogSetup::OnButtonCalibration()
{

}


void DialogSetup::OnButtonTable()
{

}


void DialogSetup::OnButtonPassword()
{

}


void DialogSetup::OnButtonAddressIP()
{

}


void DialogSetup::OnButtonMyComputer()
{

}
