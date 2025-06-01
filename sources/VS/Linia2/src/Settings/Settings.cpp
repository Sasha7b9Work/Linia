// 2023/08/09 19:11:58 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Settings/Settings.h"
#include "Utils/Configurator.h"
#include "Panels/Notebook/PageUser/PageUser.h"
#include "Panels/Notebook/PageMaster/PageMaster.h"
#include "Panels/Notebook/PageMaster/OfflineModeDialog.h"
#include "Panels/Notebook/PageMaster/OSDPModeDialog.h"
#include "Panels/Notebook/PageMaster/AntiBreakDialog.h"
#include "Panels/Notebook/PageMaster/ExtendedSettingsDialog.h"
#include "Panels/Notebook/PageMaster/WiegandDialog.h"
#include "Panels/Dialogs/PasswordExtraDialog.h"
#include "Panels/Dialogs/SettingsSerialPort.h"
#include "Panels/Notebook/PageMaster/ModeEKODialog.h"
#include "Panels/Notebook/PageMaster/ChooseSoundsDialog.h"


namespace SS
{
    static void LoadSave(bool load, bool save);
}


uint BaudRateOSDP::FromIndexCombobox(int index)
{
    if (index == 0)
    {
        return 9600;
    }
    else if (index == 1)
    {
        return 19200;
    }
    else if (index == 2)
    {
        return 38400;
    }

    return 9600;
}


namespace SET
{
    ValueCheckButton mode_eco("mode_eco", false);

    namespace MASTER
    {
        ValueComboBox melody[3] =
        {
            ValueComboBox("melody0", 0),
            ValueComboBox("melody1", 1),
            ValueComboBox("melody2", 2)
        };

        ValueSpinCtrl volume[3] =
        {
            ValueSpinCtrl("volume0", 3),
            ValueSpinCtrl("volume1", 3),
            ValueSpinCtrl("volume2", 3)
        };

        ValueColor      color_green("color_green", Color(0, 255, 0, 255));
        ValueColor      color_red("color_red", Color(255, 0, 0, 255));

        // Запись на мастер-карту
        ValueCheckBox   write_config("master_write_config", true);
    }

    // Дополнительно - Автономный режим
    namespace OFFLINE
    {
        ValueCheckButton enabled("offline_mode_enabled", false);
        ValueCheckBox    security_mode("offline_security_mode", false);
        ValueTextCtrl    time_lock("offline_time_lock", 5);
        ValueTextCtrl    time_alarm("offline_time_alarm", 30);
        Value<wxString>  file_cards("gui_file_offline_cards", "");
    }

    namespace WIEGAND
    {
        ValueTextCtrl   value{ "wiegand_value", 26 };
        ValueCheckBox   full_guid{ "wiegand_full_guild", false };
        ValueCheckBox   control_bits{ "wiegand_control_bits", true };
        ValueCheckBox   inverse_control_bits{ "wiegand_inverse_control_bits", false };
        ValueCheckBox   reverse_order_bits{ "wiegand_reverse_order_bits", false };
        ValueCheckBox   nuid_discard_LSB{ "wiegand_nuid_discard_lsb", false };

        //   CBA9876543210
        // 0b0000011111111
        //   |||||||||||||
        //   |||||++++++++--- 8 бит значения
        //   ||||+----------- full guid             Если 1, то передаём максимально возможное количество бит, отбрасывая те, которые не влазят. Иначе - передаём 3 байта NUID
        //   |||+------------ control bits          Если 1, то передаём два контрольных бита
        //   ||+------------- inverse control bits  Если 1, то меняются местами расчёт чётного и нечётного битов
        //   |+-------------- reverse order bits    Если 1, то биты передаются в обратном порядке
        //   +--------------- nuid discard LSB      Если 1, то из четырёхбайтного NUID отбрасывается 
        uint GetFullValue()
        {
            uint result = (uint)value.Get();

            if (full_guid.Get())            _SET_BIT(result, 8);
            if (control_bits.Get())         _SET_BIT(result, 9);
            if (inverse_control_bits.Get()) _SET_BIT(result, 10);
            if (reverse_order_bits.Get())   _SET_BIT(result, 11);
            if (nuid_discard_LSB.Get())     _SET_BIT(result, 12);

            return result;
        }
    }

    // Дополнительно - Режим OSDP
    namespace OSDP
    {
        ValueCheckButton enabled("osdp_enabled", false);
        ValueTextCtrl    address("osdp_address", 1);
        ValueComboBox    baudrate("osdp_baudrate", 0);
        ValueCheckBox    crypto_enabled("osdp_crypto_enabled", false);
    }

    // Дополнительно - Датчик отрыва
    namespace ANTIBREAK
    {
        ValueCheckButton enabled("antibreak_enabled", false);
        ValueTextCtrl    number("antibreak_number", 0);
        ValueComboBox    sens("antibreak_sens", 0);

        uint8 GetRAW()
        {
            if (!enabled.Get())
            {
                return 0;
            }

            return (uint8)(sens.GetIndex() + 1);
        }
    }

    // Расширенные настройки
    namespace EXT
    {
        ValueCheckButton enabled("ext_master_enable_adv_settings", false);
        ValueComboBox    mode_read_card("ext_mode_read_card", 0);
        ValueTextCtrl    period_autorepeat("ext_period_autorepeat", 5000);
        ValueCheckBox    parity("ext_parity", false);
        ValueCheckBox    inverse_code("ext_inverse_code", false);
        ValueCheckBox    control_bit("ext_control_bit", false);
    }

    namespace MISC
    {
        ValueCheckBox disable_less_SL3{ "disable_less_SL3", false };
    }

    namespace USER
    {
        ValueTextCtrl   number_first("user_number_first", 1);
        ValueTextCtrl   number_last("user_number_last", 100);
        ValueTextCtrl   number_next("user_number_next", 1);
        ValueCheckBox   enabled_range_generation("user_enabled_range_generation", false);
        ValueCheckBox   write_client_number("user_write_client_number", true);
        Value<wxString> file_cards("gui_file_cards_normal", "base.cards");
    }

    namespace GUI
    {
        Value<wxPoint>   position("gui_position", { 100, 100 });
        Value<wxPoint>   pos_console("gui_pos_console", { 10, 10 });
        Value<wxPoint>   size_console("gui_size_console", { 400, 400 });
        Value<wxPoint>   pos_list("gui_pos_list", { 10, 10 });
        Value<wxPoint>   size_list("gui_size_list", { 400, 400 });
        Value<bool>      maximized_list("gui_maximized_list", false);
        Value<bool>      maximized_console("gui_maximized_console", false);
        Value<int>       current_page_notebook("gui_current_page_notebook", 0);
        ValueCheckButton serial_port_auto_find("serial_port_auto_find", true);
        Value<int>       serial_port_num("serial_port_num", 0);
    }

    void Init()
    {
        Load("");
    }

    void Load(const wxString &file_path)
    {
        Config::SetFile(file_path);

        SS::LoadSave(true, false);

        mode_eco.LoadToControl(ModeEKODialog::self, ID_CHECKBUTTON_MODE_EKO);

        GUI::serial_port_auto_find.LoadToControl(SettingsSerialPort::self, ID_CHECKBUTTON_COMPORT_AUTOFIND);

        MASTER::color_green.Load(PageMaster::self->buttonGREEN);
        MASTER::color_red.Load(PageMaster::self->buttonRED);

        WIEGAND::full_guid.LoadToControl(WiegandDialog::self, ID_CHECKBOX_WIEGAND_FULL_GUID);
        WIEGAND::control_bits.LoadToControl(WiegandDialog::self, ID_CHECKBOX_WIEGAND_CONTROL_BITS);
        WIEGAND::inverse_control_bits.LoadToControl(WiegandDialog::self, ID_CHECKBOX_WIEGAND_INVERSE_CONTROL_BITS);
        WIEGAND::reverse_order_bits.LoadToControl(WiegandDialog::self, ID_CHECKBOX_WIEGAND_REVERSE_ORDER_BITS);
        WIEGAND::nuid_discard_LSB.LoadToControl(WiegandDialog::self, ID_CHECKBOX_WIEGAND_NUID_DISCARD_LSB);
        WIEGAND::value.LoadToControl(WiegandDialog::self, ID_TEXTCNTRL_WIEGAND_VALUE);

        // Конфигурация
        for (int i = 0; i < 3; i++)
        {
            MASTER::melody[i].LoadToControl(ChooseSoundsDialog::self, ID_COMBOBOX_MELODY + i);
            MASTER::volume[i].LoadToControl(ChooseSoundsDialog::self, ID_SPINCNTRL_VOLUME + i);
        }

        // Запись на мастер-карту
        MASTER::write_config.LoadToControl(PageMaster::self, ID_CHECKBOX_WRITE_MASTER_CONFIG);

        // Дополнительно - Автономный режим
        OFFLINE::enabled.LoadToControl(OfflineModeDialog::self, ID_CHECKBUTTON_OFFLINE_ENABLED);
        OFFLINE::security_mode.LoadToControl(OfflineModeDialog::self, ID_CHECKBOX_OFFLINE_SECURITY_MODE);
        OFFLINE::time_lock.LoadToControl(OfflineModeDialog::self, ID_TEXTCNTRL_OFFLINE_TIME_LOCK);
        OFFLINE::time_alarm.LoadToControl(OfflineModeDialog::self, ID_TEXTCNTRL_OFFLINE_TIME_ALARM);
        OFFLINE::file_cards.Load();
        if (!wxFileExists(wxGetCwd() + "/" + OFFLINE::file_cards.Get()))
        {
            OFFLINE::file_cards.Set("");
        }

        // Дополнительно - режим OSDP
        OSDP::address.LoadToControl(OSDPModeDialog::self, ID_TEXTCNTRL_OSDP_ADDRESS);
        OSDP::baudrate.LoadToControl(OSDPModeDialog::self, ID_COMBOBOX_OSDP_BAUDRATE);
        OSDP::enabled.LoadToControl(OSDPModeDialog::self, ID_CHECKBUTTON_OSDP_ENABLED);
        OSDP::crypto_enabled.LoadToControl(OSDPModeDialog::self, ID_CHECKBOX_OSDP_ENCRYPTION);

        // Дополнительно - Датчик отрыва
        ANTIBREAK::enabled.LoadToControl(AntiBreakDialog::self, ID_CHECKBUTTON_ANTIBREAK_ENABLED);
        ANTIBREAK::number.LoadToControl(AntiBreakDialog::self, ID_TEXTCNTRL_ANTIBREAK_NUMBER);
        ANTIBREAK::sens.LoadToControl(AntiBreakDialog::self, ID_COMBOBOX_ANTIBREAK_SENS);

        // Расширенные настройки
        EXT::mode_read_card.LoadToControl(ExtendedSettingsDialog::self, ID_COMBOBOX_EXT_MODE_READ_CARD);
        EXT::period_autorepeat.LoadToControl(ExtendedSettingsDialog::self, ID_TEXTCNTRL_EXT_PERIOD_AUTOREPEAT);
        EXT::parity.LoadToControl(ExtendedSettingsDialog::self, ID_CHECKBOX_EXT_PARITY);
        EXT::inverse_code.LoadToControl(ExtendedSettingsDialog::self, ID_CHECKBOX_EXT_INVERSION_CODE);
        EXT::control_bit.LoadToControl(ExtendedSettingsDialog::self, ID_CHECKBOX_EXT_CONTROL_BIT);
        EXT::enabled.LoadToControl(ExtendedSettingsDialog::self, ID_CHECKBUTTON_EXT_SET_ENABLE);
        EXT::enabled.SendEventToGUI(ExtendedSettingsDialog::self, ID_CHECKBUTTON_EXT_SET_ENABLE);

        MISC::disable_less_SL3.LoadToControl(PasswordExtraDialog::self, ID_CHECKBOX_DISABLE_BELOW_LEVEL_SL3);

        ////////////////////////////////////////////////// Карты доступа

        // Генерация номера карты
        USER::number_first.LoadToControl(PageUser::self, ID_TEXTCNTRL_NUMBER_FIRST);
        USER::number_last.LoadToControl(PageUser::self, ID_TEXTCNTRL_NUMBER_LAST);
        USER::number_next.LoadToControl(PageUser::self, ID_TEXTCNTRL_NUMBER_NEXT);
        USER::enabled_range_generation.LoadToControl(PageUser::self, ID_CHECKBOX_RANGE);
        USER::enabled_range_generation.ApplyToGUI(PageUser::self, ID_CHECKBOX_RANGE);
        USER::file_cards.Load();
        if (!wxFileExists(wxGetCwd() + "/" + USER::file_cards.Get()))
        {
            USER::file_cards.Set("base.cards");
        }


        // Запись на карту
        USER::write_client_number.LoadToControl(PageUser::self, ID_CHECKBOX_WRITE_CLIENT_NEW_NUMBER);

        Config::SetFile("");

        SS::LoadSave(false, true);
    }


    void Save(const wxString &file_path)
    {
        mode_eco.Set(false);
        OFFLINE::enabled.Set(false);
        OSDP::enabled.Set(false);
        ANTIBREAK::enabled.Set(false);

        SS::LoadSave(true, false);

        Config::SetFile(file_path);

        SS::LoadSave(false, true);

        MASTER::color_green.Save();
        MASTER::color_red.Save();

        WIEGAND::value.Save();
        WIEGAND::full_guid.Save();
        WIEGAND::control_bits.Save();
        WIEGAND::inverse_control_bits.Save();
        WIEGAND::reverse_order_bits.Save();
        WIEGAND::nuid_discard_LSB.Save();

        // Конфигурация
        for (ValueComboBox &value : MASTER::melody)
        {
            value.Save();
        }
        for (ValueSpinCtrl &value : MASTER::volume)
        {
            value.Save();
        }

        // Запись на мастер-карту
        MASTER::write_config.Save();

        // Дополнительно - Автономный режим
        OFFLINE::security_mode.Save();
        OFFLINE::time_lock.Save();
        OFFLINE::time_alarm.Save();
        OFFLINE::file_cards.Save();

        // Дополнительно - режим OSDP
        OSDP::address.Save();
        OSDP::baudrate.Save();
        OSDP::crypto_enabled.Save();

        // Дополнительно - Датчик отрыва
        ANTIBREAK::number.Save();
        ANTIBREAK::sens.Save();

        // Расширенные настройки
        EXT::mode_read_card.Save();
        EXT::period_autorepeat.Save();
        EXT::parity.Save();
        EXT::inverse_code.Save();
        EXT::control_bit.Save();
        EXT::enabled.Save();

        MISC::disable_less_SL3.Save();

        ////////////////////////////////////////////////// Карты доступа

        // Генерация номера карты
        USER::number_first.Save();
        USER::number_last.Save();
        USER::number_next.Save();
        USER::enabled_range_generation.Save();
        USER::file_cards.Save();

        // Запись на карту
        USER::write_client_number.Save();

        Config::SetFile("");
    }
}


void SET::GUI::Load()
{
    position.Load();
    pos_console.Load();
    size_console.Load();
    pos_list.Load();
    size_list.Load();
    maximized_list.Load();
    maximized_console.Load();
    current_page_notebook.Load();
    serial_port_num.Load();

    serial_port_auto_find.LoadToControl(SettingsSerialPort::self, ID_CHECKBUTTON_MODE_EKO);
}



const Password password_factory{ 0, "" };

Settings gset
{
    { 0, "/password_card" },       // password_card     Текущий пароль карты
    { 0, "/password_master" },     // password_master   Пароль к мастер-карте
    { 0, "/password_current" },    // password_current  Действующий пароль системы
    { { { 0x0101010101010101, 0x0101010101010101 } },
    {   { 0x0202020202020202, 0x0202020202020202 } },
    {   { 0x0303030303030303, 0x0303030303030303 } },
    {   { 0x0404040404040404, 0x0404040404040404 } },
    {   { 0x0505050505050505, 0x0505050505050505 } } }
};


void SS::LoadSave(bool load, bool save)
{
    if (load)
    {
        gset.password_master.value = Config::ReadUInt64(gset.password_master.key);
        gset.password_current.value = Config::ReadUInt64(gset.password_current.key);
    }

    if(save)
    {
        Config::WriteUInt64(gset.password_master.key, gset.password_master.value);
        Config::WriteUInt64(gset.password_current.key, gset.password_current.value);
    }
}


wxString Password::ToString() const
{
    return wxString::Format("%llu", value);
}
