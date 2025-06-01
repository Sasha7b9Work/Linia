// 2023/09/01 11:11:43 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Reader/Card.h"
#include "Panels/CommonPanel.h"
#include "Utils/GlobalFunctions.h"
#include "Panels/Notebook/Notebook.h"
#include "Panels/Dialogs/CardInfoDialog.h"
#include "DataBase/DataBasePasswords.h"
#include "Reader/Indicator.h"
#include "Reader/PasswordSender.h"
#include "Panels/Notebook/PageUser/PageUser.h"
#include "Panels/Notebook/PageMaster/PageMaster.h"
#include "Utils/StringUtils.h"
#include "Panels/Dialogs/KeysSL0Dialog.h"
#include "Panels/Dialogs/EnterKeysSL0Dialog.h"


namespace Card
{
    struct FunctionCard
    {
        enum E
        {
            None,       // Карта не загружена
            Unknown,    // Назначение карты неизвестно (не подошёл пароль или не определяется назначение)
            Master,     // Мастер-карта
            User        // Пользовательская карта
        };
    };

    static FunctionCard::E function_card;

    static UID uid;
    static uint64 number = 0;       // Для мастер-карты в младших битах 0xFFFFFFFF
    static bool inserted = false;
    static wxString typeCard;

    static SettingsMaster set_master;

    static void SetEnabled(bool);
}


const wxString &Card::GetTypeCard()
{
    return typeCard;
}


const SettingsMaster &Card::GetSettingsMaster()
{
    return set_master;
}


UID Card::GetUID()
{
    return uid;
}


uint64 Card::GetNumber()
{
    return number;
}


bool Card::IsInserted()
{
    return inserted;
}


void Card::InsertUnknown(const std::string &_double_id, const wxString &_typeCard)
{
    function_card = FunctionCard::Unknown;

    String type(_typeCard.c_str().AsChar());

    SU::ReplaceSymbols(type.c_str(), '_', ' ');

    typeCard = type.c_str();

    uid = UID(_double_id);

    SetEnabled(true);

    CardInfoDialog::Fill();
}


void Card::InsertUser(const std::string &double_id, const wxString &_typeCard, const std::string &_number)
{
    function_card = FunctionCard::User;

    String type(_typeCard.c_str().AsChar());

    SU::ReplaceSymbols(type.c_str(), '_', ' ');

    typeCard = type.c_str();

    uid = UID(double_id);

    uint64 pass = 0;

    if (PasswordSender::CurrentPassword().ToULongLong(&pass))
    {
        PasswDB::Append(pass);
    }

    wxString(_number).ToULongLong(&number);

    SetEnabled(true);

    CardInfoDialog::Fill();
}


bool Card::InsertMaster(const std::string &_double_id, const wxString &_typeCard, const std::string &config)
{
    if (config.length() != sizeof(SettingsMaster) * 2)         // Размер строки в два раза больше размера настроек, потому что каждый байт занимает два символа
    {
        return false;
    }

    function_card = FunctionCard::Master;

    String type(_typeCard.c_str().AsChar());

    SU::ReplaceSymbols(type.c_str(), '_', ' ');

    typeCard = type.c_str(); 

    {   // Получаем конфигурацию
        const char *pointer_str = config.c_str();

        uint *pointer_set = (uint *)&set_master;

        for (int i = 0; i < (int)(sizeof(SettingsMaster) / sizeof(*pointer_set)); i++)
        {
            char buffer[9] = { '0', '0', '0', '0', '0', '0', '0', '0', '\0' };

            for (int j = 0; j < 8; j++)
            {
                buffer[j] = *pointer_str++;
            }

            *pointer_set++ = (uint)std::stoll(buffer, nullptr, 16);
        }

        if (!set_master.CheckSumIsMatches())
        {
            return false;
        }
    }

    uid = UID(_double_id);

    number = 0xFFFFFFFF;

    SetEnabled(true);

    CardInfoDialog::Fill();

    return true;
}


void Card::Eject()
{
    function_card = FunctionCard::None;

    SetEnabled(false);

    CardInfoDialog::Fill();

//    EnterKeysSL0Dialog::self->Close();

    KeysSL0Dialog::self->Close();
}


void Card::SetEnabled(bool enable)
{
    enable ? Indicator::Card() : Indicator::On();

    inserted = enable;

    GF::FindAnyButton(CommonPanel::self, ID_BUTTON_CARD_INFO)->SetLabel(enable ? (IsUnknown() ? _L("Неизвестная карта") : _L("Информация")) : _L("Текущий пароль"));
    if (enable)
    {
        GF::FindAnyButton(PageMaster::self, ID_BUTTON_WRITE_MASTER_CARD)->SetLabel(_L("Записать на карту"));
        GF::FindAnyButton(PageUser::self, ID_BUTTON_WRITE_USER_CARD)->SetLabel(_L("Записать на карту"));
    }
    GF::FindAnyButton(PageMaster::self, ID_BUTTON_WRITE_MASTER_CARD)->Enable(enable);
    GF::FindAnyButton(PageUser::self, ID_BUTTON_WRITE_USER_CARD)->Enable(enable);
    GF::FindAnyButton(CommonPanel::self, ID_BUTTON_PASSWORD_MASTER_CARD)->Enable(Notebook::self->GetSelection() == 1 ? false : enable);
}


bool Card::IsMaster()
{
    return ((GetNumber() & 0xFFFFFFFF) == 0xFFFFFFFF) &&
        (function_card == FunctionCard::Master);
}


bool Card::IsUser()
{
    return (function_card == FunctionCard::User);
}


bool Card::IsUnknown()
{
    return (function_card == FunctionCard::Unknown);
}


wxString CardInfo::TimeNow()
{
    wxDateTime time = wxDateTime::Now();

    return time.Format("%d/%m/%y %H:%M:%S");
}


wxString CardInfo::ToString() const
{
    return wxString::Format("%llu %s %u", number, time, permission ? 1 : 0);
}


wxString CardInfo::ToStringWithID() const
{
    return wxString::Format("%s %llu %s %u", id, number, time, permission ? 1 : 0);
}
