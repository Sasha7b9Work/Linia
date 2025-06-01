// 2024/01/29 16:16:08 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Panels/Dialogs/AutoMovedDialog.h"


class OSDPModeDialog : public AutoMovedDialog
{
public:

    static OSDPModeDialog *self;

    static void Create();
    static void Delete();

    virtual int ShowModal() override;

    bool CurrentSettingsEqualFactory() const;

    void ResetToFactory(bool factory);

private:

    OSDPModeDialog();

    void OnEventButton(wxCommandEvent &);

    void OnEventToggleButton(wxCommandEvent &);

    void OnEventCheckBox(wxCommandEvent &);
};
