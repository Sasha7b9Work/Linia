#include "defines.h"
#include "GUI/PageSettings/MyComputer/FilePanelOperations.h"
#include "GUI/PageSettings/MyComputer/FilePanel.h"
#include "GUI/PageSettings/MyComputer/FileOperations.h"
#include "GUI/PageSettings/MyComputer/ClipboardManager.h"
#include "GUI/PageSettings/MyComputer/UndoManager.h"
#include "GUI/PageSettings/MyComputer/FTPController.h"
#pragma warning(push, 0)
    #include <wx/log.h>
    #include <wx/msgdlg.h>
    #include <wx/textdlg.h>
#pragma warning(pop)


// Вспомогательная функция для преобразования типа источника
static FilePanelSourceType ConvertSourceType(FilePanel::SourceType type)
{
    switch (type)
    {
    case FilePanel::SOURCE_LOCAL: return SOURCE_TYPE_LOCAL;
    case FilePanel::SOURCE_USB: return SOURCE_TYPE_USB;
    case FilePanel::SOURCE_FTP: return SOURCE_TYPE_FTP;
    default: return SOURCE_TYPE_LOCAL;
    }
}

FilePanelOperations::FilePanelOperations(FilePanel *_panel) :
    panel(_panel)
{
}

void FilePanelOperations::HandleCopyOperation(wxCommandEvent &)
{
    if (!panel->HasSelectedFiles())
    {
        panel->UpdateStatus("Не выбраны файлы или папки");
        return;
    }

    wxArrayString selectedFiles = panel->GetSelectedFiles();
    wxLogDebug("Копирование: выбрано %d элементов", (int)selectedFiles.size());

    for (const wxString &file : selectedFiles)
    {
        if (panel->GetSourceType() == FilePanel::SOURCE_FTP)
        {
            wxLogDebug("  - %s (FTP файл/каталог)", file);
        }
        else
        {
            wxString fullPath = panel->GetCurrentPath() + wxFileName::GetPathSeparator() + file;
            wxLogDebug("  - %s (директория: %d)", file, wxDirExists(fullPath));
        }
    }

    ClipboardManager::ClipboardData data;
    data.files = selectedFiles;
    data.sourcePath = panel->GetCurrentPath();
    data.isCut = false;
    data.sourceType = ConvertSourceType(panel->GetSourceType());
    ClipboardManager::GetInstance().SetData(data);

    panel->UpdateStatus(ClipboardManager::GetInstance().GetOperationDescription());
}

void FilePanelOperations::HandleMoveOperation(wxCommandEvent &)
{
    if (!panel->HasSelectedFiles())
    {
        panel->UpdateStatus("Не выбраны файлы или папки");
        return;
    }

    wxArrayString selectedFiles = panel->GetSelectedFiles();
    ClipboardManager::ClipboardData data;
    data.files = selectedFiles;
    data.sourcePath = panel->GetCurrentPath();
    data.isCut = true;
    data.sourceType = ConvertSourceType(panel->GetSourceType());
    ClipboardManager::GetInstance().SetData(data);

    panel->UpdateStatus(ClipboardManager::GetInstance().GetOperationDescription());
}

void FilePanelOperations::HandlePasteOperation(wxCommandEvent &)
{
    HandlePasteOperationToTarget(nullptr);
}

void FilePanelOperations::HandlePasteOperationToTarget(FilePanel *targetPanel)
{
    ClipboardManager::ClipboardData data = ClipboardManager::GetInstance().GetData();
    if (data.IsEmpty())
    {
        panel->UpdateStatus("Буфер обмена пуст");
        return;
    }

    FilePanel *destPanel = targetPanel ? targetPanel : panel;
    wxString destPath = destPanel->GetCurrentPath();
    FilePanel::SourceType destType = destPanel->GetSourceType();

    bool fromFTP = (data.sourceType == SOURCE_TYPE_FTP);
    bool toFTP = (destType == FilePanel::SOURCE_FTP);

    wxLogDebug("HandlePasteOperationToTarget: fromFTP=%d, toFTP=%d, sourceType=%d, destType=%d", fromFTP, toFTP, data.sourceType, destType);
    wxLogDebug("  Source path: %s", data.sourcePath);
    wxLogDebug("  Dest path: %s", destPath);
    wxLogDebug("  Files count: %d", (int)data.files.size());

    if (fromFTP || toFTP)
    {
        HandleFTPPasteOperation(data, destPanel, fromFTP, toFTP);
        return;
    }

    // Локальные операции
    FileOperation operation(data.isCut ? OperationType::MOVE : OperationType::COPY, data.files, data.sourcePath, destPath);

    int totalFilesCopied = 0;
    int totalDirsCopied = 0;

    for (const wxString &filename : data.files)
    {
        if (filename == "..") continue;

        wxString srcFullPath = data.sourcePath + wxFileName::GetPathSeparator() + filename;
        wxString destFullPath = destPath + wxFileName::GetPathSeparator() + filename;

        bool isDirectory = wxDirExists(srcFullPath);

        FileOperationResult result;
        if (isDirectory)
        {
            result = data.isCut ? FileOperations::MoveDirectory(srcFullPath, destFullPath, panel)
                : FileOperations::CopyDirectory(srcFullPath, destFullPath, panel);
        }
        else
        {
            result = data.isCut ? FileOperations::MoveFile(srcFullPath, destFullPath, true)
                : FileOperations::CopyFile(srcFullPath, destFullPath, true);
        }

        if (result.success)
        {
            totalFilesCopied += result.filesProcessed;
            totalDirsCopied += result.dirsProcessed;
        }
        else
        {
            panel->UpdateStatus("Ошибка при вставке: " + result.errorMessage);
            panel->RefreshFileList();
            if (targetPanel && targetPanel != panel)
            {
                targetPanel->RefreshFileList();
            }
            return;
        }
    }

    UndoManager::GetInstance().AddOperation(operation);

    if (data.isCut)
    {
        ClipboardManager::GetInstance().Clear();
    }

    wxString resultMessage;
    if (totalFilesCopied > 0 && totalDirsCopied > 0)
    {
        resultMessage = wxString::Format("Скопировано: %d файлов и %d папок", totalFilesCopied, totalDirsCopied);
    }
    else if (totalFilesCopied > 0)
    {
        resultMessage = wxString::Format("Скопировано файлов: %d", totalFilesCopied);
    }
    else if (totalDirsCopied > 0)
    {
        resultMessage = wxString::Format("Скопировано папок: %d", totalDirsCopied);
    }
    else
    {
        resultMessage = "Нечего копировать";
    }

    panel->UpdateStatus(resultMessage);
    panel->RefreshFileList();
    if (targetPanel && targetPanel != panel)
    {
        targetPanel->RefreshFileList();
    }
}

void FilePanelOperations::HandleFTPPasteOperation(const ClipboardManager::ClipboardData &data, FilePanel *destPanel, bool fromFTP, bool toFTP)
{
    wxString destPath = destPanel->GetCurrentPath();
    int totalFiles = 0;
    wxString tempDir = wxFileName::GetTempDir();

    wxLogDebug("HandleFTPPasteOperation: fromFTP=%d, toFTP=%d", fromFTP, toFTP);
    wxLogDebug("  Source path: %s", data.sourcePath);
    wxLogDebug("  Dest path: %s", destPath);
    wxLogDebug("  Files: %d", (int)data.files.size());

    FilePanel *sourcePanel = panel;

    for (const wxString &filename : data.files)
    {
        if (filename == "..") continue;

        bool success = false;

        if (fromFTP && !toFTP)
        {
            // FTP -> Local
            FTPController *srcFTP = sourcePanel->GetFTPController();
            if (!srcFTP || !srcFTP->IsConnected())
            {
                panel->UpdateStatus("Нет подключения к исходному FTP серверу");
                continue;
            }

            wxString remotePath = filename;
            wxString localPath = destPath + wxFileName::GetPathSeparator() + filename;

            panel->UpdateStatus(wxString::Format("Скачивание: %s...", filename));
            success = srcFTP->DownloadFile(remotePath, localPath);

            if (success && data.isCut)
            {
                srcFTP->DeleteFile(remotePath);
            }
        }
        else if (!fromFTP && toFTP)
        {
            // Local -> FTP
            wxLogDebug("  Local->FTP копирование файла: %s", filename);

            FTPController *destFTP = destPanel->GetFTPController();
            if (!destFTP)
            {
                wxLogDebug("    destFTP is NULL!");
                panel->UpdateStatus("Нет FTP контроллера на целевой панели");
                continue;
            }
            if (!destFTP->IsConnected())
            {
                wxLogDebug("    destFTP not connected!");
                panel->UpdateStatus("Нет подключения к целевому FTP серверу");
                continue;
            }

            wxString localPath = data.sourcePath + wxFileName::GetPathSeparator() + filename;
            wxString remotePath = filename;

            wxLogDebug("    Local path: %s", localPath);
            wxLogDebug("    Remote path: %s", remotePath);

            if (!wxFileExists(localPath) && !wxDirExists(localPath))
            {
                wxLogDebug("    File does not exist!");
                panel->UpdateStatus(wxString::Format("Файл не найден: %s", localPath));
                continue;
            }

            panel->UpdateStatus(wxString::Format("Загрузка: %s...", filename));

            if (wxDirExists(localPath))
            {
                wxLogDebug("    Is directory - skipping");
                panel->UpdateStatus(wxString::Format("Пропуск директории: %s (пока не поддерживается)", filename));
                continue;
            }

            wxLogDebug("    Uploading file...");
            success = destFTP->UploadFile(localPath, remotePath);
            wxLogDebug("    Upload result: %d", success);

            if (!success)
            {
                wxLogDebug("    Upload failed: %s", destFTP->GetLastError());
                panel->UpdateStatus(wxString::Format("Ошибка загрузки: %s", destFTP->GetLastError()));
            }

            if (success && data.isCut)
            {
                if (wxFileExists(localPath))
                {
                    wxRemoveFile(localPath);
                }
            }
        }
        else if (fromFTP && toFTP)
        {
            // FTP -> FTP
            FTPController *srcFTP = panel->GetFTPController();
            FTPController *destFTP = destPanel->GetFTPController();

            if (srcFTP && srcFTP->IsConnected() && destFTP && destFTP->IsConnected())
            {
                wxString tempFile = tempDir + wxFileName::GetPathSeparator() + filename;
                wxString remoteSrc = filename;
                wxString remoteDest = filename;

                panel->UpdateStatus(wxString::Format("Копирование через временный файл: %s...", filename));

                if (srcFTP->DownloadFile(remoteSrc, tempFile))
                {
                    if (destFTP->UploadFile(tempFile, remoteDest))
                    {
                        success = true;

                        if (data.isCut)
                        {
                            srcFTP->DeleteFile(remoteSrc);
                        }
                    }
                }
                if (wxFileExists(tempFile))
                {
                    wxRemoveFile(tempFile);
                }
            }
        }

        if (success)
        {
            totalFiles++;
        }
        else
        {
            panel->UpdateStatus(wxString::Format("Ошибка при обработке: %s", filename));
        }
    }

    if (data.isCut && totalFiles > 0)
    {
        ClipboardManager::GetInstance().Clear();
    }

    wxString resultMessage = wxString::Format("%s %d файлов", wxString(data.isCut ? "Перемещено" : "Скопировано"), totalFiles);
    panel->UpdateStatus(resultMessage);

    wxLogDebug("HandleFTPPasteOperation completed: totalFiles=%d, destPanel=%p, this=%p", totalFiles, destPanel, panel);

    if (fromFTP)
    {
        wxLogDebug("  Refreshing source FTP panel (this)");
        panel->RefreshFileList();
    }
    if (toFTP)
    {
        if (destPanel)
        {
            wxLogDebug("  Refreshing destination FTP panel");
            destPanel->RefreshFileList();
        }
    }
    if (!fromFTP && !toFTP)
    {
        panel->RefreshFileList();
        if (destPanel && destPanel != panel)
        {
            destPanel->RefreshFileList();
        }
    }
}

void FilePanelOperations::HandleDeleteOperation(wxCommandEvent &)
{
    if (!panel->HasSelectedFiles())
    {
        panel->UpdateStatus("Не выбраны файлы или папки");
        return;
    }

    wxArrayString selectedFiles = panel->GetSelectedFiles();
    wxListCtrl *fileList = panel->GetFileList();

    int fileCount = 0, dirCount = 0;

    if (panel->GetSourceType() == FilePanel::SOURCE_FTP)
    {
        for (const wxString &filename : selectedFiles)
        {
            if (filename == "..") continue;
            wxString type = fileList->GetItemText(
                fileList->FindItem(-1, filename), 2
            );
            if (type == "<DIR>") dirCount++;
            else fileCount++;
        }
    }
    else
    {
        for (const wxString &filename : selectedFiles)
        {
            if (filename == "..") continue;
            wxString fullPath = panel->GetCurrentPath() + wxFileName::GetPathSeparator() + filename;
            if (wxDirExists(fullPath)) dirCount++;
            else fileCount++;
        }
    }

    wxString message;
    if (fileCount > 0 && dirCount > 0)
    {
        message = wxString::Format("Удалить %d файлов и %d папок?", fileCount, dirCount);
    }
    else if (fileCount > 0)
    {
        message = wxString::Format("Удалить %d файлов?", fileCount);
    }
    else
    {
        message = wxString::Format("Удалить %d папок?", dirCount);
    }

    if (wxMessageBox(message, "Подтверждение удаления", wxYES_NO | wxICON_QUESTION, panel) != wxYES)
    {
        return;
    }

    // FTP удаление
    if (panel->GetSourceType() == FilePanel::SOURCE_FTP)
    {
        FTPController *ftpCtrl = panel->GetFTPController();
        if (!ftpCtrl || !ftpCtrl->IsConnected())
        {
            panel->UpdateStatus("Нет подключения к FTP");
            return;
        }

        int totalDeleted = 0;
        for (const wxString &filename : selectedFiles)
        {
            if (filename == "..") continue;

            wxString type = fileList->GetItemText(
                fileList->FindItem(-1, filename), 2
            );

            bool success = false;
            if (type == "<DIR>")
            {
                success = ftpCtrl->RemoveDirectory(filename);
            }
            else
            {
                success = ftpCtrl->DeleteFile(filename);
            }

            if (success)
            {
                totalDeleted++;
            }
            else
            {
                panel->UpdateStatus(wxString::Format("Ошибка удаления: %s - %s", filename, ftpCtrl->GetLastError()));
            }
        }

        panel->UpdateStatus(wxString::Format("Удалено элементов: %d", totalDeleted));
        panel->RefreshFileList();
        return;
    }

    // Локальное удаление
    FileOperation operation(OperationType::_DELETE, selectedFiles, panel->GetCurrentPath());

    int totalFilesDeleted = 0;
    int totalDirsDeleted = 0;

    for (const wxString &filename : selectedFiles)
    {
        if (filename == "..") continue;

        wxString fullPath = panel->GetCurrentPath() + wxFileName::GetPathSeparator() + filename;

        wxLogDebug("Удаление: %s", fullPath);

        if (wxDirExists(fullPath))
        {
            FileOperationResult result = FileOperations::DeleteDirectory(fullPath, panel);
            if (result.success)
            {
                totalFilesDeleted += result.filesProcessed;
                totalDirsDeleted += result.dirsProcessed;
            }
            else
            {
                panel->UpdateStatus("Ошибка удаления директории: " + result.errorMessage);
                panel->RefreshFileList();
                return;
            }
        }
        else if (wxFileExists(fullPath))
        {
            if (wxRemoveFile(fullPath))
            {
                totalFilesDeleted++;
            }
            else
            {
                panel->UpdateStatus("Не удалось удалить файл: " + filename);
                panel->RefreshFileList();
                return;
            }
        }
    }

    UndoManager::GetInstance().AddOperation(operation);

    wxString resultMessage;
    if (totalFilesDeleted > 0 && totalDirsDeleted > 0)
    {
        resultMessage = wxString::Format("Удалено: %d файлов и %d папок", totalFilesDeleted, totalDirsDeleted);
    }
    else if (totalFilesDeleted > 0)
    {
        resultMessage = wxString::Format("Удалено файлов: %d", totalFilesDeleted);
    }
    else if (totalDirsDeleted > 0)
    {
        resultMessage = wxString::Format("Удалено папок: %d", totalDirsDeleted);
    }
    else
    {
        resultMessage = "Ничего не удалено";
    }

    panel->UpdateStatus(resultMessage);
    panel->RefreshFileList();
}

void FilePanelOperations::HandleCreateFolder(wxCommandEvent &)
{
    wxString folderName = wxGetTextFromUser("Введите имя папки:","Создание папки", "Новая папка", panel);

    if (folderName.IsEmpty())
    {
        return;
    }

    // FTP создание папки
    if (panel->GetSourceType() == FilePanel::SOURCE_FTP)
    {
        FTPController *ftpCtrl = panel->GetFTPController();
        if (!ftpCtrl || !ftpCtrl->IsConnected())
        {
            panel->UpdateStatus("Нет подключения к FTP");
            return;
        }

        if (ftpCtrl->MakeDirectory(folderName))
        {
            panel->UpdateStatus("Папка создана: " + folderName);
            panel->RefreshFileList();
        }
        else
        {
            panel->UpdateStatus("Ошибка создания папки: " + ftpCtrl->GetLastError());
        }
        return;
    }

    // Локальное создание папки
    wxFileName newFolder(panel->GetCurrentPath(), folderName);
    FileOperationResult result = FileOperations::CreateDirectory(newFolder.GetFullPath(), panel);

    if (result.success)
    {
        wxArrayString files;
        files.Add(folderName);
        FileOperation operation(OperationType::CREATE_FOLDER, files, panel->GetCurrentPath());
        UndoManager::GetInstance().AddOperation(operation);

        panel->UpdateStatus("Папка создана: " + folderName);
        panel->RefreshFileList();
    }
    else
    {
        panel->UpdateStatus("Ошибка создания папки: " + result.errorMessage);
    }
}

void FilePanelOperations::HandleRefresh(wxCommandEvent &)
{
    panel->RefreshFileList();
    panel->UpdateStatus("Список обновлен");
}

void FilePanelOperations::HandleUndo()
{
    if (!UndoManager::GetInstance().CanUndo())
    {
        panel->UpdateStatus("Нечего отменять");
        return;
    }

    try
    {
        FileOperation op = UndoManager::GetInstance().Undo();

        switch (op.type)
        {
        case OperationType::COPY:
        case OperationType::MOVE:
        {
            for (const wxString &filename : op.files)
            {
                wxString fullPath = op.destPath + wxFileName::GetPathSeparator() + filename;
                if (wxDirExists(fullPath))
                {
                    FileOperations::DeleteDirectory(fullPath, panel);
                }
                else if (wxFileExists(fullPath))
                {
                    wxRemoveFile(fullPath);
                }
            }

            if (op.type == OperationType::MOVE)
            {
                panel->UpdateStatus("Отмена перемещения (частично реализовано)");
            }
            else
            {
                panel->UpdateStatus("Копирование отменено");
            }
            break;
        }
        case OperationType::_DELETE:
        {
            panel->UpdateStatus("Отмена удаления не поддерживается");
            break;
        }
        case OperationType::CREATE_FOLDER:
        {
            for (const wxString &filename : op.files)
            {
                wxString fullPath = op.sourcePath + wxFileName::GetPathSeparator() + filename;
                if (wxDirExists(fullPath))
                {
                    wxRmdir(fullPath);
                }
            }
            panel->UpdateStatus("Создание папки отменено");
            break;
        }
        }

        panel->RefreshFileList();
    }
    catch (const std::exception &e)
    {
        panel->UpdateStatus(wxString::Format("Ошибка отмены: %s", wxString(e.what())));
    }
}

void FilePanelOperations::HandleRedo()
{
    if (!UndoManager::GetInstance().CanRedo())
    {
        panel->UpdateStatus("Нечего повторять");
        return;
    }

    try
    {
        FileOperation op = UndoManager::GetInstance().Redo();

        switch (op.type)
        {
        case OperationType::COPY:
        {
            for (const wxString &filename : op.files)
            {
                wxString srcPath = op.sourcePath + wxFileName::GetPathSeparator() + filename;
                wxString dstPath = op.destPath + wxFileName::GetPathSeparator() + filename;
                if (wxDirExists(srcPath))
                {
                    FileOperations::CopyDirectory(srcPath, dstPath, panel);
                }
                else if (wxFileExists(srcPath))
                {
                    FileOperations::CopyFile(srcPath, dstPath, true);
                }
            }
            panel->UpdateStatus("Повтор: копирование выполнено");
            break;
        }
        case OperationType::MOVE:
        {
            for (const wxString &filename : op.files)
            {
                wxString srcPath = op.sourcePath + wxFileName::GetPathSeparator() + filename;
                wxString dstPath = op.destPath + wxFileName::GetPathSeparator() + filename;
                if (wxDirExists(srcPath))
                {
                    FileOperations::MoveDirectory(srcPath, dstPath, panel);
                }
                else if (wxFileExists(srcPath))
                {
                    wxRenameFile(srcPath, dstPath, true);
                }
            }
            panel->UpdateStatus("Повтор: перемещение выполнено");
            break;
        }
        case OperationType::CREATE_FOLDER:
        {
            for (const wxString &filename : op.files)
            {
                wxString fullPath = op.sourcePath + wxFileName::GetPathSeparator() + filename;
                if (!wxDirExists(fullPath))
                {
                    wxMkdir(fullPath);
                }
            }
            panel->UpdateStatus("Повтор: папка создана");
            break;
        }
        case OperationType::_DELETE:
        {
            panel->UpdateStatus("Повтор удаления не поддерживается (необратимая операция)");
            break;
        }
        }

        panel->RefreshFileList();
    }
    catch (const std::exception &e)
    {
        panel->UpdateStatus(wxString::Format("Ошибка повтора: %s", wxString(e.what())));
    }
}
