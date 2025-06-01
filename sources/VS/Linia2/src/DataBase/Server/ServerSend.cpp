// 2024/08/29 17:00:35 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "DataBase/Server/Server.h"
#include "DataBase/Server/Queue.h"
#include "Utils/Timer.h"
#include "Utils/StringUtils.h"
#include "DataBase/Server/TaskRequest.h"
#include <thread>


namespace Server
{
    static void FuncThreadServer(volatile bool *is_runing);
    static volatile bool is_running_thread = false;

    static Queue<Request::Task> in_queue;               // Сюда склдываются задания, которые будет выполнять поток сервера

    static std::map<wxString, CardInfo> cards_map;      // Карт карт по их id
    static std::vector<CardInfo>        cards_vector;   // Вектор карт

    static wxTextFile *file_cards = nullptr;            // С этим файлом сейчас происходит работа

    static void AppendNew(const wxString &);

    static bool Decode(const wxString &, CardInfo &);
}


void Server::FuncThreadServer(volatile bool *_is_running)
{
    volatile bool *is_running = _is_running;             // Когда значение по этому адресу станет равным false - поток должен завершиться

    *is_running = true;

    Request::Task *task = nullptr;

    while (*is_running || task || in_queue.Size())
    {
        if (task)
        {
            task->Execute();
            SAFE_DELETE(task);
        }
        else
        {
            task = in_queue.GetFirst();
        }
    }

    *is_running = true;     // Устанавливаем признак того, что поток завершён
}


void Server::Create()
{
    std::thread(FuncThreadServer, &is_running_thread).detach();
}


void Server::Destroy()
{
    is_running_thread = false;

    while (!is_running_thread)    // Дожидаемся признака того, что поток завершён
    {
    }
}


void Server::Request::SaveBase::Execute()
{
    if (!cards_map.empty())
    {
        if (file_cards->Exists())
        {
            file_cards->Open();
        }
        else
        {
            file_cards->Create();
        }
        LOG_WRITE_SERVER("Begin save database to %s", file_cards->GetName().c_str().AsChar());
        file_cards->Clear();
        for (auto &item : cards_map)
        {
            file_cards->AddLine(item.second.ToStringWithID());
        }
        file_cards->Write();
        file_cards->Close();
        SAFE_DELETE(file_cards);
    }
}


void Server::Request::LoadBase::Execute()
{
    wxString name = wxGetCwd() + "/" + file_name;

//    LOG_WRITE_SERVER("Open file \"%s\"", name.c_str().AsChar());

    SAFE_DELETE(file_cards);

    file_cards = new wxTextFile(name);

    if (file_cards->Exists())
    {
        file_cards->Open();

        cards_map.clear();
        cards_vector.clear();

        Timer::PauseOnMS(1);

        LOG_WRITE_SERVER("Begin load database from %s", file_cards->GetName().c_str().AsChar());

//        int64 time_start = Timer::CurrentTimeMS();

        User::BeginLoadBase(file_cards->GetLineCount());

        if (file_cards->IsOpened())
        {
            AppendNew(file_cards->GetFirstLine());

            while (!file_cards->Eof())
            {
                AppendNew(file_cards->GetNextLine());
            }
        }

        file_cards->Close();

        User::EndLoadBase();

//        LOG_WRITE_SERVER("Time load %d ms. Loaded %d cards", Timer::CurrentTimeMS() - time_start, cards_map.size());
    }
    else
    {
        LOG_ERROR_SERVER("File %s not exists", file_cards->GetName().c_str().AsChar());
    }
}


void Server::AppendNew(const wxString &line)
{
    if (line.IsEmpty())
    {
        return;
    }

    CardInfo card;

    if (Decode(line, card))
    {
        cards_map[card.id] = card;
        cards_vector.push_back(card);
        User::AppendCard(card);
    }
    else
    {
        LOG_ERROR_SERVER("Incorrect input data for card : \"%s\"", line.c_str().AsChar());
    }
}


bool Server::Decode(const wxString &line, CardInfo &card)
{
    Words words(line, "= ");

    int size = words.Size();

    if (size == 4 || size == 5)
    {
        card.id = words[0];

        if (!words[1].ToULongLong(&card.number))
        {
            return false;
        }

        card.time = words[2] + " " + words[3];

        int permission = 1;

        if (size == 5)
        {
            words[4].ToInt(&permission);
        }

        card.permission = (permission != 0);

        return true;
    }

    return false;
}


void Server::Request::SaveBaseToFile::Execute()
{
    SAFE_DELETE(file_cards);

    file_cards = new wxTextFile(file_name);

    LOG_WRITE_SERVER("Save DB to file %s", file_name.c_str().AsChar());

    Server::Send::SaveBase();
}


void Server::Request::SetPermission::Execute()
{
    for (auto &item : cards_map)
    {
        if (item.second.number == number)
        {
            CardInfo &info = item.second;

            if (info.permission != permission)
            {
                info.permission = permission;

                User::AppendCard(info);
            }
        }
    }
}


void Server::Request::AppendCard::Execute()
{
    cards_map[card.id] = card;
}


void Server::Request::DeleteCardByID::Execute()
{
    cards_map.erase(id);

    User::DeleteCardByID(id);
}


void Server::Send::LoadBase(const wxString &file_name)
{
    in_queue.Push(new Request::LoadBase(file_name));
}


void Server::Send::AppendCard(const CardInfo &card)
{
    in_queue.Push(new Request::AppendCard(card));
}


void Server::Send::DeleteCardByID(const wxString &id)
{
    in_queue.Push(new Request::DeleteCardByID(id));
}


void Server::Send::SetPermission(uint64 number, bool permission)
{
    in_queue.Push(new Request::SetPermission(number, permission));
}


void Server::Send::SaveBase()
{
    in_queue.Push(new Request::SaveBase());
}


void Server::Send::SaveBaseToFile(const wxString &file_name)
{
    in_queue.Push(new Request::SaveBaseToFile(file_name));
}
