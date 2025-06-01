// 2024/03/06 15:40:37 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Panels/Dialogs/AutoMovedDialog.h"


// Из этого окна делаем разные загрузки

class LoadImportConfigDialog : public AutoMovedDialog
{
public:
    LoadImportConfigDialog();
    virtual ~LoadImportConfigDialog() { }

private:

    void OnEventButton(wxCommandEvent &);
};
