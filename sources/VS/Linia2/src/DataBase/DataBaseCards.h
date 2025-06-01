// 2024/01/22 21:07:00 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Reader/Card.h"


namespace CardsDB
{
    void Update();

    // Добавляет новую карту из програамы
    // Если карта с таким ID уже существует, то заменяет старую запись - карта перевыпущена
    void AppendCard(const CardInfo &);

    // Читать данные карты по ID
    bool ReadCard(const wxString &id, CardInfo *);

    // Возвращает true, если есть карта с данным UID
    bool ExistUID(const UID &);

    // Возвращает true, если в базе есть карта с данным номером
    bool ExistNumber(uint64);

    // Возвращает строку, содержащую битовое поле. Строка представляет собой последовательность 16-битных символов.
    // В каждом символе закодировано 4 карты. Полубайты складываются в байты, которые идут от младшего к старшему.
    // Порядок бит в байтах стандартный. Т.е:
    //    0F       F0       FF       FF
    // 00001111 11110000 11111111 11111111
    // 76543210       98
    // num_cards - столько карт должно быть закодировано. Количество переданных карт всегда кратно 8 бОльшую сторону от
    // num_cards
    wxString GetBits(uint num_cards);

    // Добавляет новую карту из базы данных (DataBase.exe) при включении
    void LoadCardFromDataBase(const CardInfo &);

    // Столько карт в базе данных
    uint GetCountCards();

    void GenerateCards();

    void DeleteCardByID(const wxString &id);

    void LoadDataBase();

    void SaveDataBase();

    void SaveDataBaseToFile(const wxString &);

    void Clear();
};
