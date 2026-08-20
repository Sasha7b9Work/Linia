#include "defines.h"
#include "GUI/PageSettings/MyComputer/FTPConnectionDialog.h"
#pragma warning(push, 0)
    #include <wx/sizer.h>
    #include <wx/stattext.h>
    #include <wx/button.h>
    #include <wx/msgdlg.h>
#pragma warning(pop)


wxBEGIN_EVENT_TABLE(FTPConnectionDialog, wxDialog)
EVT_BUTTON(wxID_OK, FTPConnectionDialog::OnOK)
EVT_BUTTON(wxID_CANCEL, FTPConnectionDialog::OnCancel)
wxEND_EVENT_TABLE()

// Безопасная очистка строки — перезаписывает содержимое нулями
static void SecureClearString(wxString &str)
{
    if (!str.IsEmpty())
    {
        // Перезаписываем каждый символ
        for (size_t i = 0; i < str.length(); ++i)
        {
            str[i] = '\0';
        }
        str.Clear();
    }
}

FTPConnectionDialog::FTPConnectionDialog(wxWindow *parent) :
    wxDialog(parent, wxID_ANY, "Подключение к FTP серверу", wxDefaultPosition, wxSize(400, 300))
{
    CreateControls();
    Center();
}

FTPConnectionDialog::~FTPConnectionDialog()
{
    ClearPassword();
}

void FTPConnectionDialog::ClearPassword()
{
    SecureClearString(password);
}

void FTPConnectionDialog::CreateControls()
{
    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

    // Сервер
    wxBoxSizer *serverSizer = new wxBoxSizer(wxHORIZONTAL);
    serverSizer->Add(new wxStaticText(this, wxID_ANY, "Сервер:", wxDefaultPosition, wxSize(100, -1)), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    serverCtrl = new wxTextCtrl(this, wxID_ANY, "");
    serverSizer->Add(serverCtrl, 1, wxEXPAND | wxALL, 5);
    mainSizer->Add(serverSizer, 0, wxEXPAND | wxALL, 5);

    // Порт (по умолчанию 22 для FTP)
    wxBoxSizer *portSizer = new wxBoxSizer(wxHORIZONTAL);
    portSizer->Add(new wxStaticText(this, wxID_ANY, "Порт:", wxDefaultPosition, wxSize(100, -1)), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    portCtrl = new wxTextCtrl(this, wxID_ANY, "22");
    portSizer->Add(portCtrl, 1, wxEXPAND | wxALL, 5);
    mainSizer->Add(portSizer, 0, wxEXPAND | wxALL, 5);

    // Имя пользователя
    wxBoxSizer *userSizer = new wxBoxSizer(wxHORIZONTAL);
    userSizer->Add(new wxStaticText(this, wxID_ANY, "Пользователь:", wxDefaultPosition, wxSize(100, -1)), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    usernameCtrl = new wxTextCtrl(this, wxID_ANY, "");
    userSizer->Add(usernameCtrl, 1, wxEXPAND | wxALL, 5);
    mainSizer->Add(userSizer, 0, wxEXPAND | wxALL, 5);

    // Пароль
    wxBoxSizer *passSizer = new wxBoxSizer(wxHORIZONTAL);
    passSizer->Add(new wxStaticText(this, wxID_ANY, "Пароль:", wxDefaultPosition, wxSize(100, -1)), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    passwordCtrl = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD);
    passSizer->Add(passwordCtrl, 1, wxEXPAND | wxALL, 5);
    mainSizer->Add(passSizer, 0, wxEXPAND | wxALL, 5);

    // Разделитель
    mainSizer->AddSpacer(10);

    // Кнопки
    wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    buttonSizer->AddStretchSpacer(1);
    buttonSizer->Add(new wxButton(this, wxID_OK, "Подключить"), 0, wxALL, 5);
    buttonSizer->Add(new wxButton(this, wxID_CANCEL, "Отмена"), 0, wxALL, 5);
    mainSizer->Add(buttonSizer, 0, wxEXPAND | wxALL, 10);

    SetSizer(mainSizer);
    mainSizer->Fit(this);
    mainSizer->SetSizeHints(this);
}

void FTPConnectionDialog::OnOK(wxCommandEvent &)
{
    server = serverCtrl->GetValue();
    wxString portStr = portCtrl->GetValue();
    long portLong;
    if (portStr.ToLong(&portLong) && portLong >= 1 && portLong <= 65535)
    {
        port = static_cast<int>(portLong);
    }
    else
    {
        wxMessageBox("Порт должен быть числом от 1 до 65535", "Ошибка", wxOK | wxICON_ERROR);
        return;
    }
    username = usernameCtrl->GetValue();
    password = passwordCtrl->GetValue();

    if (server.IsEmpty())
    {
        wxMessageBox("Введите адрес сервера", "Ошибка", wxOK | wxICON_ERROR);
        return;
    }

    if (username.IsEmpty())
    {
        wxMessageBox("Введите имя пользователя", "Ошибка", wxOK | wxICON_ERROR);
        return;
    }

    EndModal(wxID_OK);
}

void FTPConnectionDialog::OnCancel(wxCommandEvent &)
{
    EndModal(wxID_CANCEL);
}
