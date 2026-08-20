#pragma once
#include "GUI/Controls/Button.h"
#pragma warning(push, 0)
    #include <wx/dialog.h>
    #include <wx/textctrl.h>
#pragma warning(pop)


class FTPConnectionDialog : public wxDialog
{
public:

    FTPConnectionDialog(wxWindow *);
    ~FTPConnectionDialog();

    wxString GetServer() const
    {
        return server;
    }
    wxString GetHost() const
    {
        return server;
    }  // Алиас для совместимости
    int GetPort() const
    {
        return port;
    }
    wxString GetUsername() const
    {
        return username;
    }
    wxString GetPassword() const
    {
        return password;
    }
    void ClearPassword();

private:

    void CreateControls();

    wxTextCtrl *txtServer = nullptr;
    wxTextCtrl *txtPort = nullptr;
    wxTextCtrl *txtUserName = nullptr;
    wxTextCtrl *txtPassword = nullptr;

    Button *btnOK = nullptr;
    Button *btnCancel = nullptr;

    wxString server;
    int port = 22;
    wxString username;
    wxString password;
};
