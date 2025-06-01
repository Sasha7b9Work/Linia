// 2023/09/01 11:11:55 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Reader/SettingsMaster.h"


struct UID
{
    UID(const std::string &full_id = "")
    {
        uint pos = full_id.find('*');

        factory = full_id.substr(0, pos);
        nuid = full_id.substr(pos + 1);
    }

    std::string GetFactory() const
    {
        return factory;
    }

    std::string GetDoubleID() const
    {
        return nuid + "*" + factory;
    }

private:
    std::string factory;
    std::string nuid;   // Хоть здесь и написано "nuid", но для 4-хбайтного UID здесь хранится 3 из этих четырёх байт
                        // NUID также обрезан до трёх байт
};


struct CardInfo
{
    // persmission - если true, то карта разрешена, иначе - запрещена
    CardInfo(uint64 _number = 0, const wxString &_id = "", const wxString &_time = "", bool _permission = true) :
        number(_number),
        id(_id),
        time(_time),
        permission(_permission)
    {
    }

    uint64      number;          // Номер карты
    wxString    id;              // ID карты
    wxString    time;            // Дата выдачи
    bool        permission;

    static wxString TimeNow();

    // Преобразует в строку без id
    wxString ToString() const;

    // Преобразует в строку c id
    wxString ToStringWithID() const;
};


// Это наша текущая карта - на CommonPanel
namespace Card
{
    // double_id - в ней хранится два id - сначала идёт 7- или 4-х байтный ID, а потом 3-байтынй сокращённый
    void InsertUser(const std::string &double_id, const wxString &typeCard, const std::string &number);

    // Возвращает false в случае неудачи.
    // Возможные причины : - неверный конфиг (не совпадает контрольная сумма)
    bool InsertMaster(const std::string &double_id, const wxString &typeCard, const std::string &config);

    void InsertUnknown(const std::string &double_id, const wxString &typeCard);

    void Eject();

    bool IsInserted();

    UID GetUID();

    uint64 GetNumber();

    const SettingsMaster &GetSettingsMaster();

    const wxString &GetTypeCard();

    // Мастер-карта
    bool IsMaster();

    // Пользовательская карта
    bool IsUser();

    // Неизвестная карта - пароль не подошёл
    bool IsUnknown();
}
