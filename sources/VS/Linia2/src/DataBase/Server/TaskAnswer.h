// 2024/08/30 12:02:57 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


/*
*   Задания, которые сервер генерирует во время выполнения СВОЕГО потока.
*   Они будут обработаны во время работы функции Server::Update()
*/


namespace Server
{
    namespace Answer
    {
        struct Task
        {
            virtual ~Task()
            {
            }

            // Вызывается в цикле.
            // Возвращает false, если обновлять больше не нужно.
            virtual void Execute() = 0;
        };


        // Передаётся в начале загрузки новой базы
        struct BeginLoadBase : public Task
        {
            BeginLoadBase(uint _count_cards) : count_cards(_count_cards)
            {
            }
            virtual void Execute() override;
        private:
            uint count_cards;
        };


        // Добавление новой карты в базу
        struct AppendCard : public Task
        {
            AppendCard(const CardInfo &_card) : card(_card)
            {
            }
            virtual void Execute() override;
        private:
            CardInfo card;
        };


        // Вызывается после загрузки всех карт из базы
        struct EndLoadBase : public Task
        {
            EndLoadBase()
            {
            }
            virtual void Execute() override;
        };


        // Удаление карты с заданным ID
        struct DeleteCardByID : public Task
        {
            DeleteCardByID(const wxString &_id) : id(_id)
            {
            }
            virtual void Execute() override;
        private:
            wxString id;
        };
    }
}
