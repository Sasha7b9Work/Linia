// 2025/8/20 19:59:36 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class StaticBox : public wxStaticBox
{
public:
    StaticBox(wxWindow *parent, const wxString &title, const wxPoint &pos, const wxSize &size) :
        wxStaticBox(parent, wxID_ANY, title, pos, size, wxBORDER_NONE)
    {

    }
};
