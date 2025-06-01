// 2024/10/31 12:06:34 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Panels/Dialogs/AutoMovedDialog.h"


class ExtendedSettingsDialog : public AutoMovedDialog
{
public:

    static ExtendedSettingsDialog *self;

    static void Create();
    static void Delete();

    virtual int ShowModal() override;

    bool CurrentSettingsEqualFactory() const;

    void ResetToFactory(bool factory);

private:

    ExtendedSettingsDialog();

//    void OnEventToggleButton(wxCommandEvent &);

    void OnEventButton(wxCommandEvent &);
};
