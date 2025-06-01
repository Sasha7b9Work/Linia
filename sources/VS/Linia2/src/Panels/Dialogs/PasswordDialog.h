// 2023/08/09 11:05:46 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Settings/Settings.h"
#include "Controls/TextControls.h"
#include "Panels/Dialogs/AutoMovedDialog.h"


class PasswordDialog : public AutoMovedDialog
{
public:

    struct Type
    {
        enum E
        {
            TwoFields,
            ThreeFields
        };
    };

    // warning - признак того, что диалог нужно обозначать предупреждающим красным цветом
    PasswordDialog(Password *, const wxString &title, bool warning = false, Type::E  = Type::TwoFields);

    virtual ~PasswordDialog() {}

    void OnMouseEye(bool opened);

private:

    bool factory_password_view = false;

    Password *password = nullptr;

    TextCtrlPassword *txt_password_factory = nullptr;

    TextCtrlPassword *txt_password_one = nullptr;

    TextCtrlPassword *txt_password_two = nullptr;

    void OnEventButton(wxCommandEvent &);

    void OnEventTextPaste(wxCommandEvent &);
};
