// 2025/04/14 10:05:07 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Panels/Dialogs/AutoMovedDialog.h"


class WiegandDialog : public AutoMovedDialog
{
public:

    static WiegandDialog *self;

    static void Create();
    static void Delete();

    virtual int ShowModal() override;

    bool CurrentSettingsEqualFactory() const;

    void ResetToFactory(bool factory);

private:

    WiegandDialog();

    void OnEventButton(wxCommandEvent &);
};
