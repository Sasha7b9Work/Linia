// 2024/03/07 08:44:07 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Panels/Dialogs/AutoMovedDialog.h"


// Из этого окна делаем разные сохранения

class SaveExportDialog : public AutoMovedDialog
{
public:
    SaveExportDialog();
    virtual ~SaveExportDialog() { }

private:

    void OnEventButton(wxCommandEvent &);
};
