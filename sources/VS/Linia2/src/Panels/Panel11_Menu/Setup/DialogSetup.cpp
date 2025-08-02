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
        _L("Настройки пользователя"), []()
        {
            WindowUser().ShowModal();
        },
        _L("Запуск"), []()
        {
            WindowLaunch().ShowModal();
        },
        _L("Автосохранение"), []()
        {
            WindowAutoSave().ShowModal();
        },
        _L("Коррекция смещения нуля"), []()
        {
            WindowCorretionZero().ShowModal();
        },
        _L("Калибровка"), []()
        {
            WindowCalibrate().ShowModal();
        },
        _L("Таблица смещений и коэффициентов"), []()
        {
            WindowTableOffsets().ShowModal();
        },
        _L("Изменить пароль"), []()
        {
            WindowPassword().ShowModal();
        },
        _L("IP-адрес"), []()
        {
            WindowAddressIP().ShowModal();
        },
        _L("Мой компьютер"), []()
        {
            WindowMyComputer().ShowModal();
        }
    )
{
    self = this;
}

