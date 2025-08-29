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
    MenuDialog("Настройка", 275, { 2, 5 },
        "Настройки пользователя", []()
        {
            WindowUser().ShowModal();
        },
        "Запуск", []()
        {
            WindowLaunch().ShowModal();
        },
        "Автосохранение", []()
        {
            WindowAutoSave().ShowModal();
        },
        "Коррекция смещения нуля", []()
        {
            WindowCorretionZero().ShowModal();
        },
        "Калибровка", []()
        {
            WindowCalibrate().ShowModal();
        },
        "Таблица смещений и коэффициентов", []()
        {
            WindowTableOffsets().ShowModal();
        },
        "Изменить пароль", []()
        {
            WindowPassword().ShowModal();
        },
        "IP-адрес", []()
        {
            WindowAddressIP().ShowModal();
        },
        "Мой компьютер", []()
        {
            WindowMyComputer().ShowModal();
        }
    )
{
    self = this;
}

