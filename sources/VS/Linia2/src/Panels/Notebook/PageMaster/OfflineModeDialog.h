// 2024/01/29 14:33:50 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Panels/Dialogs/AutoMovedDialog.h"


// Автономный режим


class OfflineModeDialog : public AutoMovedDialog
{
public:

    static OfflineModeDialog *self;

    static void Create();
    static void Delete();

    virtual int ShowModal() override;

    bool CurrentSettingsEqualFactory() const;

    void ResetToFactory(bool factory);

private:

    OfflineModeDialog();

    void OnEventToggleButton(wxCommandEvent &);

    void OnEventButton(wxCommandEvent &);

    wxString file_name;

    wxButton *btnOpen = nullptr;
};
