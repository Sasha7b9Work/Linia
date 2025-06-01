// 2023/09/06 08:13:11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class ListPasswords : public wxFrame
{
public:

    static void Create();
    virtual ~ListPasswords();

    static ListPasswords *self;

    static void SwitchVisibility();

    virtual bool Show(bool show = true) wxOVERRIDE;

private:

    static wxTextCtrl *text;

    ListPasswords(wxFrame *parent);

    void OnSize(wxSizeEvent &);
    void OnClose(wxCloseEvent &);

    static void AddLine(const wxString &line);

    class History
    {
    public:
        void Add(const wxString &text);
        wxString Next();
        wxString Prev();

    private:
        std::vector<wxString> history;
        uint position = 1;
    };

    History history;
};
