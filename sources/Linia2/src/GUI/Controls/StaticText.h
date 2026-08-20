// 2026/08/19 11:21:02 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once
#pragma warning(push, 0)
    #include <wx/stattext.h>
#pragma warning(pop)


class StaticText : public wxStaticText
{
public:

    StaticText(wxWindow *parent, const wxString &label, const wxSize &size = wxDefaultSize) :
        wxStaticText(parent, wxID_ANY, label, wxDefaultPosition, size)
    {

    }
};
