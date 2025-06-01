// 2024/01/09 15:55:32 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Utils/StringUtils.h"
#include "Utils/Configurator.h"
#include "Panels/CommonPanel.h"
#include "DataBase/DataBaseCards.h"
#include "Settings/Settings.h"
#include "Panels/Notebook/PageUser/ListCards.h"
#include "DataBase/Server/Server.h"
#include <algorithm>
#include <cstdlib>


ListCards *ListCards::self = nullptr;


ListCards::ListCards(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size, long style) :
    wxListView(parent, id, pos, size, style)
{
    self = this;

    AppendColumn(_L("№ карты"), wxLIST_FORMAT_LEFT, WIDTH_BOX / 3);
    AppendColumn(_L("ID карты"), wxLIST_FORMAT_LEFT, WIDTH_BOX / 3);
    AppendColumn(_L("Дата выдачи"), wxLIST_FORMAT_LEFT, WIDTH_BOX / 3 + 1);

    Bind(wxEVT_LIST_COL_CLICK, &ListCards::OnEventColumn, this);
    Bind(wxEVT_LIST_ITEM_RIGHT_CLICK, &ListCards::OnEventRigthClick, this);
    Bind(wxEVT_CHAR_HOOK, &ListCards::OnEventKeyHook, this);

    SetBackgroundColour(parent->GetBackgroundColour());

    SetOwnBackgroundColour(parent->GetBackgroundColour());
}


void ListCards::SetWidthColumn(int col, int width)
{
    wxListItem item;
    GetColumn(col, item);
    item.SetWidth(width);
    SetColumn(col, item);
}


void ListCards::OnEventRigthClick(wxListEvent& event)
{
    wxMenu menu;

    if (SET::OFFLINE::enabled.Get())
    {
        bool need_red = false;          // true, если нужен пункт "Запретить"
        bool need_green = false;        // true, если нужен пункт "Разрешить"

        long line = GetFirstSelected();

        while (line >= 0 && (!need_red || !need_green))
        {
            CardInfo info;

            if (ReadCard(line, &info))
            {
                if (info.permission)
                {
                    need_red = true;
                }
                else
                {
                    need_green = true;
                }
            }

            line = GetNextSelected(line);
        }

        if (need_red)
        {
            menu.Append(ID_MENU_LIST_CARD_MARK_AS_RED, _L("Запретить"));
            Connect(ID_MENU_LIST_CARD_MARK_AS_RED, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(ListCards::OnEventMenu));
        }

        if (need_green)
        {
            menu.Append(ID_MENU_LIST_CARD_MARK_AS_GREEN, _L("Разрешить"));
            Connect(ID_MENU_LIST_CARD_MARK_AS_GREEN, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(ListCards::OnEventMenu));
        }

        //    menu.AppendSeparator();

        //    menu.Append(ID_MENU_LIST_CARD_SELECT_ALL, _L("Выделить все"));
        //    Connect(ID_MENU_LIST_CARD_SELECT_ALL, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(ListCards::OnEventMenu));

        //    menu.AppendSeparator();

        //    menu.Append(ID_MENU_LIST_CARD_MARK_AS_GREEN, wxString::Format("Line %d", GetFocusedItem()));

        //    menu.AppendSeparator();
        //
        //    menu.Append(wxID_ANY, cardsDB->GetBits(600));
    }

    menu.Append(ID_MENU_LIST_CARD_DELETE, _L("Удалить"));
    Connect(ID_MENU_LIST_CARD_DELETE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(ListCards::OnEventMenu));

    wxPoint pos_mouse = event.GetPoint();

    PopupMenu(&menu, pos_mouse.x, pos_mouse.y);
}


void ListCards::OnEventMenu(wxCommandEvent &event)
{
    if (event.GetId() == ID_MENU_LIST_CARD_SELECT_ALL)
    {
        SelectAllItems();
    }
    else if(event.GetId() == ID_MENU_LIST_CARD_MARK_AS_GREEN || event.GetId() == ID_MENU_LIST_CARD_MARK_AS_RED)
    {
        std::map<uint64, int> numbers;

        long line = GetFirstSelected();

        bool permission = (event.GetId() == ID_MENU_LIST_CARD_MARK_AS_GREEN);

        while (line >= 0)
        {
            CardInfo info;

            if (ReadCard(line, &info))
            {
                auto exists = numbers.find(info.number);

                if (exists == numbers.end())
                {
                    numbers[info.number] = 1;

                    Server::Send::SetPermission(info.number, permission);
                }
            }

            line = GetNextSelected(line);
        }
    }
    else if (event.GetId() == ID_MENU_LIST_CARD_DELETE)
    {
        long line = GetFirstSelected();

        while (line >= 0)
        {
            CardInfo info;

            if (ReadCard(line, &info))
            {
                Server::Send::DeleteCardByID(info.id);
            }

            line = GetNextSelected(line);
        }
    }
}


static int wxCALLBACK CompareByNumbers(wxIntPtr item1, wxIntPtr item2, wxIntPtr order)
{
    if (item1 == item2)
    {
        return 0;
    }

    int result = (bool)order ? 1 : -1;

    return (item1 < item2) ? -result : result;
}


void ListCards::OnEventColumn(wxListEvent &event)
{
    static bool order = true;

    order = !order;

    if (event.GetColumn() == 0)                             // Сортировка по номерам
    {
        for (int i = 0; i < GetItemCount(); i++)
        {
            uint value = ReadUInt(i, 0);
            SetItemData(i, (int)value);
        }
    }
    else if (event.GetColumn() == 1)                        // Сортировка по ID
    {
        struct Struct
        {
            uint64 value;
            int index;
        };

        std::vector<Struct> vector;

        for (int i = 0; i < GetItemCount(); i++)
        {
            uint64 value = 0;

            GetItemText(i, 1).ToULongLong(&value, 16);

            vector.emplace_back(Struct{ value, i });
        }

        std::sort(vector.begin(), vector.end(),
            [](Struct &a, Struct &b)
            {
                return a.value < b.value;
            });

        for (int i = 0; i < (int)vector.size(); i++)
        {
            SetItemData(vector[(uint)i].index, i);
        }
    }
    else if (event.GetColumn() == 2)                        // Сортировка по дате
    {
        for (uint i = 0; i < (uint)GetItemCount(); i++)
        {
            wxString item = GetItemText((int)i, 2);

            Words words(item, " :/");

            int month = 0;
            int day = 0;
            int year = 0;
            int hour = 0;
            int minutes = 0;
            int secs = 0;

            wxString(words.At(0).c_str()).ToInt(&month);
            wxString(words.At(1).c_str()).ToInt(&day);
            wxString(words.At(2).c_str()).ToInt(&year);
            wxString(words.At(3).c_str()).ToInt(&hour);
            wxString(words.At(4).c_str()).ToInt(&minutes);
            wxString(words.At(5).c_str()).ToInt(&secs);

            wxDateTime date_time(
                (wxDateTime::wxDateTime_t)day,
                (wxDateTime::Month)(month - 1),
                year + 2000,
                (wxDateTime::wxDateTime_t)hour,
                (wxDateTime::wxDateTime_t)minutes,
                (wxDateTime::wxDateTime_t)secs
            );

            int value = (int)date_time.GetTicks();
            SetItemData((int)i, value);
        }
    }

    SortItems(CompareByNumbers, order);
}


uint ListCards::ReadUInt(int _line, int col)
{
    uint result = 0;

    GetItemText(_line, col).ToUInt(&result);

    return result;
}


void ListCards::SetCard(const CardInfo &card)
{
    long line = GetItemForCard(card.id);

    if (line < 0)
    {
        line = GetItemCount();

        InsertItem(line, "");
    }

    wxListItem item;
    item.SetId(line);
    item.SetColumn(0);
    if (SET::OFFLINE::enabled.Get())
    {
        item.SetBackgroundColour(card.permission ? wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW) : wxColor(255, 64, 64));
        item.SetTextColour(card.permission ? *wxBLACK : wxColor(255, 255, 255));
    }
    else
    {
        item.SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW));
        item.SetTextColour(*wxBLACK);
    }
    item.SetText(wxString::Format("%llu", card.number));

    SetItem(item);
    SetItem(line, 1, card.id);
    SetItem(line, 2, card.time);
}


void ListCards::DeleteCard(const wxString& id)
{
    long line = GetItemForCard(id);

    if (line >= 0)
    {
        DeleteItem(line);
    }
}


long ListCards::GetItemForCard(const wxString &id)
{
    long count = GetItemCount();

    for (long i = 0; i < count; i++)
    {
        if (id == GetItemText(i, 1))
        {
            return i;
        }
    }

    return -1;
}


bool ListCards::ReadCard(long line, CardInfo *info)
{
    if (line >= 0 && line < GetItemCount())
    {
        wxString id = GetItemText(line, 1);

        CardsDB::ReadCard(id, info);

        return true;
    }
    else
    {
        LOG_ERROR("Incorrect number line : %ld", line);
    }

    return false;
}


void ListCards::OnEventKeyHook(wxKeyEvent &event)
{
    (void)event;

//    if (event.GetKeyCode() == 'A')
//    {
//        if (event.ControlDown())
//        {
//            SelectAllItems();
//        }
//    }

    event.Skip();
}


void ListCards::SelectAllItems()
{
    long count = GetItemCount();

    for (long i = 0; i < count; i++)
    {
        Select(i);
    }
}


void ListCards::Clear()
{
    DeleteAllItems();
}
