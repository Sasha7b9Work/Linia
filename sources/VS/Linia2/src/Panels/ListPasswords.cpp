// 2023/09/06 08:13:15 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/ListPasswords.h"
#include "Utils/String.h"
#include "Panels/Notebook/Notebook.h"
#include "Communicator/ComPort.h"
#include "Reader/Reader.h"
#include "Utils/Configurator.h"
#include "DataBase/DataBasePasswords.h"
#include "Utils/GlobalFunctions.h"
#include "Panels/CommonPanel.h"
#include "Settings/Settings.h"


ListPasswords *ListPasswords::self = nullptr;

wxTextCtrl *ListPasswords::text = nullptr;


ListPasswords::ListPasswords(wxFrame *parent) : wxFrame(parent, wxID_ANY, _L("Архив паролей"))
{
    text = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, { 600, 300 }, wxTE_MULTILINE | wxTE_READONLY);

    wxFont font(11, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Courier New"));
    text->SetFont(font);

    Bind(wxEVT_SIZE, &ListPasswords::OnSize, this);
    Bind(wxEVT_CLOSE_WINDOW, &ListPasswords::OnClose, this);
}

ListPasswords::~ListPasswords()
{
    self = nullptr;
}


void ListPasswords::OnSize(wxSizeEvent &)
{
//    wxPoint clientOrigin = GetClientAreaOrigin();

    wxSize clientSize = GetClientSize();

    wxSize sizeText = clientSize;

    text->SetSize(sizeText);
}


void ListPasswords::Create()
{
    if (!self)
    {
        self = new ListPasswords(nullptr);

        self->SetPosition(SET::GUI::pos_list.Get());

        self->SetClientSize(SET::GUI::size_list.Get().x, SET::GUI::size_list.Get().y);

        self->Maximize(SET::GUI::maximized_list.Get());
    }
}


void ListPasswords::AddLine(const wxString &str)
{
    if (self)
    {
        text->WriteText(str);
        text->WriteText(wxT("\n"));
    }
}


void ListPasswords::SwitchVisibility()
{
    if (self)
    {
        self->Show(!self->IsShown());
    }
}


void ListPasswords::OnClose(wxCloseEvent &)
{
    GF::FindAnyButton(CommonPanel::self, ID_BUTTON_OPEN_ARCHIVE_PASSWORDS)->SetLabel(_L("Открыть архив паролей"));

    self->Show(false);
}


void ListPasswords::History::Add(const wxString &txt)
{
    if ((history.size() == 0) ||
        (history[history.size() - 1].compare(txt) != 0))
    {
        history.push_back(txt);
        position = history.size() - 1;
    }
}


wxString ListPasswords::History::Next()
{
    if (history.size() == 0)
    {
        return "";
    }

    wxString result = history[position];

    position++;
    if (position == history.size())
    {
        position = 0;
    }

    return result;
}


wxString ListPasswords::History::Prev()
{
    if (history.size() == 0)
    {
        return "";
    }

    wxString result = history[position];

    position = (position == 0) ? (history.size() - 1) : (position - 1);

    return result;
}


bool ListPasswords::Show(bool show)
{
    if (show)
    {
        text->Clear();

        for (int i = 0; i < PasswDB::Count(); i++)
        {
            AddLine(wxString::Format("%llu", PasswDB::GetPassword(i)));
        }
    }

    return wxFrame::Show(show);
}
