// 2024/10/31 12:06:40 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Notebook/PageMaster/ExtendedSettingsDialog.h"
#include "Settings/Settings.h"
#include "Controls/TextControls.h"
#include "Utils/SystemDepend.h"
#include "Panels/Notebook/PageMaster/WiegandDialog.h"


ExtendedSettingsDialog *ExtendedSettingsDialog::self = nullptr;


void ExtendedSettingsDialog::Create()
{
    self = new ExtendedSettingsDialog();
}


void ExtendedSettingsDialog::Delete()
{
    SAFE_DELETE(self);
}


int ExtendedSettingsDialog::ShowModal()
{
    SET::EXT::enabled.SendEventToGUI();

    return AutoMovedDialog::ShowModal();
}


ExtendedSettingsDialog::ExtendedSettingsDialog() :
    AutoMovedDialog(_("Расширенные настройки"))
{
    const int width = BUTTON_SIZE_SMALL.GetWidth() * 2 + 10;

    wxWindow *window = new wxWindow(this, wxID_ANY);

    new wxButton(window, ID_BUTTON_WIEGAND, "Wiegand", { X_0, Y_0 - 15 }, BUTTON_SIZE);

    new CheckButton(window, ID_CHECKBUTTON_EXT_SET_ENABLE, _L("Вкл-Откл"), { X_0, Y_1 - 15 }, { width, BUTTON_SIZE.y });

    new wxCheckBox(window, ID_CHECKBOX_EXT_PARITY, _L("Паритет (+2 бит)"), { X_0, Y_2 - 10 });

    new wxCheckBox(window, ID_CHECKBOX_EXT_INVERSION_CODE, _L("Инверсия выдачи кода"), { X_0, Y_2 + 20 });

    new wxCheckBox(window, ID_CHECKBOX_EXT_CONTROL_BIT, _L("Контрольный бит"), { X_0, Y_3 + 10 });

    new wxStaticText(window, ID_STATICTEXT_MODE_READ_CARD, _L("Режим чтения карты"), { X_0, Y_4 });

    wxArrayString choices;
    choices.Add(_L("С автоповтором"));
    choices.Add(_L("Однократный"));

    new wxComboBox(window, ID_COMBOBOX_EXT_MODE_READ_CARD, choices[(uint)SET::EXT::mode_read_card.GetIndex()], { 10, 205 }, BUTTON_SIZE, choices, wxCB_READONLY);

    new wxStaticText(window, ID_STATICTEXT_PERIOD_AUTOREPEAT, _L("Период автоповтора, мс"), { 10, 235 });

    new TextCtrlNumbers(window, ID_TEXTCNTRL_EXT_PERIOD_AUTOREPEAT, SET::EXT::period_autorepeat.Get(), { 10, 255 }, { 50, TEXTCNTRL_HEIGHT });

    AutoMovedDialog::SetFixedSize({ 30 + 2 * BUTTON_SIZE_SMALL.x, 300 });

    Bind(wxEVT_BUTTON, &ExtendedSettingsDialog::OnEventButton, this);
}


void ExtendedSettingsDialog::OnEventButton(wxCommandEvent &event)
{
    int id = event.GetId();

    if (id == ID_BUTTON_WIEGAND)
    {
        WiegandDialog::self->ShowModal();
    }
}
