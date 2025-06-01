// 2024/01/29 14:34:27 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Controls/TextControls.h"
#include "Settings/Settings.h"
#include "DataBase/DataBaseCards.h"
#include "Panels/Notebook/PageMaster/OfflineModeDialog.h"


OfflineModeDialog *OfflineModeDialog::self = nullptr;


void OfflineModeDialog::Create()
{
    if (!self)
    {
        self = new OfflineModeDialog();
    }
}


void OfflineModeDialog::Delete()
{
    SAFE_DELETE(self);
}


int OfflineModeDialog::ShowModal()
{
    SET::OFFLINE::enabled.SendEventToGUI();

    return AutoMovedDialog::ShowModal();
}


OfflineModeDialog::OfflineModeDialog() :
    AutoMovedDialog(_L("Автономный режим"))
{
    const int width = BUTTON_SIZE_SMALL.GetWidth() * 2 + 10;

    new CheckButton(this, ID_CHECKBUTTON_OFFLINE_ENABLED, _L("Вкл-Откл"), { 10, 10 }, { width, BUTTON_SIZE_SMALL.GetHeight() });

    int y = 45;

    btnOpen = new wxButton(this, ID_BUTTON_OPEN_OFFLINE_DATABASE, _L("Выбрать"), { 10, y }, { width, BUTTON_SIZE_SMALL.GetHeight() });

    if(!SET::OFFLINE::file_cards.Get().IsEmpty())
    {
        btnOpen->SetLabel(SET::OFFLINE::file_cards.Get());
        file_name = SET::OFFLINE::file_cards.Get();
    }

    y = 30;

    (new wxCheckBox(this, ID_CHECKBOX_OFFLINE_SECURITY_MODE, _L("Режим охраны"), { 10, y + 50 }))->SetValue(SET::OFFLINE::security_mode.Get());

    new TextCtrlNumbersLimits(this, ID_TEXTCNTRL_OFFLINE_TIME_LOCK, SET::OFFLINE::time_lock.Get(), 0, 240, { 10, y + 80 }, { 50, TEXTCNTRL_HEIGHT });

    new wxStaticText(this, ID_STATICTEXT_TIME_LOCK, _L("Время замка, c"), { 70, y + 82 });

    new TextCtrlNumbersLimits(this, ID_TEXTCNTRL_OFFLINE_TIME_ALARM, SET::OFFLINE::time_alarm.Get(), 0, 240, { 10, y + 115 }, { 50, TEXTCNTRL_HEIGHT });

    new wxStaticText(this, ID_STATICTEXT_TIME_ALARM, _L("Время тревоги, c"), { 70, y + 117 });

    new wxButton(this, wxID_CANCEL, _L_CLOSE, { 10, y + 150 }, BUTTON_SIZE_SMALL);

    new wxButton(this, wxID_OK, _L_SAVE, { 10 + BUTTON_SIZE_SMALL.GetWidth() + 10, y + 150 }, BUTTON_SIZE_SMALL);

    Bind(wxEVT_TOGGLEBUTTON, &OfflineModeDialog::OnEventToggleButton, this);
    Bind(wxEVT_BUTTON, &OfflineModeDialog::OnEventButton, this);

    SetFixedSize({ 30 + 2 * BUTTON_SIZE_SMALL.GetWidth(), 220 });
}


void OfflineModeDialog::OnEventToggleButton(wxCommandEvent &event)
{
    int id = event.GetId();
    bool checked = event.IsChecked();

    if (id == ID_CHECKBUTTON_OFFLINE_ENABLED)
    {
        FindWindow(ID_TEXTCNTRL_OFFLINE_TIME_LOCK)->Enable(checked);
        FindWindow(ID_STATICTEXT_TIME_LOCK)->Enable(checked);
        FindWindow(ID_TEXTCNTRL_OFFLINE_TIME_ALARM)->Enable(checked);
        FindWindow(ID_STATICTEXT_TIME_ALARM)->Enable(checked);
        FindWindow(ID_CHECKBOX_OFFLINE_SECURITY_MODE)->Enable(checked);
        FindWindow(ID_BUTTON_OPEN_OFFLINE_DATABASE)->Enable(checked);
    }
}


void OfflineModeDialog::OnEventButton(wxCommandEvent &event)
{
    int id = event.GetId();

    if (id == wxID_OK)
    {
        SET::OFFLINE::file_cards.Set(file_name);

        if (SET::OFFLINE::file_cards.Get().IsEmpty() && FIND_CHECKBUTTON(ID_CHECKBUTTON_OFFLINE_ENABLED)->GetValue())
        {
            wxString name = SET::OFFLINE::file_cards.Get();

            SET::OFFLINE::file_cards.Set(name);

            wxMessageBox(_L("Для работы в автономном режиме нужно выбрать файл."), _L("Сообщение"), wxICON_INFORMATION);

            return;
        }
        else
        {
            SET::OFFLINE::enabled.SetFromControl();
            SET::OFFLINE::security_mode.SetFromControl();
            SET::OFFLINE::time_alarm.SetFromControl();
            SET::OFFLINE::time_lock.SetFromControl();

            CardsDB::LoadDataBase();
        }
    }
    else if (id == wxID_CANCEL)
    {
        SET::OFFLINE::enabled.LoadToControl();
        SET::OFFLINE::security_mode.LoadToControl();
        SET::OFFLINE::time_lock.LoadToControl();
        SET::OFFLINE::time_alarm.LoadToControl();
    }
    else if (id == ID_BUTTON_OPEN_OFFLINE_DATABASE)
    {
        wxFileDialog dialog(this, _L("Загрузить карты"), wxGetCwd(), wxEmptyString, wxString(_L("Файлы с картами разрешений")) + "(*.auto) | *.auto", wxFD_OPEN);

        if (dialog.ShowModal() == wxID_OK)
        {
            if (!wxFileExists(dialog.GetPath()))
            {
                wxTextFile file(dialog.GetPath());
                file.Create();
                file.Close();
            }

            file_name = wxFileName(dialog.GetPath()).GetFullName();

            FIND_ANY_BUTTON(ID_BUTTON_OPEN_OFFLINE_DATABASE)->SetLabel(file_name);
        }
    }

    event.Skip();
}


bool OfflineModeDialog::CurrentSettingsEqualFactory() const
{
    return
        SET::OFFLINE::enabled.ValueIsDefault() &&
        SET::OFFLINE::security_mode.ValueIsDefault() &&
        SET::OFFLINE::time_alarm.ValueIsDefault() &&
        SET::OFFLINE::time_lock.ValueIsDefault();
}


void OfflineModeDialog::ResetToFactory(bool factory)
{
    SET::OFFLINE::enabled.ResetToFactory(factory);
    SET::OFFLINE::security_mode.ResetToFactory(factory);
    SET::OFFLINE::time_alarm.ResetToFactory(factory);
    SET::OFFLINE::time_lock.ResetToFactory(factory);
}
