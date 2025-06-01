// 2023/08/23 15:56:34 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class UpgradeDialog : public wxDialog
{
public:

    UpgradeDialog();
    virtual ~UpgradeDialog() { }

    static UpgradeDialog *self;

    virtual int ShowModal() override;

private:

    void OnEventButton(wxCommandEvent &);

    void OnEventKeyHook(wxKeyEvent &);

    static wxString file_name;

    static void Upgrade(pchar);

    static bool in_progress;
};
