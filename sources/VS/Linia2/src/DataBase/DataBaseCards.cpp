// 2024/01/22 15:16:54 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "DataBase/DataBaseCards.h"
#include "Settings/Settings.h"
#include "Panels/Notebook/PageUser/ListCards.h"
#include "DataBase/Server/Server.h"
#include <cstring>


namespace CardsDB
{
    static std::map<wxString, CardInfo> cards;

    static uint left_generate_cards = 0;

    static wxString file_database;

    static void LoadFromFile(const wxString &);
}


void CardsDB::GenerateCards()
{
    left_generate_cards = 600;
};


void CardsDB::Update()
{
    if (left_generate_cards)
    {
        for (int i = 0; i < 10; i++)
        {
            if (left_generate_cards)
            {
                wxString time = CardInfo::TimeNow();

                CardsDB::AppendCard(CardInfo(left_generate_cards, wxString::Format("%llu", std::rand() * std::rand()), time, true));

                left_generate_cards--;
            }
            else
            {
                break;
            }
        }
    }
}


bool CardsDB::ExistUID(const UID &uid)
{
    CardInfo info;

    return ReadCard(uid.GetFactory(), &info);
}


bool CardsDB::ReadCard(const wxString &id, CardInfo *card)
{
    auto info = cards.find(id);

    if (info == cards.end())
    {
        return false;
    }

    *card = info->second;

    return true;
}


bool CardsDB::ExistNumber(uint64 number)
{
    for (auto card : cards)
    {
        if (card.second.number == number)           // \todo это убивает быстродействие
        {
            return true;
        }
    }

    return false;
}


wxString CardsDB::GetBits(uint num_cards)
{
    const int NUM_BYTES = 1024 * 10;

    uint8 bytes[NUM_BYTES];

    std::memset(bytes, 0x00, NUM_BYTES);

    for (auto &item : cards)
    {
        CardInfo &card = item.second;

        uint64 number = card.number;

        if (number <= num_cards && card.permission)
        {
            uint64 index = card.number / 8;

            int bit = (int)(number - index * 8);

            bytes[index] |= (1 << bit);
        }
    }

    char buffer[NUM_BYTES / 4] = { '\0' };

    uint counter = (num_cards + 1) / 8;

    if (((num_cards + 1) % 8) != 0)
    {
        counter++;
    }

    for (uint i = 0; i < counter; i++)
    {
        char byte[32];

        std::sprintf(byte, "%02X", bytes[i]);

        std::strcat(buffer, byte);
    }

    return wxString(buffer);
}


void CardsDB::AppendCard(const CardInfo &card)
{
    Server::Send::AppendCard(card);

    LoadCardFromDataBase(card);
}


void CardsDB::LoadCardFromDataBase(const CardInfo &card)
{
    cards[card.id] = card;

    ListCards::self->SetCard(card);
}


uint CardsDB::GetCountCards()
{
    return cards.size();
}


void CardsDB::DeleteCardByID(const wxString& id)
{
    cards.erase(id);

    ListCards::self->DeleteCard(id);
}


void CardsDB::LoadDataBase()
{
    if (SET::OFFLINE::enabled.Get())
    {
        LoadFromFile(SET::OFFLINE::file_cards.Get());
    }
    else
    {
        LoadFromFile(SET::USER::file_cards.Get());
    }
}


void CardsDB::SaveDataBase()
{
    Server::Send::SaveBase();
}


void CardsDB::SaveDataBaseToFile(const wxString &file_name)
{
    Server::Send::SaveBaseToFile(file_name);
}


void CardsDB::LoadFromFile(const wxString &file_name)
{
    if (file_name == file_database)
    {
        return;
    }

    file_database = file_name;

    Clear();

    SaveDataBase();

    Server::Send::LoadBase(file_database);
}


void CardsDB::Clear()
{
    cards.clear();
    ListCards::self->Clear();
}
