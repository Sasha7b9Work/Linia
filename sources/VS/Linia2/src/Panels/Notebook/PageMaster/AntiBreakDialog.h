// 2024/05/28 11:58:10 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Panels/Dialogs/AutoMovedDialog.h"


// Датчик отрыва


class AntiBreakDialog : public AutoMovedDialog
{
public:

    static AntiBreakDialog *self;

    static void Create();
    static void Delete();

    virtual int ShowModal() override;

    bool CurrentSettingsEqualFactory() const;

    void ResetToFactory(bool factory);

private:

    AntiBreakDialog();

    void OnEventToggleButton(wxCommandEvent &);

    void OnEventButton(wxCommandEvent &);

    void OnEventComboBox(wxCommandEvent &);

    void OnEventText(wxCommandEvent &);
};
