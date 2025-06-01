// 2025/04/14 10:04:50 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Notebook/PageMaster/WiegandDialog.h"
#include "Controls/TextControls.h"


WiegandDialog *WiegandDialog::self = nullptr;


void WiegandDialog::Create()
{
    if (!self)
    {
        self = new WiegandDialog();
    }
}


void WiegandDialog::Delete()
{
    SAFE_DELETE(self);
}


int WiegandDialog::ShowModal()
{
    return AutoMovedDialog::ShowModal();
}


WiegandDialog::WiegandDialog() :
    AutoMovedDialog(_L("Настройки Wiegand"))
{
    //    const int width = BUTTON_SIZE_SMALL.GetWidth() * 2 + 10;

    new wxStaticText(this, wxID_ANY, "Wiegand", { 10, 13 });

    int d = 130;

    new TextCtrlNumbers(this, ID_TEXTCNTRL_WIEGAND_VALUE, 0, { 10 + 130, 10 }, { BUTTON_WIDTH - d, TEXTCNTRL_HEIGHT });

    new wxCheckBox(this, ID_CHECKBOX_WIEGAND_FULL_GUID, _L("Полный UID"), { 10, 45 });

    new wxCheckBox(this, ID_CHECKBOX_WIEGAND_CONTROL_BITS, _L("Контрольные биты"), { 10, 75 });

    new wxCheckBox(this, ID_CHECKBOX_WIEGAND_INVERSE_CONTROL_BITS, _L("Инверсия контрольных бит"), { 10, 105 });

    new wxCheckBox(this, ID_CHECKBOX_WIEGAND_REVERSE_ORDER_BITS, _L("Обратный порядок бит"), { 10, 135 });

    new wxCheckBox(this, ID_CHECKBOX_WIEGAND_NUID_DISCARD_LSB, _L("Отбрасывать младший байт в NUID"), { 10, 165 });

    int y = 195;

    new wxButton(this, wxID_CANCEL, _L_CLOSE, { 10, y }, BUTTON_SIZE_SMALL);

    new wxButton(this, wxID_OK, _L_SAVE, { 10 + BUTTON_SIZE_SMALL.GetWidth() + 10, y }, BUTTON_SIZE_SMALL);

    Bind(wxEVT_BUTTON, &WiegandDialog::OnEventButton, this);

    AutoMovedDialog::SetFixedSize({ 30 + 2 * BUTTON_SIZE_SMALL.GetWidth(), 240 });
}


void WiegandDialog::OnEventButton(wxCommandEvent &event)
{
    int id = event.GetId();

    if (id == wxID_OK)
    {
        SET::WIEGAND::value.SetFromControl();
        SET::WIEGAND::full_guid.SetFromControl();
        SET::WIEGAND::control_bits.SetFromControl();
        SET::WIEGAND::inverse_control_bits.SetFromControl();
        SET::WIEGAND::reverse_order_bits.SetFromControl();
        SET::WIEGAND::nuid_discard_LSB.SetFromControl();
    }
    else if (id == wxID_CANCEL)
    {
        SET::WIEGAND::value.LoadToControl();
        SET::WIEGAND::full_guid.LoadToControl();
        SET::WIEGAND::control_bits.LoadToControl();
        SET::WIEGAND::inverse_control_bits.LoadToControl();
        SET::WIEGAND::reverse_order_bits.LoadToControl();
        SET::WIEGAND::nuid_discard_LSB.LoadToControl();
    }

    event.Skip();
}


bool WiegandDialog::CurrentSettingsEqualFactory() const
{
    return
        SET::WIEGAND::value.ValueIsDefault() &&
        SET::WIEGAND::full_guid.ValueIsDefault() &&
        SET::WIEGAND::control_bits.ValueIsDefault() &&
        SET::WIEGAND::inverse_control_bits.ValueIsDefault() &&
        SET::WIEGAND::reverse_order_bits.ValueIsDefault() &&
        SET::WIEGAND::nuid_discard_LSB.ValueIsDefault();
}


void WiegandDialog::ResetToFactory(bool factory)
{
    SET::WIEGAND::value.ResetToFactory(factory);
    SET::WIEGAND::full_guid.ResetToFactory(factory);
    SET::WIEGAND::control_bits.ResetToFactory(factory);
    SET::WIEGAND::inverse_control_bits.ResetToFactory(factory);
    SET::WIEGAND::reverse_order_bits.ResetToFactory(factory);
    SET::WIEGAND::nuid_discard_LSB.ResetToFactory(factory);
}