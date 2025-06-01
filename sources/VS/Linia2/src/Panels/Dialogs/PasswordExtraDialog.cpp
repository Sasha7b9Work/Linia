// 2025/04/02 14:03:03 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Dialogs/PasswordExtraDialog.h"
#include "Utils/SystemDepend.h"
#include "Settings/Settings.h"


PasswordExtraDialog *PasswordExtraDialog::self = nullptr;


void PasswordExtraDialog::Create()
{
    if (!self)
    {
        self = new PasswordExtraDialog();
    }
}


void PasswordExtraDialog::Delete()
{
    SAFE_DELETE(self);
}


PasswordExtraDialog::PasswordExtraDialog() :
    AutoMovedDialog(_L("Дополнительные параметры"))
{
    wxWindow *window = new wxWindow(this, wxID_ANY);

    new wxCheckBox(window, ID_CHECKBOX_DISABLE_BELOW_LEVEL_SL3, _L("Отключить карты ниже SL3"), { X_0, Y_0 - 10 });

    GF::FindCheckBox(this, ID_CHECKBOX_DISABLE_BELOW_LEVEL_SL3)->SetToolTip(_L("Hint disable less SL3"));

    int y = Y_1 - 10;

    new wxButton(window, wxID_CANCEL, _L_CLOSE, { 10, y  }, BUTTON_SIZE_SMALL);

    new wxButton(window, wxID_OK, _L_SAVE, { 10 + BUTTON_SIZE_SMALL.GetWidth() + 10, y }, BUTTON_SIZE_SMALL);

    Bind(wxEVT_BUTTON, &PasswordExtraDialog::OnEventButton, this);

    SetFixedSize({ 30 + 2 * BUTTON_SIZE_SMALL.x, 100 });
}


int PasswordExtraDialog::ShowModal()
{
    SET::MISC::disable_less_SL3.LoadToControl();

    return AutoMovedDialog::ShowModal();
}


void PasswordExtraDialog::OnEventButton(wxCommandEvent &event)
{
    if (event.GetId() == wxID_OK)
    {
        SET::MISC::disable_less_SL3.SetFromControl();
    }

    event.Skip();
}
