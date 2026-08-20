#include "defines.h"
#include "GUI/PageSettings/MyComputer/FTPController.h"
#include "GUI/PageSettings/MyComputer/FilePanel.h"
#pragma warning(push, 0)
    #include <wx/msgdlg.h>
    #include <wx/log.h>
    #include <wx/wfstream.h>
#pragma warning(pop)


#ifdef WIN32
#define S_IRUSR 0000400  /* Read permission, owner */
#define S_IWUSR 0000200  /* Write permission, owner */
#define S_IXUSR 0000100  /* Execute permission, owner */
#define S_IRWXU (S_IRUSR | S_IWUSR | S_IXUSR)  /* чтение + запись + выполнение */
#endif


FTPController::FTPController(FilePanel *_view) :
    view(_view),
    currentPath("/")
{
}


FTPController::~FTPController()
{
    Disconnect();
}


void FTPController::ParseFTPUrl(const wxString &url, wxString &host, wxString &user, wxString &_password, wxString &path, int &_port)
{
    wxString tempUrl = url;

    // Убираем ftp:// (6 символов)
    if (tempUrl.StartsWith("ftp://"))
    {
        tempUrl = tempUrl.Mid(6);
    }

    // Извлекаем путь
    int pathPos = tempUrl.Find('/');
    if (pathPos != wxNOT_FOUND)
    {
        path = tempUrl.Mid((size_t)pathPos);
        tempUrl = tempUrl.Left((size_t)pathPos);
    }
    else
    {
        path = "/";
    }

    // Извлекаем user:pass@ — ищем ПОСЛЕДНИЙ '@' для защиты от '@' в пароле
    int atPos = tempUrl.Find('@', true);  // true = from end
    if (atPos != wxNOT_FOUND)
    {
        wxString userPass = tempUrl.Left((size_t)atPos);
        tempUrl = tempUrl.Mid((size_t)atPos + 1U);

        // Ищем ПЕРВЫЙ ':' для разделения user:password
        int colonPos = userPass.Find(':');
        if (colonPos != wxNOT_FOUND)
        {
            user = userPass.Left((size_t)colonPos);
            _password = userPass.Mid((size_t)colonPos + 1U);
        }
        else
        {
            user = userPass;
            _password = "";
        }
    }
    else
    {
        user = wxGetUserId();
        _password = "";
    }

    // Извлекаем host:port
    int colonPos = tempUrl.Find(':');
    if (colonPos != wxNOT_FOUND)
    {
        host = tempUrl.Left((size_t)colonPos);
        wxString portStr = tempUrl.Mid((size_t)colonPos + 1U);
        long portLong;
        if (portStr.ToLong(&portLong) && portLong >= 1 && portLong <= 65535)
        {
            _port = static_cast<int>(portLong);
        }
        else
        {
            _port = 22;
        }
    }
    else
    {
        host = tempUrl;
        _port = 22;
    }

    // Валидация: хост не должен быть пустым и не должен содержать спецсимволы
    host = host.Trim().Trim(false);
    if (host.IsEmpty())
    {
        host = "localhost";
    }
}

int FTPController::VerifyKnownHost()
{
    ssh_key srv_pubkey = nullptr;
    unsigned char *hash = nullptr;
    size_t hlen;
    char *hexa;

    int rc = ssh_get_server_publickey(session_ssh, &srv_pubkey);
    if (rc < 0)
    {
        return -1;
    }

    rc = ssh_get_publickey_hash(srv_pubkey, SSH_PUBLICKEY_HASH_SHA256, &hash, &hlen);
    ssh_key_free(srv_pubkey);
    if (rc < 0)
    {
        return -1;
    }

    enum ssh_known_hosts_e state = ssh_session_is_known_server(session_ssh);

    switch (state)
    {
    case SSH_KNOWN_HOSTS_OK:
        ssh_clean_pubkey_hash(&hash);
        return 0;

    case SSH_KNOWN_HOSTS_CHANGED:
        last_error = "Внимание: ключ хоста изменился! Возможна атака MITM!";
        ssh_clean_pubkey_hash(&hash);
        return -1;

    case SSH_KNOWN_HOSTS_OTHER:
        last_error = "Хост найден, но тип ключа изменился";
        ssh_clean_pubkey_hash(&hash);
        return -1;

    case SSH_KNOWN_HOSTS_NOT_FOUND:
    case SSH_KNOWN_HOSTS_UNKNOWN:
    {
        hexa = ssh_get_hexa(hash, hlen);
        wxString message = wxString::Format("Хост неизвестен. Отпечаток ключа:\n%s\n\nДоверяете этому хосту?", wxString(hexa));
        ssh_string_free_char(hexa);
        ssh_clean_pubkey_hash(&hash);

        int answer = wxMessageBox(message, "Неизвестный хост", wxYES_NO | wxICON_QUESTION);

        if (answer == wxYES)
        {
            rc = ssh_session_update_known_hosts(session_ssh);
            if (rc < 0)
            {
                last_error = wxString::Format("Ошибка обновления known_hosts: %s", wxString(ssh_get_error(session_ssh)));
                return -1;
            }
            return 0;
        }
        return -1;
    }

    case SSH_KNOWN_HOSTS_ERROR:
        last_error = wxString::Format("Ошибка проверки known_hosts: %s", wxString(ssh_get_error(session_ssh)));
        ssh_clean_pubkey_hash(&hash);
        return -1;
    }

    ssh_clean_pubkey_hash(&hash);
    return -1;
}

bool FTPController::Connect(const wxString &host, const wxString &user, const wxString &_password, int _port)
{
    if (is_connected)
    {
        Disconnect();
    }

    // Создаем SSH сессию
    session_ssh = ssh_new();
    if (!session_ssh)
    {
        last_error = "Не удалось создать SSH сессию";
        return false;
    }

    // Настраиваем параметры подключения
    ssh_options_set(session_ssh, SSH_OPTIONS_HOST, host.utf8_str().data());
    ssh_options_set(session_ssh, SSH_OPTIONS_PORT, &_port);
    ssh_options_set(session_ssh, SSH_OPTIONS_USER, user.utf8_str().data());

    // Устанавливаем таймаут
    long timeout = 10; // 10 секунд
    ssh_options_set(session_ssh, SSH_OPTIONS_TIMEOUT, &timeout);

    // Подключаемся
    int rc = ssh_connect(session_ssh);
    if (rc != SSH_OK)
    {
        last_error = wxString::Format("Не удалось подключиться: %s", wxString(ssh_get_error(session_ssh)));
        ssh_free(session_ssh);
        session_ssh = nullptr;
        return false;
    }

    // Проверяем ключ хоста
    if (VerifyKnownHost() < 0)
    {
        ssh_disconnect(session_ssh);
        ssh_free(session_ssh);
        session_ssh = nullptr;
        return false;
    }

    // Аутентификация
    rc = ssh_userauth_password(session_ssh, nullptr, _password.utf8_str().data());
    if (rc != SSH_AUTH_SUCCESS)
    {
        last_error = wxString::Format("Ошибка аутентификации: %s", wxString(ssh_get_error(session_ssh)));
        ssh_disconnect(session_ssh);
        ssh_free(session_ssh);
        session_ssh = nullptr;
        return false;
    }

    // Создаем FTP сессию
    session_sftp = sftp_new(session_ssh);
    if (!session_sftp)
    {
        last_error = wxString::Format("Не удалось создать FTP сессию: %s", wxString(ssh_get_error(session_ssh)));
        ssh_disconnect(session_ssh);
        ssh_free(session_ssh);
        session_ssh = nullptr;
        return false;
    }

    rc = sftp_init(session_sftp);
    if (rc != SSH_OK)
    {
        last_error = wxString::Format("Ошибка инициализации FTP: %s", wxString(ssh_get_error(session_ssh)));
        sftp_free(session_sftp);
        session_sftp = nullptr;
        ssh_disconnect(session_ssh);
        ssh_free(session_ssh);
        session_ssh = nullptr;
        return false;
    }

    // Получаем реальный текущий каталог (используем canonicalize для получения абсолютного пути)
    char *realPath = sftp_canonicalize_path(session_sftp, ".");
    if (realPath)
    {
        currentPath = wxString::FromUTF8(realPath);
        initialPath = currentPath;  // Сохраняем начальный путь
        ssh_string_free_char(realPath);
    }
    else
    {
        currentPath = "/";
        initialPath = "/";
    }

    is_connected = true;
    last_error.Clear();

    wxLogDebug("FTPController::Connect - current directory: %s", currentPath);
    wxLogDebug("FTPController::Connect - initial directory: %s", initialPath);

    return true;
}

void FTPController::Disconnect()
{
    if (session_sftp)
    {
        sftp_free(session_sftp);
        session_sftp = nullptr;
    }

    if (session_ssh)
    {
        ssh_disconnect(session_ssh);
        ssh_free(session_ssh);
        session_ssh = nullptr;
    }

    currentPath.Clear();
    initialPath.Clear();
    is_connected = false;
}

wxString FTPController::BuildFullPath(const wxString &name) const
{
    if (name.StartsWith("/"))
    {
        return name;
    }
    if (currentPath.EndsWith("/"))
    {
        return currentPath + name;
    }
    return currentPath + "/" + name;
}

bool FTPController::ChangeDirectory(const wxString &path)
{
    if (!is_connected || !session_sftp)
    {
        last_error = "Нет подключения к FTP";
        return false;
    }

    wxLogDebug("FTPController::ChangeDirectory: attempting to change to '%s'", path);
    wxLogDebug("  Current: '%s', Initial: '%s'", currentPath, initialPath);

    // Проверяем, что новый путь начинается с начального пути
    if (!path.StartsWith(initialPath))
    {
        wxLogDebug("  -> BLOCKED: path does not start with initial directory");
        last_error = "Навигация за пределы начального каталога запрещена";
        return false;
    }

    // Проверяем, что путь не короче начального (не выше в иерархии)
    if (path.Length() < initialPath.Length())
    {
        wxLogDebug("  -> BLOCKED: path is shorter than initial directory");
        last_error = "Навигация за пределы начального каталога запрещена";
        return false;
    }

    // Проверяем, существует ли директория
    sftp_dir dir = sftp_opendir(session_sftp, path.utf8_str().data());
    if (!dir)
    {
        last_error = wxString::Format("Не удалось открыть директорию: %s", wxString(ssh_get_error(session_ssh)));
        wxLogDebug("  -> FAILED: %s", last_error);
        return false;
    }

    sftp_closedir(dir);
    currentPath = path;
    wxLogDebug("  -> SUCCESS: changed to '%s'", currentPath);
    return true;
}

wxString FTPController::GetCurrentDirectory() const
{
    return currentPath;
}

wxArrayString FTPController::ListFiles()
{
    wxArrayString files;

    if (!is_connected || !session_sftp)
    {
        last_error = "Нет подключения к FTP";
        return files;
    }

    sftp_dir dir = sftp_opendir(session_sftp, currentPath.utf8_str().data());
    if (!dir)
    {
        last_error = wxString::Format("Не удалось открыть директорию: %s", wxString(ssh_get_error(session_ssh)));
        return files;
    }

    sftp_attributes attr;
    while ((attr = sftp_readdir(session_sftp, dir)) != nullptr)
    {
        wxString name = wxString::FromUTF8(attr->name);

        // Пропускаем . и .. и директории
        if (name != "." && name != ".." && attr->type != SSH_FILEXFER_TYPE_DIRECTORY)
        {
            files.Add(name);
        }

        sftp_attributes_free(attr);
    }

    if (!sftp_dir_eof(dir))
    {
        last_error = "Ошибка чтения директории";
    }

    sftp_closedir(dir);
    return files;
}

wxArrayString FTPController::ListDirectories()
{
    wxArrayString dirs;

    if (!is_connected || !session_sftp)
    {
        last_error = "Нет подключения к FTP";
        return dirs;
    }

    sftp_dir dir = sftp_opendir(session_sftp, currentPath.utf8_str().data());
    if (!dir)
    {
        last_error = wxString::Format("Не удалось открыть директорию: %s", wxString(ssh_get_error(session_ssh)));
        return dirs;
    }

    sftp_attributes attr;
    while ((attr = sftp_readdir(session_sftp, dir)) != nullptr)
    {
        wxString name = wxString::FromUTF8(attr->name);

        // Пропускаем . и .. , но добавляем все директории
        // Также пропускаем диски Windows (C:, D:, и т.д.)
        bool isWindowsDrive = (name.Length() == 2 && name[1] == ':' &&
            wxIsalpha(name[0]));

        if (name != "." && name != ".." && !isWindowsDrive &&
            attr->type == SSH_FILEXFER_TYPE_DIRECTORY)
        {
            dirs.Add(name);
        }

        sftp_attributes_free(attr);
    }

    if (!sftp_dir_eof(dir))
    {
        last_error = "Ошибка чтения директории";
    }

    sftp_closedir(dir);
    return dirs;
}

bool FTPController::DownloadFile(const wxString &remoteFile, const wxString &localFile)
{
    if (!is_connected || !session_sftp)
    {
        last_error = "Нет подключения к FTP";
        return false;
    }

    wxString fullPath = BuildFullPath(remoteFile);
    // Открываем удаленный файл
    sftp_file file = sftp_open(session_sftp, fullPath.utf8_str().data(), O_RDONLY, 0);
    if (!file)
    {
        last_error = wxString::Format("Не удалось открыть удаленный файл: %s", wxString(ssh_get_error(session_ssh)));
        return false;
    }

    // Открываем локальный файл
    wxFileOutputStream output(localFile);
    if (!output.IsOk())
    {
        last_error = "Не удалось создать локальный файл";
        sftp_close(file);
        return false;
    }

    // Читаем и записываем данные порциями
    char buffer[16384];
    ssize_t nbytes;
    bool success = true;

    while ((nbytes = sftp_read(file, buffer, sizeof(buffer))) > 0)
    {
        output.Write(buffer, (size_t)nbytes);
        if (!output.IsOk())
        {
            last_error = "Ошибка записи в локальный файл";
            success = false;
            break;
        }
    }

    if (nbytes < 0)
    {
        last_error = wxString::Format("Ошибка чтения удаленного файла: %s", wxString(ssh_get_error(session_ssh)));
        success = false;
    }

    sftp_close(file);
    return success;
}

bool FTPController::UploadFile(const wxString &localFile, const wxString &remoteFile)
{
    if (!is_connected || !session_sftp)
    {
        last_error = "Нет подключения к FTP";
        return false;
    }

    // Открываем локальный файл
    wxFileInputStream input(localFile);
    if (!input.IsOk())
    {
        last_error = "Не удалось открыть локальный файл";
        return false;
    }

    wxString fullPath = BuildFullPath(remoteFile);
    // Создаем удаленный файл
    sftp_file file = sftp_open(session_sftp, fullPath.utf8_str().data(), O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);

    if (!file)
    {
        last_error = wxString::Format("Не удалось создать удаленный файл: %s", wxString(ssh_get_error(session_ssh)));
        return false;
    }

    // Читаем и записываем данные порциями
    char buffer[16384];
    bool success = true;

    while (!input.Eof())
    {
        input.Read(buffer, sizeof(buffer));
        size_t bytesRead = input.LastRead();

        if (bytesRead > 0)
        {
            ssize_t nwritten = sftp_write(file, buffer, bytesRead);
            if (nwritten != (ssize_t)bytesRead)
            {
                last_error = wxString::Format("Ошибка записи в удаленный файл: %s", wxString(ssh_get_error(session_ssh)));
                success = false;
                break;
            }
        }
    }

    sftp_close(file);
    return success;
}

bool FTPController::DeleteFile(const wxString &remoteFile)
{
    if (!is_connected || !session_sftp)
    {
        last_error = "Нет подключения к FTP";
        return false;
    }

    wxString fullPath = BuildFullPath(remoteFile);
    int rc = sftp_unlink(session_sftp, fullPath.utf8_str().data());
    if (rc != SSH_OK)
    {
        last_error = wxString::Format("Не удалось удалить файл: %s", wxString(ssh_get_error(session_ssh)));
        return false;
    }

    return true;
}

bool FTPController::RenameFile(const wxString &oldName, const wxString &newName)
{
    if (!is_connected || !session_sftp)
    {
        last_error = "Нет подключения к FTP";
        return false;
    }

    wxString fullOld = BuildFullPath(oldName);
    wxString fullNew = BuildFullPath(newName);
    int rc = sftp_rename(session_sftp, fullOld.utf8_str().data(), fullNew.utf8_str().data());
    if (rc != SSH_OK)
    {
        last_error = wxString::Format("Не удалось переименовать: %s", wxString(ssh_get_error(session_ssh)));
        return false;
    }

    return true;
}

bool FTPController::MakeDirectory(const wxString &dirName)
{
    if (!is_connected || !session_sftp)
    {
        last_error = "Нет подключения к FTP";
        return false;
    }

    wxString fullPath = BuildFullPath(dirName);
    int rc = sftp_mkdir(session_sftp, fullPath.utf8_str().data(), S_IRWXU);
    if (rc != SSH_OK)
    {
        last_error = wxString::Format("Не удалось создать директорию: %s", wxString(ssh_get_error(session_ssh)));
        return false;
    }

    return true;
}

bool FTPController::RemoveDirectory(const wxString &dirName)
{
    if (!is_connected || !session_sftp)
    {
        last_error = "Нет подключения к FTP";
        return false;
    }

    wxString fullPath = BuildFullPath(dirName);
    int rc = sftp_rmdir(session_sftp, fullPath.utf8_str().data());
    if (rc != SSH_OK)
    {
        last_error = wxString::Format("Не удалось удалить директорию: %s", wxString(ssh_get_error(session_ssh)));
        return false;
    }

    return true;
}

bool FTPController::IsDirectory(const wxString &path) const
{
    if (!is_connected || !session_sftp)
    {
        return false;
    }

    wxString fullPath = path.StartsWith("/") ? path : BuildFullPath(path);
    sftp_attributes attr = sftp_stat(session_sftp, fullPath.utf8_str().data());
    if (!attr)
    {
        return false;
    }

    bool isDir = (attr->type == SSH_FILEXFER_TYPE_DIRECTORY);
    sftp_attributes_free(attr);
    return isDir;
}

wxULongLong FTPController::GetFileSize(const wxString &filename)
{
    if (!is_connected || !session_sftp)
    {
        return 0;
    }

    wxString fullPath = BuildFullPath(filename);

    sftp_attributes attr = sftp_stat(session_sftp, fullPath.utf8_str().data());
    if (!attr)
    {
        return 0;
    }

    wxULongLong size = attr->size;
    sftp_attributes_free(attr);

    return size;
}

wxDateTime FTPController::GetFileModTime(const wxString &filename)
{
    wxDateTime dt;

    if (!is_connected || !session_sftp)
    {
        return dt;
    }

    wxString fullPath = BuildFullPath(filename);

    sftp_attributes attr = sftp_stat(session_sftp, fullPath.utf8_str().data());
    if (!attr)
    {
        return dt;
    }

    dt = wxDateTime((time_t)attr->mtime);
    sftp_attributes_free(attr);

    return dt;
}
