// 2023/10/27 09:01:06 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class LoadSoundDialog : public wxDialog
{
public:
    LoadSoundDialog();
    virtual ~LoadSoundDialog() { }

    static LoadSoundDialog *self;

private:

    void OnEventButton(wxCommandEvent &);

    void OnEventKeyHook(wxKeyEvent &);

    static void Upgrade(pchar);

    // Возвращает true, если файл может быть загружен
    // В bad_line сохраняется номер неправильной строки
    static bool FileIsValid(const wxString &file_name, int *bad_line);

    static bool in_progress;
};


namespace Downloader
{
    extern int written_files;           // Столько файлов уже было записано
    extern int written_bytes_in_file;   // Столько байт записано из текущего файла
    extern int bytes_all;               // Столько байт нужно записать всего в память
    extern int written_bytes_all;       // Всего записано байт

    extern wxString names[SOUNDS_IN_MEMORY];

    // Подготовка к записи файлов
    void Prepare();

    void StartDownload(int num_file);

    void SendBytes(int offset, int num_bytes);

    void ContinueDownload(pchar message);

    // Возвращает общий объём загружаемых файлов
    int DownloadSize();

    // Столько байт будет загружено в считыватель
    int BinaryDownloadSize(const wxString &file_name);
}