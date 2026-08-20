#pragma once
#include <libssh/sftp.h>
#pragma warning(push, 0)
    #include <wx/arrstr.h>
    #include <wx/longlong.h>
    #include <wx/datetime.h>
#pragma warning(pop)


class FilePanel;


// @brief Контроллер для работы с FTP-соединениями
// Этот класс предоставляет интерфейс для работы с FTP-серверами используя библиотеку libssh.

class FTPController
{
public:
    explicit FTPController(FilePanel *_view);
    ~FTPController();

    // Основные операции
    bool Connect(const wxString &host, const wxString &user, const wxString &password, int port = 22);
    void Disconnect();
    bool IsConnected() const
    {
        return m_isConnected;
    }

    // Навигация
    bool ChangeDirectory(const wxString &path);
    wxString GetCurrentDirectory() const;
    wxArrayString ListFiles();
    wxArrayString ListDirectories();

    // Файловые операции
    bool DownloadFile(const wxString &remoteFile, const wxString &localFile);
    bool UploadFile(const wxString &localFile, const wxString &remoteFile);
    bool DeleteFile(const wxString &remoteFile);
    bool RenameFile(const wxString &oldName, const wxString &newName);
    bool MakeDirectory(const wxString &dirName);
    bool RemoveDirectory(const wxString &dirName);

    // Информация о файле
    wxULongLong GetFileSize(const wxString &filename);
    wxDateTime GetFileModTime(const wxString &filename);
    bool IsDirectory(const wxString &path) const;

    // Обработка ошибок
    wxString GetLastError() const
    {
        return m_lastError;
    }
    wxString GetInitialDirectory() const
    {
        return m_initialPath;
    }

private:
    void ParseFTPUrl(const wxString &url, wxString &host, wxString &user, wxString &password, wxString &path, int &port);
    int VerifyKnownHost();
    wxString BuildFullPath(const wxString &name) const;

    FilePanel *view;
    ssh_session session_ssh = nullptr;
    sftp_session session_sftp = nullptr;
    bool m_isConnected;
    wxString currentPath;
    wxString m_initialPath;  // Начальный путь для ограничения навигации
    wxString m_lastError;
};
