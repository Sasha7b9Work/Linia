// 2024/01/20 12:22:48 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Utils/Timer.h"
#include "Reader/Card.h"
#include "DataBase/DataBaseCards.h"
#include "Utils/GlobalFunctions.h"
#include "MainWindow.h"
#include "DataBase/Server/Server.h"
#include "DataBase/Server/Queue.h"
#include "DataBase/Server/TaskAnswer.h"
#include "Panels/Notebook/PageUser/PageUser.h"


namespace Server
{
    static bool is_initialized = false;     // Эта переменная нужна, чтобы при первой загрузке базы карт не выводить сообщение
    static uint need_received_cards = 0;    // Столько карт должно быть принято. Нужно только для прогресс-бара

    static Queue<Answer::Task> out_queue;   // Сюда складывается задания, которые будет выполнять вызывающий поток в функции Server::Update()
}


void Server::Update()
{
    static Answer::Task *task = nullptr;

    int counter = 0;

    while (out_queue.Size() || task)
    {
        if (task)
        {
            task->Execute();
            SAFE_DELETE(task);
        }

        task = out_queue.GetFirst();

        if (counter++ > 1000)
        {
            break;
        }
    }
}


void Server::User::BeginLoadBase(uint count_cards)
{
    out_queue.Push(new Answer::BeginLoadBase(count_cards));
}


void Server::User::AppendCard(const CardInfo &card)
{
    out_queue.Push(new Answer::AppendCard(card));
}


void Server::User::EndLoadBase()
{
    out_queue.Push(new Answer::EndLoadBase());
}


void Server::User::DeleteCardByID(const wxString &id)
{
    out_queue.Push(new Answer::DeleteCardByID(id));
}


void Server::Answer::DeleteCardByID::Execute()
{
    CardsDB::DeleteCardByID(id);
}


void Server::Answer::AppendCard::Execute()
{
    CardsDB::LoadCardFromDataBase(card);

    PageUser::self->progress.Update((float)(CardsDB::GetCountCards()) * 100.0f / (float)(need_received_cards));
}


void Server::Answer::BeginLoadBase::Execute()
{
    CardsDB::Clear();

    need_received_cards = count_cards;

    if (need_received_cards > 0)
    {
        PageUser::self->progress.ResetAndShow();
    }

#ifdef GENERATE_CARDS_IF_EMPTY

    if (need_received_cards == 0)
    {
        CardsDB::GenerateCards();
    }

#endif
}


void Server::Answer::EndLoadBase::Execute()
{
    PageUser::self->progress.Hide();

    if (is_initialized)
    {
        GF::ShowModalInfo(MainWindow::self, _L("Список карт загружен"));
    }

    is_initialized = true;
}
