// 2025/04/02 14:02:35 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Panels/Dialogs/AutoMovedDialog.h"


// Пароль - Дополнительно


class PasswordExtraDialog : public AutoMovedDialog
{
public:

    static PasswordExtraDialog *self;

    static void Create();
    static void Delete();

    virtual int ShowModal() override;

private:

    PasswordExtraDialog();

    void OnEventButton(wxCommandEvent &);
};
