// 2025/7/12 16:18:43 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel11_Menu/Setup/DialogSetup.h"
#include "Panels/Panel11_Menu/Setup/WindowCorrectionZero.h"
#include "Panels/Panel11_Menu/Setup/WindowCalibrate.h"
#include "Panels/Panel11_Menu/Setup/WindowTableOffsets.h"
#include "Panels/Panel11_Menu/Setup/WindowMyComputer.h"
#include "Panels/Panel11_Menu/Setup/WindowUser.h"
#include "Panels/Panel11_Menu/Setup/WindowLaunch.h"
#include "Panels/Panel11_Menu/Setup/WindowAutoSave.h"
#include "Panels/Panel11_Menu/Setup/WindowPassword.h"
#include "Panels/Panel11_Menu/Setup/WindowAddressIP.h"


DialogSetup *DialogSetup::self = nullptr;


DialogSetup::DialogSetup() :
    MenuDialog(_L("Настройка"), 275,
        _L("Настройки пользователя"), ID_BTN_USER, []()
        {
            WindowUser().ShowModal();
        },
        _L("Запуск"), ID_BTN_LAUNCH, []()
        {
            WindowLaunch().ShowModal();
        },
        _L("Автосохранение"), ID_BTN_AUTOSAVE, []()
        {
            WindowAutoSave().ShowModal();
        },
        _L("Коррекция смещения нуля"), ID_BTN_CORRECTION_ZERO, []()
        {
            WindowCorretionZero().ShowModal();
        },
        _L("Калибровка"), ID_BTN_CALIBRATION, []()
        {
            WindowCalibrate().ShowModal();
        },
        _L("Таблица смещений и коэффициентов"), ID_BTN_TABLE, []()
        {
            WindowTableOffsets().ShowModal();
        },
        _L("Изменить пароль"), ID_BTN_PASSWORD, []()
        {
            WindowPassword().ShowModal();
        },
        _L("IP-адрес"), ID_BTN_ADDRESS_IP, []()
        {
            WindowAddressIP().ShowModal();
        },
        _L("Мой компьютер"), ID_BTN_MY_COMPUTER, []()
        {
            WindowMyComputer().ShowModal();
        }
    )
{
    self = this;
}

