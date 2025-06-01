// 2024/10/31 16:45:33 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Panels/Dialogs/AutoMovedDialog.h"
#include "Controls/Buttons.h"


class ModeEKODialog : public AutoMovedDialog
{
public:

    static ModeEKODialog *self;

    static void Create();
    static void Delete();

    virtual int ShowModal() override;

private:

    ModeEKODialog();

    void OnEventToggleButton(wxCommandEvent &);

    void OnEventButton(wxCommandEvent &);

    CheckButton *btn_enable = nullptr;
};
