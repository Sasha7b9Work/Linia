// 2023/08/11 11:28:48 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Reader/Reader.h"


class ReaderInfoDialog : public wxDialog
{
public:

    ReaderInfoDialog(const ReaderInfo &);
    virtual ~ReaderInfoDialog() { }

    static wxString typeDevice;
    static wxString idDevice;
    static wxString versionFirmware;
    static wxString idFirmware;

private:

    void OnEventButton(wxCommandEvent &);

    void OnEventKeyHook(wxKeyEvent &);

    wxBoxSizer *CreateColumn(const wxArrayString &);
};
