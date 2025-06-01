// 2024/01/09 15:55:28 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Reader/Card.h"


class ListCards : public wxListView
{
public:

    ListCards(wxWindow *parent, wxWindowID, const wxPoint &, const wxSize &, long);

    static ListCards *self;

    void SetWidthColumn(int col, int width);

    // Добавляет информацию о карте. Если карта с таким ID уже есть, то перезаписывается информация о ней.
    void SetCard(const CardInfo &);

    void DeleteCard(const wxString &id);

    void Clear();

private:

    void OnEventKeyHook(wxKeyEvent &);

    // По шапке
    void OnEventColumn(wxListEvent &);

    // По строке
    void OnEventRigthClick(wxListEvent &);

    // Пункт из выпдающего меню на списке карт
    void OnEventMenu(wxCommandEvent &);

    uint ReadUInt(int line, int col);

    // Возвращает индекс итема для карты c данным id
    long GetItemForCard(const wxString &id);

    bool ReadCard(long, CardInfo *);

    void SelectAllItems();
};
