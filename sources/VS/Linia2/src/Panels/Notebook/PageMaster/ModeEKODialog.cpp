// 2024/10/31 16:45:45 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Notebook/PageMaster/ModeEKODialog.h"
#include "Utils/SystemDepend.h"
#include "Settings/Settings.h"


ModeEKODialog *ModeEKODialog::self = nullptr;


void ModeEKODialog::Create()
{
    self = new ModeEKODialog();
}


void ModeEKODialog::Delete()
{
    SAFE_DELETE(self);
}


int ModeEKODialog::ShowModal()
{
    return AutoMovedDialog::ShowModal();
}


ModeEKODialog::ModeEKODialog() :
    AutoMovedDialog(_L("Режим ЭКО"))
{
    wxWindow *window = new wxWindow(this, wxID_ANY);

    const int width = BUTTON_SIZE_SMALL.GetWidth() * 2 + 10;

    btn_enable = new CheckButton(window, ID_CHECKBUTTON_MODE_EKO, _L("Вкл-Откл"), { X_0, 10 }, { width, BUTTON_SIZE_SMALL.GetHeight() });

    btn_enable->SetToolTip(SET::mode_eco.Get() ? _L("Hint eco on") : _L("Hint exo off"));

    const int y = 50;

    new wxButton(window, wxID_CANCEL, _L_CLOSE, { 10, y }, BUTTON_SIZE_SMALL);

    new wxButton(window, wxID_OK, _L_SAVE, { 10 + BUTTON_SIZE_SMALL.GetWidth() + 10, y }, BUTTON_SIZE_SMALL);

    Bind(wxEVT_TOGGLEBUTTON, &ModeEKODialog::OnEventToggleButton, this);
    Bind(wxEVT_BUTTON, &ModeEKODialog::OnEventButton, this);

    AutoMovedDialog::SetFixedSize({ 30 + 2 * BUTTON_SIZE_SMALL.GetWidth(), 100 });
}


void ModeEKODialog::OnEventToggleButton(wxCommandEvent &event)
{
    if (event.GetId() == btn_enable->GetId())
    {
        btn_enable->SetToolTip(SET::mode_eco.Get() ? _L("Hint eco on") : _L("Hint exo off"));
    }
}


void ModeEKODialog::OnEventButton(wxCommandEvent &event)
{
    int id = event.GetId();

    if (id == wxID_OK)
    {
        SET::mode_eco.SetFromControl();
    }
    else if (id == wxID_CANCEL)
    {
        SET::mode_eco.LoadToControl();
    }

    event.Skip();
}
