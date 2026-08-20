#pragma once
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
        return m_server;
    }
    wxString GetHost() const
    {
        return m_server;
    }  // Алиас для совместимости
    int GetPort() const
    {
        return m_port;
    }
    wxString GetUsername() const
    {
        return m_username;
    }
    wxString GetPassword() const
    {
        return m_password;
    }
    void ClearPassword();

private:
    void CreateControls();
    void OnOK(wxCommandEvent &);
    void OnCancel(wxCommandEvent &);

    wxTextCtrl *serverCtrl = nullptr;
    wxTextCtrl *portCtrl = nullptr;
    wxTextCtrl *usernameCtrl = nullptr;
    wxTextCtrl *passwordCtrl = nullptr;

    wxString m_server;
    int m_port = 22;
    wxString m_username;
    wxString m_password;

    wxDECLARE_EVENT_TABLE();
};
