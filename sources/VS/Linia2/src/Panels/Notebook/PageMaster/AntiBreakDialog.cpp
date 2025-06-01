// 2024/05/28 11:58:30 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Controls/TextControls.h"
#include "Settings/Settings.h"
#include "Panels/Notebook/PageMaster/AntiBreakDialog.h"


AntiBreakDialog *AntiBreakDialog::self = nullptr;


void AntiBreakDialog::Create()
{
    self = new AntiBreakDialog();
}


void AntiBreakDialog::Delete()
{
    SAFE_DELETE(self);
}


int AntiBreakDialog::ShowModal()
{
    SET::ANTIBREAK::enabled.SendEventToGUI();

    return AutoMovedDialog::ShowModal();
}


AntiBreakDialog::AntiBreakDialog() :
    AutoMovedDialog(_L("Датчик отрыва"))
{
    const int width = BUTTON_SIZE_SMALL.GetWidth() * 2 + 10;

    new CheckButton(this, ID_CHECKBUTTON_ANTIBREAK_ENABLED, _L("Вкл-Откл"), { 10, 10 }, { width, BUTTON_SIZE_SMALL.GetHeight() });

    int y = 45;

    const int dy = 40;

    new TextCtrlNumbersLimits(this, ID_TEXTCNTRL_ANTIBREAK_NUMBER, 1, 1, 0xFFFFFF,
        { 10, y }, { 100, BUTTON_SIZE_SMALL.GetHeight() });

    new wxStaticText(this, ID_STATICTEXT_ANTIBREAK_NUMBER, _L("Номер"), { 120, y + 3 });

    y += dy;

    wxArrayString choices;
    choices.Add(_L("Макс"));
    for (int i = 0; i < 13; i++)
    {
        choices.Add(wxString::Format("%d", 14 - i - 1));
    }
    choices.Add(_L("Мин"));

    new wxComboBox(this, ID_COMBOBOX_ANTIBREAK_SENS, choices[0], {10, y}, {100, TEXTCNTRL_HEIGHT}, choices, wxCB_READONLY);

    new wxStaticText(this, ID_STATICTEXT_ANTIBREAK_SENS, _L("Чувствительность"), { 120, y + 3 });

    y += dy;

    new wxButton(this, wxID_CANCEL, _L_CLOSE, { 10, y }, BUTTON_SIZE_SMALL);
    new wxButton(this, wxID_OK, _L_SAVE, { 10 + BUTTON_SIZE_SMALL.GetWidth() + 10, y }, BUTTON_SIZE_SMALL);

    Bind(wxEVT_TOGGLEBUTTON, &AntiBreakDialog::OnEventToggleButton, this);
    Bind(wxEVT_BUTTON, &AntiBreakDialog::OnEventButton, this);
    Bind(wxEVT_COMBOBOX, &AntiBreakDialog::OnEventComboBox, this);
    Bind(wxEVT_TEXT, &AntiBreakDialog::OnEventText, this);

    AutoMovedDialog::SetFixedSize({ 30 + 2 * BUTTON_SIZE_SMALL.GetWidth(), 170 });
}


void AntiBreakDialog::OnEventToggleButton(wxCommandEvent &event)
{
    int id = event.GetId();
    bool checked = event.IsChecked();

    if (id == ID_CHECKBUTTON_ANTIBREAK_ENABLED)
    {
        FindWindow(ID_TEXTCNTRL_ANTIBREAK_NUMBER)->Enable(checked);
        FindWindow(ID_STATICTEXT_ANTIBREAK_NUMBER)->Enable(checked);
        FindWindow(ID_COMBOBOX_ANTIBREAK_SENS)->Enable(checked);
        FindWindow(ID_STATICTEXT_ANTIBREAK_SENS)->Enable(checked);
    }
}


void AntiBreakDialog::OnEventButton(wxCommandEvent &event)
{
    int id = event.GetId();

    if (id == wxID_OK)
    {
        SET::ANTIBREAK::enabled.SetFromControl();
        SET::ANTIBREAK::number.SetFromControl();
        SET::ANTIBREAK::sens.SetFromControl();
    }
    else if (id == wxID_CANCEL)
    {
        SET::ANTIBREAK::enabled.LoadToControl();
        SET::ANTIBREAK::number.LoadToControl();
        SET::ANTIBREAK::sens.LoadToControl();
    }

    event.Skip();
}


void AntiBreakDialog::OnEventComboBox(wxCommandEvent &event)
{
    int id = event.GetId();

    if (id == ID_COMBOBOX_ANTIBREAK_SENS)
    {
        SET::ANTIBREAK::sens.SetFromControl();
    }
}


void AntiBreakDialog::OnEventText(wxCommandEvent &event)
{
    int id = event.GetId();

    if (id == ID_TEXTCNTRL_ANTIBREAK_NUMBER)
    {
        SET::ANTIBREAK::number.SetFromControl();
    }
}


bool AntiBreakDialog::CurrentSettingsEqualFactory() const
{
    return
        SET::ANTIBREAK::enabled.ValueIsDefault() &&
        SET::ANTIBREAK::number.ValueIsDefault() &&
        SET::ANTIBREAK::sens.ValueIsDefault();
}


void AntiBreakDialog::ResetToFactory(bool factory)
{
    SET::ANTIBREAK::enabled.ResetToFactory(factory);
    SET::ANTIBREAK::number.ResetToFactory(factory);
    SET::ANTIBREAK::sens.ResetToFactory(factory);
}
