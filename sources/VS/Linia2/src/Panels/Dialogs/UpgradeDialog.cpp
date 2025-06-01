// 2023/08/23 15:56:38 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Dialogs/UpgradeDialog.h"
#include "Utils/GlobalFunctions.h"
#include "Reader/Reader.h"
#include "Communicator/ComPort.h"
#include "Utils/Buffer.h"
#include "Panels/ConsoleRS232.h"


wxString       UpgradeDialog::file_name;
UpgradeDialog *UpgradeDialog::self = nullptr;
bool           UpgradeDialog::in_progress = false;


UpgradeDialog::UpgradeDialog() :
    wxDialog(nullptr, wxID_ANY, _L("Загрузка прошивки"))
{
    file_name.Clear();

    in_progress = false;

    self = this;

    wxSize client_size(200, 115);

    int d = 20;

    wxBoxSizer *box_vertical = new wxBoxSizer(wxVERTICAL);

    box_vertical->AddSpacer(d);
    box_vertical->Add(new wxButton(this, ID_BUTTON_FILE_UPGRADE, _L("Выбор файла"), wxDefaultPosition, { 150, BUTTON_HEIGHT }));

    box_vertical->AddSpacer(d);
    box_vertical->Add(new wxButton(this, ID_BUTTON_UPGRADE_START, _L("Начать загрузку"), wxDefaultPosition, { 150, BUTTON_HEIGHT }));
    FindWindow(ID_BUTTON_UPGRADE_START)->Enable(!file_name.IsEmpty());

    wxBoxSizer *box = new wxBoxSizer(wxHORIZONTAL);
    box->AddSpacer(d);
    box->Add(box_vertical);

    SetSizer(box);

    SetClientSize(client_size);

    Bind(wxEVT_BUTTON, &UpgradeDialog::OnEventButton, this);
    Bind(wxEVT_CHAR_HOOK, &UpgradeDialog::OnEventKeyHook, this);
}


void UpgradeDialog::OnEventButton(wxCommandEvent &event)
{
    int id = event.GetId();

    if (id == ID_BUTTON_FILE_UPGRADE)
    {
        wxFileDialog dialog(this, _L("Выбор файла с образом прошивки"), wxEmptyString, wxEmptyString,
            wxString(_L("Файлы с двоичными образами")) + "(*.bin) | *.bin; *.BIN",
            wxFD_OPEN);

        if (dialog.ShowModal() == wxID_OK)
        {
            file_name = dialog.GetPath();

            wxFile file;

            wxString version;

            if (file.Open(file_name))
            {
                uint8 buffer[32];

                file.Read(buffer, 32);

                version = wxString::Format(" v%u", buffer[12] * 256 + buffer[13]);

                file.Close();
            }

            FIND_ANY_BUTTON(ID_BUTTON_FILE_UPGRADE)->SetLabelText(wxFileName::FileName(file_name).GetFullName() + version);
            FIND_ANY_BUTTON(ID_BUTTON_FILE_UPGRADE)->SetToolTip(file_name + version);

            FindWindow(ID_BUTTON_UPGRADE_START)->Enable(true);
        }
    }
    else if (id == ID_BUTTON_UPGRADE_START)
    {
        if (!in_progress)
        {
            FindWindow(ID_BUTTON_FILE_UPGRADE)->Enable(false);

            Upgrade(nullptr);
        }
    }
}


void UpgradeDialog::OnEventKeyHook(wxKeyEvent &event)
{
    if (event.GetKeyCode() == 0x1b)     // Escape
    {
        Close();
    }

    event.Skip();
}


void UpgradeDialog::Upgrade(pchar message)
{
    in_progress = true;

    static uint8 data[10 * 1024 * 1024];

    static int written_bytes = 0;

    static int all_bytes = 0;

    if (message == nullptr)
    {
        wxFile file;

        file.Open(file_name);

        LOG_WRITE("Upload %s : %d bytes", file_name.c_str().AsChar(), (int)file.Length());

        written_bytes = 0;

        all_bytes = (int)file.Length();

        file.Read(data, (size_t)all_bytes);

        file.Close();

        Reader::Send(wxString::Format("#UPDATE SIZE=%d", all_bytes));

        ComPort::CallbackOnReceive::Set(Upgrade);
    }
    else
    {
        ParserBuffer buffer(message);

        if (buffer.FirstBytesIs("#DATA"))
        {
            Parameters parameters;

            SU::SplitToParameters(message, parameters, " ");

            int offset = 0;
            int size = 256;

            if (wxString(parameters.Find("OFFSET").c_str()).ToInt(&offset) &&
                wxString(parameters.Find("SIZE").c_str()).ToInt(&size))
            {
                Reader::SendBuffer(data + offset, size);
                written_bytes += size;

                GF::FindAnyButton(self, ID_BUTTON_UPGRADE_START)->SetLabel(wxString::Format("%d/%d", written_bytes, all_bytes));
            }
        }
        else if (buffer.FirstBytesIs("#END"))
        {
            ComPort::CallbackOnReceive::Reset();

            self->EndModal(777);
        }
    }
}


int UpgradeDialog::ShowModal()
{
    int result = wxDialog::ShowModal();

    wxPoint coord = GetPosition();

    coord.x += 200;

    SetPosition(coord);

    Update();

    return result;
}
