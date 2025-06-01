// 2023/08/11 11:28:33 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Dialogs/ReaderInfoDialog.h"
#include "Utils/GlobalFunctions.h"
#include "Panels/Dialogs/UpgradeDialog.h"
#include "Communicator/ComPort.h"
#include "Panels/Dialogs/LoadSoundDialog.h"


wxString ReaderInfoDialog::typeDevice;
wxString ReaderInfoDialog::idDevice;
wxString ReaderInfoDialog::versionFirmware;
wxString ReaderInfoDialog::idFirmware;


ReaderInfoDialog::ReaderInfoDialog(const ReaderInfo &info) :
    wxDialog(nullptr, wxID_ANY, _L("Информация о считывателе"))
{
    wxSize client_size{ 290, 600 };

    int d = 19;

    wxBoxSizer *box1 = new wxBoxSizer(wxHORIZONTAL);

    box1->AddSpacer(d);

    wxArrayString names;
    names.Add(_L("Тип") + " :");
    names.Add(_L("Серийный номер") + " :");
    names.Add(_L("Аппаратная версия") + " :");
    names.Add(_L("Версия загрузчика") + " :");
    names.Add(_L("Версия программы") + " :");
    names.Add(_L("Дата сборки") + " :");
    names.Add(_L("Дата производства") + " :");
    names.Add(_L("Режим ЭКО") + " :");
    names.Add(_L("Адрес OSDP") + " :");
    names.Add(_L("Скорость OSDP") + " :");
    names.Add(_L("Информация") + " :");
    names.Add(_L("Порт") + " :");

    box1->Add(CreateColumn(names));

    box1->AddSpacer(d);

    wxArrayString values;
    values.Add(info.product_type);
    values.Add(info.serial_number);
    values.Add(info.version_hardware);
    values.Add(info.version_loader);
    values.Add(info.version_software);
    values.Add(info.time_build);
    values.Add(info.data_manufacture);
    values.Add(info.eco_enabled ? _L("Вкл") : _L("Откл"));
    values.Add(info.osdp_address);
    values.Add(info.osdp_baudrate);
    values.Add(info.info);
    values.Add(ComPort::NameOpenedPort());

    box1->Add(CreateColumn(values));

    wxBoxSizer *box2 = new wxBoxSizer(wxHORIZONTAL);
    box2->AddSpacer(25);
    box2->Add(new wxButton(this, ID_BUTTON_RESET_READER, _L("Сбросить настройки на заводские"), wxDefaultPosition, { 240, BUTTON_HEIGHT }));

    wxBoxSizer *box3 = new wxBoxSizer(wxHORIZONTAL);
    box3->AddSpacer(25);
    box3->Add(new wxButton(this, ID_BUTTON_UPGRADE_FIRMWARE, _L("Загрузить прошивку"), wxDefaultPosition, { 240, BUTTON_HEIGHT }));

    wxBoxSizer *box4 = new wxBoxSizer(wxHORIZONTAL);
    box4->AddSpacer(25);
    box4->Add(new wxButton(this, ID_BUTTON_LOAD_SOUND, _L("Загрузить звуки"), wxDefaultPosition, { 240, BUTTON_HEIGHT }));

    wxBoxSizer *box6 = new wxBoxSizer(wxHORIZONTAL);
    box6->AddSpacer(25);
    box6->Add(new wxButton(this, ID_BUTTON_CLOSE_INFO_READER, _L("Закрыть"), wxDefaultPosition, { 240, BUTTON_HEIGHT }));

    wxBoxSizer *box = new wxBoxSizer(wxVERTICAL);

    box->Add(box1);

    box->Add(box2);
    box->AddSpacer(d);
    box->Add(box3);
    box->AddSpacer(d);
    box->Add(box4);
    box->AddSpacer(d);
    box->Add(box6);

    SetSizer(box);

    wxDialog::SetClientSize(client_size);

    Bind(wxEVT_BUTTON, &ReaderInfoDialog::OnEventButton, this);
    Bind(wxEVT_CHAR_HOOK, &ReaderInfoDialog::OnEventKeyHook, this);
}


wxBoxSizer *ReaderInfoDialog::CreateColumn(const wxArrayString &lines)
{
    int d = 15;

    wxBoxSizer *box = new wxBoxSizer(wxVERTICAL);

    box->AddSpacer(d);

    for (const wxString &line : lines)
    {
        box->Add(new wxStaticText(this, wxID_ANY, line));
        box->AddSpacer(d);
    }

    return box;
}


void ReaderInfoDialog::OnEventKeyHook(wxKeyEvent &event)
{
    if (event.GetKeyCode() == 0x1b)     // Escape
    {
        Close();
    }

    event.Skip();
}


void ReaderInfoDialog::OnEventButton(wxCommandEvent &event)
{
    int id = event.GetId();

    if (id == ID_BUTTON_RESET_READER)
    {
        wxMessageDialog dlg(this, _L("Продолжение приведёт к тому, что настройки считывателя будут сброшены к заводским."), _L("Внимание!!!"), wxCENTRE | wxICON_WARNING | wxCANCEL);

        if (dlg.ShowModal() == wxID_OK)
        {
            dlg.SetMessage(_L("Вы уверены, что хотите сбросить настройки считывателя к заводским?"));

            if (dlg.ShowModal() == wxID_OK)
            {

            }
        }
    }
    else if (id == ID_BUTTON_UPGRADE_FIRMWARE)
    {
        UpgradeDialog dialog;

        if (dialog.ShowModal() == 777)
        {
            Reader::Disconnect();

            wxMessageDialog(this, _L("Не выключайте считыватель в течение 20 секунд. Идёт обновление."), _L("Внимание !!!"), wxCENTRE | wxICON_WARNING).ShowModal();
        }

        Close();
    }
    else if (id == ID_BUTTON_LOAD_SOUND)
    {
        LoadSoundDialog().ShowModal();

        Close();
    }
    else if (id == ID_BUTTON_CLOSE_INFO_READER)
    {
        Close();
    }
}
