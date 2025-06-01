// 2024/08/29 16:59:15 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Reader/Card.h"


// Сервер создаётся в отдельном потоке и занимается чтением/сохранением баз данных
namespace Server
{
    // Вызывается при старте приложения.
    // Функция создаёт поток, в котором выполняются все действия с файлами базы данных
    void Create();

    // Вызывается из глвного цикла основного потока программы.
    void Update();

    // Вызывается при завершении приложения.
    // Возвращает управление, когда все внутренние операции завершены
    void Destroy();

    // Указания серверу базы данных
    namespace Send
    {
        void LoadBase(const wxString &file_name);

        void AppendCard(const CardInfo &);

        void SaveBase();

        void SetPermission(uint64 number, bool permission);

        void DeleteCardByID(const wxString &id);

        // Сохраняет текущую базу в новый файл. Новый файл становится текущим
        void SaveBaseToFile(const wxString &file_name);
    }

    // Реализацию этих функций делает пользователь сервера. Через них возвращаются результаты работы запросов
    namespace User
    {
        // Сервер вызывает эту функцию для передачи количества карт
        void BeginLoadBase(uint count_cards);

        void AppendCard(const CardInfo &);

        void EndLoadBase();

        void DeleteCardByID(const wxString &id);
    }
}
