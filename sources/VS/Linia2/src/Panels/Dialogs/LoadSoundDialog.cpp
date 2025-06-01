// 2023/10/27 09:01:20 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Dialogs/LoadSoundDialog.h"
#include "Utils/GlobalFunctions.h"
#include "Communicator/ComPort.h"
#include "Panels/ConsoleRS232.h"
#include "Reader/Reader.h"
#include "Utils/Buffer.h"



LoadSoundDialog *LoadSoundDialog::self = nullptr;
bool             LoadSoundDialog::in_progress = false;


namespace Downloader
{
    int written_files = 0;
    int written_bytes_in_file = 0;
    int bytes_all = 0;
    int written_bytes_all = 0;

    static wxTextFile file;

    wxString names[SOUNDS_IN_MEMORY];


    namespace Binary
    {
        static int16 data[10 * 1024 * 1024];  // Здесь хранятся данные непосредственно для передачи в считыватель
        static int size = 0;
    }
}


LoadSoundDialog::LoadSoundDialog() :
    wxDialog(nullptr, wxID_ANY, _L("Загрузка звуков"))
{
    self = this;

    in_progress = false;

    wxSize client_size(470, 280);

    wxDialog::SetSize(client_size);

    const int x0 = 20;
    int y = 10;

    new wxStaticText(this, wxID_ANY, _L("Можно выбрать несколько файлов, удерживая клавишу Shift или Ctrl"), { x0, y }, { 450, 20 });

    y += 25;

    int number = 0;

    for (int col = 0; col < 2; col++)
    {
        for (int row = 0; row < 5; row++)
        {
            (new wxButton(this, ID_BUTTON_FILE_SOUND + number, wxString::Format("%s %d", _L("Файл"), number + 1), { x0 + col * 210, y + row * 30 }, { 200, BUTTON_HEIGHT }))->Enable(number == 0);
            number++;
        }
    }

    y += 160;

    (new wxButton(this, ID_BUTTON_LOAD_SOUND_START, _L("Загрузить звуки"), { 20, y }, { 410, BUTTON_HEIGHT }))->Enable(false);

    y += 35;

    Bind(wxEVT_BUTTON, &LoadSoundDialog::OnEventButton, this);
    Bind(wxEVT_CHAR_HOOK, &LoadSoundDialog::OnEventKeyHook, this);
}


void LoadSoundDialog::OnEventButton(wxCommandEvent &event)
{
    int id = event.GetId();

    if (id >= ID_BUTTON_FILE_SOUND && id <= (ID_BUTTON_FILE_SOUND + SOUNDS_IN_MEMORY))
    {
        int num_file = id - ID_BUTTON_FILE_SOUND;

        wxFileDialog dialog(this, wxString::Format("%s %d", _L("Выбор файла со звуком"), num_file + 1), wxEmptyString, wxEmptyString,
            wxString(_L("Файлы со звуками")) + " (*.txt;*.vol;*.mp3)|*.txt;*.vol;*.mp3",
            wxFD_OPEN | wxFD_MULTIPLE);

        if (dialog.ShowModal() == wxID_OK)
        {
            wxArrayString paths;

            dialog.GetPaths(paths);

            for (int i = 0; i < (int)paths.size(); i++)
            {
                if (num_file == SOUNDS_IN_MEMORY)
                {
                    break;
                }

                int bad_line = 0;

                if (FileIsValid(paths[(uint)i], &bad_line))
                {
                    Downloader::names[num_file] = paths[(uint)i];

                    FIND_ANY_BUTTON(id + i)->Enable(true);
                    FIND_ANY_BUTTON(id + i)->SetLabelText(wxFileName(Downloader::names[num_file]).GetName());
                    FIND_ANY_BUTTON(id + i)->SetToolTip(Downloader::names[num_file]);

                    num_file++;
                }
                else
                {
                    if (bad_line >= 0)              // Для нетекстовых файлов (mp3) здесь будет отрицательное значение
                    {
                        FIND_ANY_BUTTON(id + i)->SetLabelText(wxString::Format("%s : %d", _L("Неправильный файл"), bad_line));
                    }
                    break;
                }
            }

            if (num_file < 10)
            {
                FIND_ANY_BUTTON(ID_BUTTON_FILE_SOUND + num_file)->Enable(true);
            }
            else
            {
                FIND_ANY_BUTTON(ID_BUTTON_LOAD_SOUND_START)->Enable(true);
            }
        }
    }
    else if (id == ID_BUTTON_LOAD_SOUND_START)
    {
        if (!in_progress)
        {
            if (Downloader::DownloadSize() > SIZE_FOR_SOUNDS)
            {
                wxMessageDialog(this, wxString::Format("%s\n%s", _L("Недостаточно места для загрузки всех файлов."), _L("Загрузка отменена."))).ShowModal();
            }
            else
            {
                for (int i = 0; i < SOUNDS_IN_MEMORY; i++)
                {
                    FindWindow(ID_BUTTON_FILE_SOUND + i)->Enable(false);
                }

                Upgrade(nullptr);
            }
        }
    }
}


int Downloader::BinaryDownloadSize(const wxString &file_name)
{
    wxTextFile r_file;

    r_file.Open(file_name);

    return r_file.IsOpened() ? ((int)(r_file.GetLineCount() - 2) * 2) : 0;
}


int Downloader::DownloadSize()
{
    int result = 0;

    for (int i = 0; i < 10; i++)
    {
        result += BinaryDownloadSize(Downloader::names[i]);
    }

    return result;
}


bool LoadSoundDialog::FileIsValid(const wxString &name, int *bad_line)
{
    *bad_line = -1;

    wxFileName file_name(name);

    wxString ext = file_name.GetExt();

    if (ext == "mp3")
    {
        return false;
    }
    else if (ext == "vol" || ext == "txt")
    {
        wxTextFile file;

        file.Open(name);

        int num_lines = (int)file.GetLineCount();

        if (num_lines < 3)
        {
            return false;
        }

        for (int i = 1; i < num_lines - 1; i++)
        {
            wxString line = file.GetLine((uint)i);

            if (line.empty() || !line.IsNumber())
            {
                *bad_line = i;
                return false;
            }
        }

        return true;
    }

    return false;
}


void Downloader::Prepare()
{
    written_files = 0;
    written_bytes_in_file = 0;
    Binary::size = 0;
    written_bytes_all = 0;

    bytes_all = DownloadSize();
}

void Downloader::StartDownload(int num_file)
{
    file.Open(Downloader::names[num_file]);

    Binary::size = ((int)file.GetLineCount() - 2) * 2;

    written_bytes_in_file = 0;

    file.GetFirstLine();
    file.GetNextLine();

    for (int i = 0; i < Binary::size / 2; i++)
    {
        wxString line = file.GetLine(file.GetCurrentLine());

        int value = 0;

        line.ToInt(&value);

        Binary::data[i] = (int16)value;

        file.GetNextLine();
    }

    file.Close();

    Reader::Send(wxString::Format("#SOUND LOAD NUMBER=%d SIZE=%d", num_file, Binary::size));
}

void Downloader::SendBytes(int offset, int num_bytes)
{
    Reader::SendBuffer(Binary::data + offset / 2, num_bytes);
}

void Downloader::ContinueDownload(pchar message)
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
            SendBytes(offset, size);

            written_bytes_in_file += size;
            written_bytes_all += size;

            GF::FindAnyButton(LoadSoundDialog::self, ID_BUTTON_LOAD_SOUND_START)->SetLabel(wxString::Format("%d/%d", written_bytes_all, bytes_all));
        }
    }
    else if (buffer.FirstBytesIs("#END"))
    {
        file.Close();

        written_files++;

        if (written_files == SOUNDS_IN_MEMORY)
        {
            ComPort::CallbackOnReceive::Reset();

            LoadSoundDialog::self->Close();
        }
        else
        {
            StartDownload(written_files);
        }
    }
}


void LoadSoundDialog::Upgrade(pchar message)
{
    //  Алгоритм загрузки звуковых файлов в прибор
    //  1. Посылаем #LOAD SOUND number size, где number - номер загружаемого звука, size - его размер
    //  2. Прибор присылает #BULK need_bytes, где need_bytes - размер ожидаемого блока данных
    //  3. Повторяем для всех звуков от 0 до 9
    //  4. Загрузку производить строго в последовательности от 0 до 9

    in_progress = true;

    if (message == nullptr)
    {
        ComPort::CallbackOnReceive::Set(Upgrade);

        Downloader::Prepare();

        Downloader::StartDownload(0);
    }
    else
    {
        Downloader::ContinueDownload(message);
    }
}


void LoadSoundDialog::OnEventKeyHook(wxKeyEvent &event)
{
    if (event.GetKeyCode() == 0x1b)     // Escape
    {
        Close();
    }

    event.Skip();
}
