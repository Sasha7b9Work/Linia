// 2024/01/21 21:48:03 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Reader/Card.h"


/*
*   Задания, которые поступают в сервер извне
*/


namespace Server
{
    namespace Request
    {
        struct Task
        {
            virtual ~Task()
            {
            }

            // Вызывается в цикле.
            // Возвращает false, если обновлять больше не нунжно.
            virtual void Execute() = 0;
        };


        struct LoadBase : public Task
        {
            LoadBase(const wxString &_file_name) : file_name(_file_name)
            {
            }
            virtual void Execute() override;
        private:
            wxString file_name;
        };


        struct SaveBase : public Task
        {
            SaveBase()
            {
            }
            virtual void Execute() override;
        };


        struct SaveBaseToFile : public Task
        {
            SaveBaseToFile(const wxString &_file_name) : file_name(_file_name)
            {
            }
            virtual void Execute() override;
        private:
            wxString file_name;
        };


        struct SetPermission : public Task
        {
            SetPermission(uint64 _number, bool _permission) : number(_number), permission(_permission)
            {
            }
            virtual void Execute() override;
        private:
            uint64 number;
            bool permission;
        };


        struct AppendCard : public Task
        {
            AppendCard(const CardInfo &_card) : card(_card)
            {
            }
            virtual void Execute() override;
        private:
            CardInfo card;
        };


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
