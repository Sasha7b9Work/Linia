// 2023/11/16 15:48:20 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Panels/Dialogs/AutoMovedDialog.h"


class ChooseSoundsDialog : public AutoMovedDialog
{
public:

    static ChooseSoundsDialog *self;

    static void Create();
    static void Delete();

    virtual int ShowModal() override;

    bool CurrentSettingsEqualFactory() const;

    void ResetToFactory(bool factory);

private:

    ChooseSoundsDialog();

    void CreateMelody(int x, int y, int number);

    void OnEventComboBox(wxCommandEvent &);

    void OnEventSpinCtrl(wxCommandEvent &);

    void OnEventButton(wxCommandEvent &);

    void Play(int number);
};
