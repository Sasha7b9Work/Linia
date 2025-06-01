// 2024/01/29 16:15:55 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Utils/GlobalFunctions.h"
#include "Settings/Settings.h"
#include "Controls/TextControls.h"
#include "Panels/Notebook/PageMaster/OSDPModeDialog.h"


OSDPModeDialog *OSDPModeDialog::self = nullptr;


void OSDPModeDialog::Create()
{
    self = new OSDPModeDialog();
}


void OSDPModeDialog::Delete()
{
    SAFE_DELETE(self);
}


int OSDPModeDialog::ShowModal()
{
    SET::OSDP::enabled.SendEventToGUI();

    return AutoMovedDialog::ShowModal();
}


OSDPModeDialog::OSDPModeDialog() :
    AutoMovedDialog(_L("Режим OSDP"))
{
    const int width = BUTTON_SIZE_SMALL.GetWidth() * 2 + 10;

    new CheckButton(this, ID_CHECKBUTTON_OSDP_ENABLED, _L("Вкл-Откл"), { 10, 10 }, { width, BUTTON_SIZE_SMALL.GetHeight() });

    int y = 50;

    new TextCtrlNumbersLimits(this, ID_TEXTCNTRL_OSDP_ADDRESS, SET::OSDP::address.Get(), 1, 127, {10, y}, {60, TEXTCNTRL_HEIGHT});

    y += 2;

    new wxStaticText(this, ID_STATICTEXT_ADDRESS_OSDP, _L("Адрес"), { 100, y });

    wxArrayString baudrates;

    baudrates.Add("9600");
    baudrates.Add("19200");
    baudrates.Add("38400");

    y += 38;

    (new wxComboBox(this, ID_COMBOBOX_OSDP_BAUDRATE, baudrates[(uint)SET::OSDP::baudrate.GetIndex()], { 10, y }, { 60, TEXTCNTRL_HEIGHT }, baudrates, wxCB_READONLY))->SetSelection(SET::OSDP::baudrate.GetIndex());

    y += 2;

    new wxStaticText(this, ID_STATICTEXT_BAUDRATE_OSDP, _L("Скорость"), { 100, y });

    new TextCtrlPassword(this, ID_TEXTCNTRL_KEY_CRYPTO_OSDP, { 10, y + 40 });

    FindWindow(ID_TEXTCNTRL_KEY_CRYPTO_OSDP)->SetToolTip(_L("Hint key crypto"));

    new wxCheckBox(this, ID_CHECKBOX_OSDP_ENCRYPTION, _L("Шифрование"), { 120, y + 43 });

    y = 175;

    new wxButton(this, wxID_CANCEL, _L_CLOSE, { 10, y }, BUTTON_SIZE_SMALL);

    new wxButton(this, wxID_OK, _L_SAVE, { 10 + BUTTON_SIZE_SMALL.GetWidth() + 10, y }, BUTTON_SIZE_SMALL);

    Bind(wxEVT_BUTTON, &OSDPModeDialog::OnEventButton, this);
    Bind(wxEVT_TOGGLEBUTTON, &OSDPModeDialog::OnEventToggleButton, this);
    Bind(wxEVT_CHECKBOX, &OSDPModeDialog::OnEventCheckBox, this);

    SetFixedSize({ 30 + 2 * BUTTON_SIZE_SMALL.GetWidth(), 220 });
}


void OSDPModeDialog::OnEventButton(wxCommandEvent &event)
{
    if (event.GetId() == wxID_OK)
    {
        SET::OSDP::enabled.SetFromControl();
        SET::OSDP::baudrate.SetFromControl();
        SET::OSDP::address.SetFromControl();
        SET::OSDP::crypto_enabled.SetFromControl();
    }
    else if (event.GetId() == wxID_CANCEL)
    {
        SET::OSDP::enabled.LoadToControl();
        SET::OSDP::baudrate.LoadToControl();
        SET::OSDP::address.LoadToControl();
        SET::OSDP::crypto_enabled.LoadToControl();
    }

    event.Skip();
}


void OSDPModeDialog::OnEventCheckBox(wxCommandEvent &event)
{
    int id = event.GetId();
    bool checked = event.IsChecked();

    if (id == ID_CHECKBOX_OSDP_ENCRYPTION)
    {
        FindWindow(ID_TEXTCNTRL_KEY_CRYPTO_OSDP)->Enable(checked);
    }
}


void OSDPModeDialog::OnEventToggleButton(wxCommandEvent &event)
{
    if (event.GetId() == ID_CHECKBUTTON_OSDP_ENABLED)
    {
        bool enabled = event.IsChecked();

        FindWindow(ID_TEXTCNTRL_OSDP_ADDRESS)->Enable(enabled);
        FindWindow(ID_COMBOBOX_OSDP_BAUDRATE)->Enable(enabled);
        FindWindow(ID_STATICTEXT_ADDRESS_OSDP)->Enable(enabled);
        FindWindow(ID_STATICTEXT_BAUDRATE_OSDP)->Enable(enabled);
        FindWindow(ID_TEXTCNTRL_KEY_CRYPTO_OSDP)->Enable(enabled);
        FindWindow(ID_CHECKBOX_OSDP_ENCRYPTION)->Enable(enabled);
    }
}


bool OSDPModeDialog::CurrentSettingsEqualFactory() const
{
    return
        SET::OSDP::enabled.ValueIsDefault() &&
        SET::OSDP::baudrate.ValueIsDefault() &&
        SET::OSDP::address.ValueIsDefault() &&
        SET::OSDP::crypto_enabled.ValueIsDefault();
}


void OSDPModeDialog::ResetToFactory(bool factory)
{
    SET::OSDP::enabled.ResetToFactory(factory);
    SET::OSDP::baudrate.ResetToFactory(factory);
    SET::OSDP::address.ResetToFactory(factory);
    SET::OSDP::crypto_enabled.ResetToFactory(factory);
}
