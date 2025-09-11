// 2025/7/12 16:18:43 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PanelMenu/Setup/DialogSetup.h"
#include "Panels/PanelMenu/Setup/WindowCorrectionZero.h"
#include "Panels/PanelMenu/Setup/WindowCalibrate.h"
#include "Panels/PanelMenu/Setup/WindowTableOffsets.h"
#include "Panels/PanelMenu/Setup/WindowMyComputer.h"
#include "Panels/PanelMenu/Setup/WindowUser.h"
#include "Panels/PanelMenu/Setup/WindowLaunch.h"
#include "Panels/PanelMenu/Setup/WindowAutoSave.h"
#include "Panels/PanelMenu/Setup/WindowPassword.h"
#include "Panels/PanelMenu/Setup/WindowAddressIP.h"


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

