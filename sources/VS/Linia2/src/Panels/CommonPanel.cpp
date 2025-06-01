// 2023/07/04 18:00:47 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "CommonPanel.h"
#include "Panels/Dialogs/PasswordDialog.h"
#include "Settings/Settings.h"
#include "Utils/GlobalFunctions.h"
#include "Panels/Dialogs/ReaderInfoDialog.h"
#include "Panels/Dialogs/CardInfoDialog.h"
#include "Panels/Dialogs/ReaderSettingsDialog.h"
#include "Panels/Dialogs/UpgradeDialog.h"
#include "Panels/ListPasswords.h"
#include "Panels/Dialogs/LoadImportConfigDialog.h"
#include "Panels/Dialogs/SaveExportDialog.h"
#include "DataBase/DataBasePasswords.h"
#include "Reader/Reader.h"
#include "Panels/Notebook/PageMaster/PageMaster.h"
#include "Panels/Dialogs/SettingsSerialPort.h"
#include "Utils/SystemDepend.h"
#include "Panels/Dialogs/KeysSL0Dialog.h"
#include "Panels/Dialogs/EnterKeysSL0Dialog.h"
#include "Panels/Dialogs/PasswordExtraDialog.h"
#include <ctime>


wxSize CommonPanel::SIZE_BUTTON{180, BUTTON_HEIGHT};
CommonPanel *CommonPanel::self = nullptr;


CommonPanel::CommonPanel(wxFrame *parent) : wxWindow(parent, wxID_ANY, { 0, 0 }, { WIDTH, HEIGHT })
{
    SettingsSerialPort::Create();

    PasswordExtraDialog::Create();

    KeysSL0Dialog::Create();

    EnterKeysSL0Dialog::Create(&gset.keysSL0[0], &gset.keysSL0[1], &gset.keysSL0[2], &gset.keysSL0[3], &gset.keysSL0[4]);

    self = this;

    const int X_BOX = 8;

    const int width_box = WIDTH_BOX + (BUTTON_WIDTH_BIG - BUTTON_WIDTH);

    wxSize button_size(BUTTON_WIDTH_BIG, BUTTON_HEIGHT);

#define HEIGHT_BOX(num) ((num) * button_size.y + ((num) - 1) * (Y_1_S - Y_0 - button_size.y) + Y_0 * 2 - SD::D::CommonPanel::Delta())

    {
        wxStaticBox *boxPassword = new wxStaticBox(this, wxID_ANY, _L("Пароль"), { X_BOX, 10 }, { width_box, HEIGHT_BOX(3)});

        (new wxButton(boxPassword, ID_BUTTON_PASSWORD_MASTER_CARD, _L("Пароль к мастер карте"), { X_0, Y_0 }, button_size))->Enable(false);

        new wxButton(boxPassword, ID_BUTTON_CURRENT_SYSTEM_PASSWORD, _L("Действующий пароль системы"), { X_0, Y_1_S }, button_size);

        new wxButton(boxPassword, ID_BUTTON_PASSWORD_EXTRA, _L("Дополнительно"), { X_0, Y_2_S }, button_size);
    }


    {
        wxStaticBox *boxCard = new wxStaticBox(this, wxID_ANY, _L("Карта"), { X_BOX, 150 }, { width_box, HEIGHT_BOX(1)});

        new wxButton(boxCard, ID_BUTTON_CARD_INFO, _L("Текущий пароль"), { X_0, Y_0 }, button_size);
    }

    {
        wxStaticBox *boxReader = new wxStaticBox(this, wxID_ANY, _L("Считыватель"), { X_BOX, 225 }, { width_box, HEIGHT_BOX(1)});

        (new wxButton(boxReader, ID_BUTTON_READER_INFO, _L("Не подключён"), { X_0, Y_0 }, button_size));
    }

    {
        wxStaticBox *boxControl = new wxStaticBox(this, wxID_ANY, _L("Команды управления"), { X_BOX, 300 }, { width_box, HEIGHT_BOX(4)});

        new wxButton(boxControl, ID_BUTTON_LOAD_IMPORT, _L("Загрузить"), { X_0, Y_0 }, button_size);

        new wxButton(boxControl, ID_BUTTON_SAVE_EXPORT, _L("Сохранить"), { X_0, Y_1_S }, button_size);

        new wxButton(boxControl, ID_BUTTON_OPEN_ARCHIVE_PASSWORDS, _L("Открыть архив паролей"), { X_0, Y_2_S}, button_size);

        new wxButton(boxControl, ID_BUTTON_OPEN_ARCHIVE_EVENTS, _L("Открыть архив событий"), { X_0, Y_3_S }, button_size);
    }

    wxWindow::SetMaxClientSize({ 200 + (BUTTON_WIDTH_BIG - BUTTON_WIDTH), 600});

    Bind(wxEVT_BUTTON, &CommonPanel::OnEventButton, this);
}


CommonPanel::~CommonPanel()
{
    self = nullptr;

    EnterKeysSL0Dialog::Delete();

    KeysSL0Dialog::Delete();

    PasswordExtraDialog::Delete();

    SettingsSerialPort::Delete();
}


void CommonPanel::OnEventButton(wxCommandEvent &event)
{
    int id = event.GetId();

    if (id == ID_BUTTON_PASSWORD_MASTER_CARD)
    {
        PasswordDialog dlg(&gset.password_master, _L("Пароль к мастер карте"), true, PasswordDialog::Type::ThreeFields);

        dlg.ShowModal();

        PageMaster::self->OnChangeSettings();
    }
    else if (id == ID_BUTTON_CURRENT_SYSTEM_PASSWORD)
    {
        PasswordDialog dlg(&gset.password_current, _L("Действующий пароль системы"), true);

        dlg.ShowModal();

        PageMaster::self->OnChangeSettings();

        PasswDB::Append(gset.password_current.value);
    }
    else if (id == ID_BUTTON_PASSWORD_EXTRA)
    {
        PasswordExtraDialog::self->ShowModal();
    }
    else if (id == ID_BUTTON_CARD_INFO)
    {
        if (Card::IsInserted())
        {
            CardInfoDialog dlg;

            dlg.ShowModal();
        }
        else
        {
            PasswordDialog dlg(&gset.password_card, _L("Текущий пароль карты"));

            dlg.ShowModal();

            PageMaster::self->OnChangeSettings();
        }
    }
    else if (id == ID_BUTTON_READER_INFO)
    {
        if (Reader::IsConnected())
        {
            ReaderInfoDialog dlg(Reader::GetInfo());

            dlg.ShowModal();
        }
        else
        {
            SettingsSerialPort::self->ShowModal();
        }
    }
    else if (id == ID_BUTTON_LOAD_IMPORT)
    {
        LoadImportConfigDialog dlg;

        dlg.ShowModal();
//
//        wxFileDialog dialog(this, _L("Загрузить настройки"), wxEmptyString, wxEmptyString,
//            wxString(_L("Файлы с настройками")) + "(*.conf) | *.conf; *.CONF",
//            wxFD_OPEN);
//
//        if (dialog.ShowModal() == wxID_OK)
//        {
//            SET::Load(dialog.GetPath());
//            SET::Save("");
//        }
    }
    else if (id == ID_BUTTON_SAVE_EXPORT)
    {
        SaveExportDialog dlg;

        dlg.ShowModal();

//        wxFileDialog dialog(this, _L("Сохранить настройки"), wxEmptyString, wxEmptyString,
//            wxString(_L("Файлы с настройками")) + "(*.conf) | *.conf; *.CONF",
//            wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
//
//        if (dialog.ShowModal() == wxID_OK)
//        {
//            SET::Save(dialog.GetPath());
//        }
    }
    else if (id == ID_BUTTON_OPEN_ARCHIVE_PASSWORDS)
    {
        ListPasswords::self->Show(!ListPasswords::self->IsShown());

        FIND_ANY_BUTTON(ID_BUTTON_OPEN_ARCHIVE_PASSWORDS)->SetLabel(ListPasswords::self->IsShown() ? _L("Закрыть архив паролей") : _L("Открыть архив паролей"));
    }
    else if (id == ID_BUTTON_OPEN_ARCHIVE_EVENTS)
    {
        wxString command(wxString("notepad.exe") + " events.log");

        wxExecute(command);
    }
}


void CommonPanel::CallbackOnReaderConnected(bool is_connected)
{
    wxAnyButton *button = FIND_ANY_BUTTON(ID_BUTTON_READER_INFO);

    if (button->GetToolTip())
    {
        button->UnsetToolTip();
    };

    if (is_connected)
    {
        button->SetLabel(_L("Информация"));
    }
    else
    {
        button->SetLabel(_L("Не подключён"));
    }
}


void CommonPanel::CallbackOnTimer()
{
    if (!Reader::IsConnected())
    {
        wxAnyButton *button = FIND_ANY_BUTTON(ID_BUTTON_READER_INFO);

        button->SetLabel(wxString::Format("%s %d", _L("Подключение"), Reader::TimeFromDisconnect()));
        if (!button->GetToolTip())
        {
            button->SetToolTip(_L("Hint find device"));
        }
    }
}
