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
            WindowUser().ShowOnWindow(DialogSetup::self->FindButton("Настройки пользователя"));
        },
        "Запуск", []()
        {
            WindowLaunch().ShowOnWindow(DialogSetup::self->FindButton("Запуск"));
        },
        "Автосохранение", []()
        {
            WindowAutoSave().ShowOnWindow(DialogSetup::self->FindButton("Автосохранение"));
        },
        "Коррекция смещения нуля", []()
        {
            WindowCorretionZero().ShowOnWindow(DialogSetup::self->FindButton("Коррекция смещения нуля"));
        },
        "Калибровка", []()
        {
            WindowCalibrate().ShowOnWindow(DialogSetup::self->FindButton("Калибровка"));
        },
        "Таблица смещений и коэффициентов", []()
        {
            WindowTableOffsets().ShowOnWindow(DialogSetup::self->FindButton("Таблица смещений и коэффициентов"));
        },
        "Изменить пароль", []()
        {
            WindowPassword().ShowOnWindow(DialogSetup::self->FindButton("Изменить пароль"));
        },
        "IP-адрес", []()
        {
            WindowAddressIP().ShowOnWindow(DialogSetup::self->FindButton("IP-адрес"));
        },
        "Мой компьютер", []()
        {
            WindowMyComputer().ShowOnWindow(DialogSetup::self->FindButton("Мой компьютер"));
        }
    )
{
    self = this;
}

