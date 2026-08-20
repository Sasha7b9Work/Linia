#include "defines.h"
#include "GUI/PageSettings/MyComputer/FilePanel.h"
#include "GUI/PageSettings/MyComputer/FilePanelEvents.h" 
#include "GUI/PageSettings/MyComputer/FilePanelController.h"
#include "GUI/PageSettings/MyComputer/FilePanelOperations.h"
#include "GUI/PageSettings/MyComputer/FileDropTarget.h"
#include "GUI/PageSettings/MyComputer/FileOperations.h"
#include "GUI/PageSettings/MyComputer/ClipboardManager.h"
#include "GUI/PageSettings/MyComputer/UndoManager.h"
#include "GUI/PageSettings/MyComputer/FTPConnectionDialog.h"
#include "GUI/PageSettings/MyComputer/FTPController.h"
#include "GUI/Controls/Bitmap.h"
#pragma warning(push, 0)
    #include <wx/log.h>
    #include <wx/dir.h>
    #include <wx/dirdlg.h>
    #include <wx/menu.h>
    #include <wx/textdlg.h>
    #include <wx/msgdlg.h>
    #include <wx/dcmemory.h>
#pragma warning(pop)


#ifdef __WXMSW__
#include <windows.h>
#endif


FilePanel::FilePanel(wxWindow *parent, DisplayMode mode):
    wxPanel(parent, wxID_ANY),
    displayMode(mode),
    controller(std::make_unique<FilePanelController>(this)),
    operations(std::make_unique<FilePanelOperations>(this))
{
    CreateControls();
    BindEvents();
    UpdateVisualState();

    // Для MODE_BUTTONS начинаем с экрана выбора (только кнопки)
    if (displayMode == MODE_BUTTONS)
    {
        UpdatePanelState();
    }
    else
    {
        // Для других режимов инициализируем список файлов
        controller->RefreshFileList();
    }

    Bind(wxEVT_LEFT_DOWN, [this](wxMouseEvent &event)
        {
            SetFocus();
            // Отправляем событие активации асинхронно
            wxCommandEvent *activateEvent = new wxCommandEvent(wxEVT_FILEPANEL_ACTIVATED, GetId());
            activateEvent->SetEventObject(this);
            GetParent()->GetEventHandler()->QueueEvent(activateEvent);
            event.Skip();
        });

    Bind(wxEVT_SET_FOCUS, [this](wxFocusEvent &event)
        {
            wxCommandEvent *activateEvent = new wxCommandEvent(wxEVT_FILEPANEL_ACTIVATED, GetId());
            activateEvent->SetEventObject(this);
            GetParent()->GetEventHandler()->QueueEvent(activateEvent);
            event.Skip();
        });

    Bind(wxEVT_CHAR_HOOK, [this](wxKeyEvent &event)
        {
            if (event.ControlDown())
            {
                int keyCode = event.GetKeyCode();
                wxLogDebug("OnKeyDown: Ctrl + %c (keyCode=%d)", (char)keyCode, keyCode);

                switch (keyCode)
                {
                case 'A':
                case 'a':
                {
                    // Выделить все
                    if (fileList)
                    {
                        long count = fileList->GetItemCount();
                        for (long i = 0; i < count; i++)
                        {
                            fileList->SetItemState(i, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
                        }
                        UpdateStatus(wxString::Format("Выделено элементов: %ld", count));
                    }
                    return;
                }
                case 'C':
                case 'c':
                {
                    wxLogDebug("  -> HandleCopyOperation");
                    wxCommandEvent evt;
                    HandleCopyOperation(evt);
                    return;
                }
                case 'X':
                case 'x':
                {
                    wxLogDebug("  -> HandleMoveOperation");
                    wxCommandEvent evt;
                    HandleMoveOperation(evt);
                    return;
                }
                case 'V':
                case 'v':
                {
                    wxLogDebug("  -> HandlePasteOperation");
                    wxCommandEvent evt;
                    HandlePasteOperation(evt);
                    return;
                }
                case 'Z':
                case 'z':
                {
                    HandleUndo();
                    return;
                }
                case 'Y':
                case 'y':
                {
                    HandleRedo();
                    return;
                }
                }
            }
            else if (event.GetKeyCode() == WXK_DELETE)
            {
                wxCommandEvent evt;
                HandleDeleteOperation(evt);
                return;
            }
            else if (event.GetKeyCode() == WXK_F2)
            {
                // Переименование выделенного файла/папки
                if (!fileList || !controller)
                {
                    event.Skip(); return;
                }

                long sel = fileList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
                if (sel == -1)
                {
                    event.Skip(); return;
                }

                wxString oldName = fileList->GetItemText(sel, 0);
                if (oldName == "..")
                {
                    event.Skip(); return;
                }

                wxTextEntryDialog dlg(this, "Новое имя:", "Переименование", oldName);
                if (dlg.ShowModal() != wxID_OK) return;

                wxString newName = dlg.GetValue().Trim().Trim(false);
                if (newName.IsEmpty() || newName == oldName) return;

                wxString basePath = controller->GetCurrentPath();
                wxString sep = wxFileName::GetPathSeparator();
                if (sourceType == SOURCE_FTP) sep = "/";

                wxString oldPath = basePath + sep + oldName;
                wxString newPath = basePath + sep + newName;

                if (RenameFileInternal(oldPath, newPath))
                {
                    controller->RefreshFileList();
                    UpdateStatus("Переименовано: " + oldName + " → " + newName);
                }
                else
                {
                    wxMessageBox("Не удалось переименовать: " + oldName, "Ошибка", wxOK | wxICON_ERROR);
                }
                return;
            }

            event.Skip();
        });
}


FilePanel::~FilePanel()
{
    // Только отключаем FTP-соединение, не обновляя UI (виджеты могут быть уже разрушены)
    if (ftpController)
    {
        ftpController->Disconnect();
    }
    // Ресурсы (controller, ftpController) освобождаются автоматически через std::unique_ptr
}


bool FilePanel::ConnectToFTP(const wxString &host, int _port, const wxString &user, const wxString &pass)
{
    if (!ftpController)
    {
        ftpController = std::make_unique<FTPController>(this);
    }

    if (!ftpController->Connect(host, user, pass, _port))
    {
        UpdateStatus("Ошибка подключения к FTP: " + ftpController->GetLastError());
        return false;
    }

    sourceType = SOURCE_FTP;
    wxString ftpPath = ftpController->GetCurrentDirectory();

    // Сохраняем начальный каталог для ограничения навигации
    ftpInitialDirectory = ftpPath;
    wxLogDebug("FTP connected, initial directory: %s", ftpInitialDirectory);

    if (!ftpPath.IsEmpty())
    {
        pathCtrl->SetValue(ftpPath);
    }

    RefreshFileList();
    UpdateStatus("Подключено к FTP: " + host);
    return true;
}


void FilePanel::DisconnectFTP()
{
    if (ftpController)
    {
        ftpController->Disconnect();
        ftpController.reset();
        ftpInitialDirectory.Clear();
        sourceType = SOURCE_LOCAL;
        controller->SetPath(wxGetCwd());
        RefreshFileList();
        UpdateStatus("Отключено от FTP");
    }
}


bool FilePanel::IsFTPConnected() const
{
    return ftpController && ftpController->IsConnected();
}


void FilePanel::SetActive(bool active)
{
    is_active = active;
    UpdateVisualState();
    // Обновляем статус при активации панели
    if (is_active && controller->HasSelectedFiles())
    {
        controller->UpdateStatusForSelection();
    }
    else if (is_active)
    {
        UpdateStatus("Панель активна, элементы не выбраны");
    }
}


void FilePanel::UpdateStatus(const wxString &status) const
{
    wxWindow *parent = GetParent();
    if (!parent || parent->IsBeingDeleted())
    {
        return;
    }

    wxWindow *grandParent = parent->GetParent();
    if (!grandParent || grandParent->IsBeingDeleted())
    {
        return;
    }

    wxCommandEvent statusEvent(wxEVT_FILEPANEL_STATUS, GetId());
    statusEvent.SetString(status);
    grandParent->ProcessWindowEvent(statusEvent);
}


void FilePanel::CreateControls()
{
    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

    // Создаем либо ComboBox либо кнопки в зависимости от режима
    if (displayMode == MODE_COMBOBOX)
    {
        // Выпадающий список выбора типа источника
        wxBoxSizer *sourceTypeSizer = new wxBoxSizer(wxHORIZONTAL);
        wxStaticText *sourceLabel = new wxStaticText(this, wxID_ANY, "Источник:");
        {
            comboTypeSource = new wxComboBox(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, nullptr, wxCB_READONLY);
            comboTypeSource->Bind(wxEVT_COMBOBOX, [this](wxCommandEvent &)
                {
                    int selection = comboTypeSource->GetSelection();
                    sourceType = static_cast<SourceType>(selection);

                    UpdateStatus(wxString::Format("Переключено на: %s", GetSourceTypeString()));
                    UpdateControlsForSourceType();
                    RefreshFileList();
                });
            comboTypeSource->Append("Локальная файловая система");
            comboTypeSource->Append("USB флешка");
            comboTypeSource->Append("FTP соединение");
            comboTypeSource->SetSelection(0);
        }

        sourceTypeSizer->Add(sourceLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
        sourceTypeSizer->Add(comboTypeSource, 1, wxEXPAND | wxALL, 5);
        mainSizer->Add(sourceTypeSizer, 0, wxEXPAND | wxALL, 0);
    }
    else if (displayMode == MODE_BUTTONS)
    {
        // Кнопки для выбора источника (будут показаны по центру)
        CreateSourceButtons();
        buttonsSizer = new wxBoxSizer(wxVERTICAL);  // Вертикальный сайзер для центрирования

        // Создаем горизонтальный сайзер для самих кнопок
        wxBoxSizer *buttonsHorzSizer = new wxBoxSizer(wxHORIZONTAL);
        buttonsHorzSizer->Add(btnLocal, 0, wxALL, 10);
        buttonsHorzSizer->Add(btnFTP, 0, wxALL, 10);

        // Добавляем кнопки в вертикальный сайзер с центрированием
        buttonsSizer->AddStretchSpacer(1);  // Пустое пространство сверху
        buttonsSizer->Add(buttonsHorzSizer, 0, wxALIGN_CENTER, 0);
        buttonsSizer->AddStretchSpacer(1);  // Пустое пространство снизу

        // Добавляем в главный сайзер - занимает все доступное пространство
        mainSizer->Add(buttonsSizer, 1, wxEXPAND, 0);
    }
    // Если MODE_NONE - не добавляем ничего

    // Строка пути с кнопкой возврата
    wxBoxSizer *pathSizer = new wxBoxSizer(wxHORIZONTAL);

    {
        // Кнопка возврата (стрелка назад) - только для MODE_BUTTONS
        if (displayMode == MODE_BUTTONS)
        {
            btnBack = new ButtonBitmap(this, wxDefaultSize, "undo.bmp", "");

            btnBack->SetToolTip("Вернуться к выбору источника");

            btnBack->Bind(wxEVT_BUTTON, [this](wxCommandEvent &)
                {
                    // Возврат к экрану выбора источника
                    panelState = STATE_SELECTION;

                    // Если был FTP - отключаемся
                    if (sourceType == SOURCE_FTP && ftpController)
                    {
                        ftpController.reset();  // Уничтожаем контроллер, это закроет соединение
                        UpdateStatus("FTP соединение закрыто");
                    }
                    else
                    {
                        UpdateStatus("Возврат к выбору источника");
                    }

                    sourceType = SOURCE_LOCAL;  // Сбрасываем тип источника
                    UpdatePanelState();
                });

            pathSizer->Add(btnBack, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
        }
    }

    {
        pathCtrl = new wxTextCtrl(this, wxID_ANY, controller->GetCurrentPath(), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);

        pathCtrl->Bind(wxEVT_TEXT, [this](wxCommandEvent &)
            {
                wxString newPath = pathCtrl->GetValue();

                if (newPath.IsEmpty())
                {
                    return;
                }

                controller->OnPathChanged(newPath);
            });
    }

    {
        browseBtn = new ButtonBitmap(this, wxDefaultSize, "directory_open.bmp", "");

        browseBtn->SetToolTip("Выбрать папку");

        browseBtn->Bind(wxEVT_BUTTON, [this](wxCommandEvent &)
            {
                // Открываем диалог выбора директории
                wxDirDialog dlg(this, L("Выберите папку"), controller->GetCurrentPath(), wxDD_DEFAULT_STYLE);

                if (dlg.ShowModal() == wxID_OK)
                {
                    controller->SetPath(dlg.GetPath());
                }

            });
    }

    pathSizer->Add(pathCtrl, 1, wxEXPAND | wxALL, 5);
    pathSizer->Add(browseBtn, 0, wxALL, 5);

    {
        // Добавляем стили для отображения разделителей
        fileList = new wxListCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxLC_VRULES | wxLC_HRULES); // wxLC_VRULES добавляет вертикальные линии

        fileList->Bind(wxEVT_LIST_ITEM_ACTIVATED, [this](wxListEvent &event)
            {
                // Двойной клик по элементу списка (файл или папка)
                controller->OnItemActivated(event.GetIndex());
            });

        fileList->Bind(wxEVT_LIST_ITEM_SELECTED, [this](wxListEvent &event)
            {
                // Активируем панель при выборе элемента
                if (!is_active)
                {
                    wxCommandEvent *activateEvent = new wxCommandEvent(wxEVT_FILEPANEL_ACTIVATED, GetId());
                    activateEvent->SetEventObject(this);
                    GetParent()->GetEventHandler()->QueueEvent(activateEvent);
                }
                controller->OnItemSelected(event.GetIndex());
            });

        fileList->Bind(wxEVT_LIST_ITEM_RIGHT_CLICK, [this](wxListEvent &)
            {
                wxMenu menu;

                auto appendWithIcon = [&](wxMenu &m, const wxString &label, const wxString &iconFile, std::function<void(wxCommandEvent &)> handler)
                    {
                        int id = wxNewId();

                        wxMenuItem *item = new wxMenuItem(&m, id, label);
                        wxBitmap &bmp = Bitmap::Get(iconFile).GetBitmap();

                        if (bmp.IsOk())
                        {
                            wxImage img = bmp.ConvertToImage();
                            img.Rescale(16, 16, wxIMAGE_QUALITY_HIGH);
                            item->SetBitmap(wxBitmap(img));
                        }

                        m.Append(item);

                        m.Bind(wxEVT_MENU, [handler](wxCommandEvent &event)
                            {
                                handler(event);
                            }, id);
                    };

                appendWithIcon(menu, "Копировать (Ctrl+C)", "edit-copy.bmp", [this](wxCommandEvent &event)
                    {
                        HandleCopyOperation(event);
                    });
                appendWithIcon(menu, "Вырезать (Ctrl+X)", "edit-cut.bmp", [this](wxCommandEvent &event)
                    {
                        HandleMoveOperation(event);
                    });
                appendWithIcon(menu, "Вставить (Ctrl+V)", "edit-paste.bmp", [this](wxCommandEvent &event)
                    {
                        HandlePasteOperation(event);
                    });
                appendWithIcon(menu, "Удалить (Del)", "edit-delete.bmp", [this](wxCommandEvent &event)
                    {
                        HandleDeleteOperation(event);
                    });
                menu.AppendSeparator();
                appendWithIcon(menu, "Создать папку", "folder-new.bmp", [this](wxCommandEvent &event)
                    {
                        HandleCreateFolder(event);
                    });
                appendWithIcon(menu, "Обновить", "view-refresh.bmp", [this](wxCommandEvent &event)
                    {
                        HandleRefresh(event);
                    });

                PopupMenu(&menu);
            });

        fileList->Bind(wxEVT_LIST_BEGIN_DRAG, [this](wxListEvent &)
            {
                // Инициируем drag-and-drop для выбранных файлов
                if (!controller->HasSelectedFiles())
                {
                    return; // Ничего не выбрано
                }

                wxArrayString selectedFiles = controller->GetSelectedFiles();
                if (selectedFiles.IsEmpty())
                {
                    return;
                }

                // Создаем wxFileDataObject для совместимости с системным D&D
                wxFileDataObject data;
                for (const wxString &file : selectedFiles)
                {
                    if (file != "..")
                    {
                        wxFileName fullPath(controller->GetCurrentPath(), file);
                        data.AddFile(fullPath.GetFullPath());
                    }
                }

                wxDropSource dragSource(this);
                dragSource.SetData(data);
                dragSource.DoDragDrop(wxDrag_CopyOnly);
            });

        fileList->Bind(wxEVT_LIST_COL_CLICK, [this](wxListEvent &event)
            {
                int col = event.GetColumn();
                if (col == sortColumn)
                {
                    sortAscending = !sortAscending;
                }
                else
                {
                    sortColumn = col;
                    sortAscending = true;
                }

                if (!fileList || fileList->GetItemCount() <= 1) return;

                // Собираем данные элементов
                struct ItemData {
                    wxString cols[4];
                    bool isParent;   // ".."
                    bool isDir;      // "<DIR>"
                };

                int count = fileList->GetItemCount();
                std::vector<ItemData> items((uint64)count);

                for (uint64 i = 0; i < (uint64)count; i++)
                {
                    for (int c = 0; c < 4; c++)
                    {
                        items[i].cols[c] = fileList->GetItemText((long)i, c);
                    }
                    items[i].isParent = (items[i].cols[0] == "..");
                    items[i].isDir = (items[i].cols[2] == "<DIR>");
                }

                int sortCol = sortColumn;
                bool asc = sortAscending;

                std::sort(items.begin(), items.end(), [sortCol, asc](const ItemData &a, const ItemData &b)
                    {
                        // ".." всегда первый
                        if (a.isParent) return true;
                        if (b.isParent) return false;
                        // Папки перед файлами
                        if (a.isDir != b.isDir) return a.isDir;

                        int cmp = 0;
                        if (sortCol == 1)
                        {
                            cmp = a.cols[1].CmpNoCase(b.cols[1]);
                        }
                        else
                        {
                            cmp = a.cols[sortCol].CmpNoCase(b.cols[sortCol]);
                        }
                        return asc ? (cmp < 0) : (cmp > 0);
                    });

                // Перестраиваем список
                fileList->Freeze();
                fileList->DeleteAllItems();

                for (uint64 i = 0; i < (uint64)count; i++)
                {
                    long item = fileList->InsertItem((long)i, items[i].cols[0]);
                    fileList->SetItem(item, 1, items[i].cols[1]);
                    fileList->SetItem(item, 2, items[i].cols[2]);
                    fileList->SetItem(item, 3, items[i].cols[3]);
                }

                fileList->Thaw();
            });

        fileList->InsertColumn(0, "Имя", wxLIST_FORMAT_LEFT, 180);
        fileList->InsertColumn(1, "Размер", wxLIST_FORMAT_LEFT, 80);
        fileList->InsertColumn(2, "Тип", wxLIST_FORMAT_LEFT, 80);
        fileList->InsertColumn(3, "Изменен", wxLIST_FORMAT_LEFT, 140);
    }

    mainSizer->Add(pathSizer, 0, wxEXPAND | wxALL, 0);
    mainSizer->Add(fileList, 1, wxEXPAND | wxALL, 5);

    SetSizer(mainSizer);
}


void FilePanel::BindEvents()
{
    // Существующий код...
    fileList->SetDropTarget(new FileDropTarget(this));

    // Добавьте обработку фокуса для списка файлов
    fileList->Bind(wxEVT_SET_FOCUS, [this](wxFocusEvent &event)
        {
            if (!is_active)
            {
                wxCommandEvent *activateEvent = new wxCommandEvent(wxEVT_FILEPANEL_ACTIVATED, GetId());
                activateEvent->SetEventObject(this);
                GetParent()->GetEventHandler()->QueueEvent(activateEvent);
            }
            event.Skip();
        });
}


void FilePanel::UpdateVisualState()
{
    // Изменяем цвет фона для активной/неактивной панели
    wxColour bgColour = is_active ? wxColour(240, 240, 255) : wxColour(255, 255, 255);
    SetBackgroundColour(bgColour);
    fileList->SetBackgroundColour(bgColour);
    pathCtrl->SetBackgroundColour(bgColour);
    Refresh();
}


bool FilePanel::ChangeDirectoryInternal(const wxString &path)
{
    switch (sourceType)
    {
    case SOURCE_LOCAL:
    case SOURCE_USB:
        return wxDirExists(path);

    case SOURCE_FTP:
        return (ftpController && ftpController->ChangeDirectory(path));
    }

    return false;
}


wxString FilePanel::GetCurrentDirectoryInternal() const
{
    switch (sourceType)
    {
    case SOURCE_LOCAL:
    case SOURCE_USB:
        return controller->GetCurrentPath();

    case SOURCE_FTP:
        if (ftpController)
        {
            return ftpController->GetCurrentDirectory();
        }
        return wxEmptyString;
    }
    return controller->GetCurrentPath();
}


bool FilePanel::GetDirectoryContentsInternal(wxArrayString &files, wxArrayString &dirs)
{
    files.Clear();
    dirs.Clear();

    switch (sourceType)
    {
    case SOURCE_LOCAL:
    case SOURCE_USB:
    {
        wxString currentPath = controller->GetCurrentPath();
        wxDir dir(currentPath);
        if (!dir.IsOpened()) return false;

        wxString filename;
        bool cont = dir.GetFirst(&filename, wxEmptyString, wxDIR_DIRS);
        while (cont)
        {
            dirs.Add(filename);
            cont = dir.GetNext(&filename);
        }

        cont = dir.GetFirst(&filename, wxEmptyString, wxDIR_FILES);
        while (cont)
        {
            files.Add(filename);
            cont = dir.GetNext(&filename);
        }
        return true;
    }

    case SOURCE_FTP:
    {
        if (!ftpController)
        {
            return false;
        }

        dirs = ftpController->ListDirectories();
        files = ftpController->ListFiles();
        return true;
    }
    }
    return false;
}


bool FilePanel::CreateDirectoryInternal(const wxString &name)
{
    switch (sourceType)
    {
    case SOURCE_LOCAL:
    case SOURCE_USB:
    {
        wxString fullPath = wxFileName(controller->GetCurrentPath(), name).GetFullPath();
        return wxMkdir(fullPath);
    }

    case SOURCE_FTP:
        return ftpController && ftpController->MakeDirectory(name);
    }
    return false;
}


bool FilePanel::DeleteFileInternal(const wxString &path)
{
    switch (sourceType)
    {
    case SOURCE_LOCAL:
    case SOURCE_USB:
        return wxRemoveFile(path);

    case SOURCE_FTP:
        return ftpController && ftpController->DeleteFile(path);
    }
    return false;
}


bool FilePanel::DeleteDirectoryInternal(const wxString &path)
{
    switch (sourceType)
    {
    case SOURCE_LOCAL:
    case SOURCE_USB:
    {
        // Рекурсивное удаление
        wxDir dir(path);
        if (!dir.IsOpened())
        {
            return false;
        }

        wxString filename;
        bool cont = dir.GetFirst(&filename);
        while (cont)
        {
            wxString fullPath = wxFileName(path, filename).GetFullPath();
            if (wxDirExists(fullPath))
            {
                if (!DeleteDirectoryInternal(fullPath))
                {
                    return false;
                }
            }
            else
            {
                if (!wxRemoveFile(fullPath))
                {
                    return false;
                }
            }
            cont = dir.GetNext(&filename);
        }

        return wxRmdir(path);
    }

    case SOURCE_FTP:
        return ftpController && ftpController->RemoveDirectory(path);
    }
    return false;
}


bool FilePanel::RenameFileInternal(const wxString &oldPath, const wxString &newPath)
{
    switch (sourceType)
    {
    case SOURCE_LOCAL:
    case SOURCE_USB:
        return wxRenameFile(oldPath, newPath);

    case SOURCE_FTP:
        return ftpController && ftpController->RenameFile(oldPath, newPath);
    }
    return false;
}


bool FilePanel::FileExistsInternal(const wxString &path) const
{
    switch (sourceType)
    {
    case SOURCE_LOCAL:
    case SOURCE_USB:
        return wxFileExists(path) || wxDirExists(path);

    case SOURCE_FTP:
    {
        if (!ftpController) return false;
        // Для FTP проверяем через размер файла
        wxULongLong size = ftpController->GetFileSize(path);
        return size != wxInvalidSize;
    }
    }
    return false;
}


bool FilePanel::IsDirectoryInternal(const wxString &path) const
{
    switch (sourceType)
    {
    case SOURCE_LOCAL:
    case SOURCE_USB:
        return wxDirExists(path);

    case SOURCE_FTP:
    {
        if (!ftpController)
        {
            return false;
        }

        return ftpController->IsDirectory(path);
    }
    }

    return false;
}


bool FilePanel::CopyFileBetweenSystems(const wxString &sourcePath, FileSystemType _sourceType, const wxString &destPath, FileSystemType destType, wxWindow *)
{
    // Local -> Local
    if (_sourceType == FS_LOCAL && destType == FS_LOCAL)
    {
        return wxCopyFile(sourcePath, destPath);
    }

    // Local -> FTP
    if (_sourceType == FS_LOCAL && destType == FS_FTP)
    {
        if (!ftpController)
        {
            return false;
        }

        return ftpController->UploadFile(sourcePath, destPath);
    }

    // FTP -> Local
    if (_sourceType == FS_FTP && destType == FS_LOCAL)
    {
        if (!ftpController)
        {
            return false;
        }

        return ftpController->DownloadFile(sourcePath, destPath);
    }

    // FTP -> FTP
    if (_sourceType == FS_FTP && destType == FS_FTP)
    {
        // Через временный файл
        wxString tempFile = wxFileName::CreateTempFileName("ftp_copy");

        bool success = ftpController->DownloadFile(sourcePath, tempFile);
        if (success)
        {
            success = ftpController->UploadFile(tempFile, destPath);
        }

        wxRemoveFile(tempFile);
        return success;
    }

    return false;
}


void FilePanel::HandleCopyOperation(wxCommandEvent &event)
{
    operations->HandleCopyOperation(event);
}


void FilePanel::HandleMoveOperation(wxCommandEvent &event)
{
    operations->HandleMoveOperation(event);
}


void FilePanel::HandlePasteOperation(wxCommandEvent &event)
{
    operations->HandlePasteOperation(event);
}


void FilePanel::HandlePasteOperationToTarget(FilePanel *targetPanel)
{
    operations->HandlePasteOperationToTarget(targetPanel);
}


void FilePanel::HandleDeleteOperation(wxCommandEvent &event)
{
    operations->HandleDeleteOperation(event);
}


void FilePanel::HandleCreateFolder(wxCommandEvent &event)
{
    operations->HandleCreateFolder(event);
}


void FilePanel::HandleRefresh(wxCommandEvent &event)
{
    operations->HandleRefresh(event);
}


void FilePanel::HandleUndo()
{
    operations->HandleUndo();
}


void FilePanel::HandleRedo()
{
    operations->HandleRedo();
}


// Методы для работы с типом источника данных
void FilePanel::SetSourceType(SourceType type)
{
    sourceType = type;
    if (comboTypeSource)
    {
        comboTypeSource->SetSelection(static_cast<int>(type));
    }
    UpdateControlsForSourceType();
    RefreshFileList();
}


wxString FilePanel::GetSourceTypeString() const
{
    switch (sourceType)
    {
    case SOURCE_LOCAL:
        return "Локальная файловая система";
    case SOURCE_USB:
        return "USB флешка";
    case SOURCE_FTP:
        return "FTP соединение";
    default:
        return "Неизвестно";
    }
}


void FilePanel::UpdateControlsForSourceType()
{
    switch (sourceType)
    {
    case SOURCE_LOCAL:
        pathCtrl->Enable(true);
        browseBtn->Enable(true);
        pathCtrl->SetValue(wxGetCwd());
        break;

    case SOURCE_USB:
    {
        pathCtrl->Enable(true);
        browseBtn->Enable(true);
        wxArrayString usbDrives = DetectUSBDrives();
        if (usbDrives.Count() > 0)
        {
            pathCtrl->SetValue(usbDrives[0]);
            UpdateStatus(wxString::Format("Найдено USB устройств: %d", usbDrives.Count()));
        }
        else
        {
            pathCtrl->SetValue("");
            UpdateStatus("USB устройства не обнаружены");
        }
        break;
    }

    case SOURCE_FTP:
        pathCtrl->Enable(true);
        browseBtn->Enable(false);
        pathCtrl->SetValue("ftp://");
        UpdateStatus("Введите FTP адрес (например: ftp://192.168.1.100/)");
        break;
    }
}

wxArrayString FilePanel::DetectUSBDrives()
{
    wxArrayString usbDrives;

#ifdef __WXMSW__
    // Для Windows - сканируем диски от D: до Z:
    for (char drive = 'D'; drive <= 'Z'; drive++)
    {
        wxString drivePath = wxString::Format("%c:\\", drive);
        if (wxDirExists(drivePath))
        {
            // Проверяем, является ли диск съемным устройством
            UINT driveType = GetDriveType(drivePath.wc_str());
            if (driveType == DRIVE_REMOVABLE || driveType == DRIVE_UNKNOWN)
            {
                usbDrives.Add(drivePath);
            }
        }
    }
#else
    // Для Linux - проверяем /media и /mnt
    wxArrayString mediaDirs;
    mediaDirs.Add("/media");
    mediaDirs.Add("/mnt");

    for (size_t i = 0; i < mediaDirs.Count(); i++)
    {
        if (wxDirExists(mediaDirs[i]))
        {
            wxDir dir(mediaDirs[i]);
            if (dir.IsOpened())
            {
                wxString username = wxGetUserId();
                wxString userMediaPath = mediaDirs[i] + "/" + username;

                if (wxDirExists(userMediaPath))
                {
                    wxDir userDir(userMediaPath);
                    wxString filename;
                    bool cont = userDir.GetFirst(&filename, "", wxDIR_DIRS);
                    while (cont)
                    {
                        usbDrives.Add(userMediaPath + "/" + filename);
                        cont = userDir.GetNext(&filename);
                    }
                }
            }
        }
    }
#endif

    return usbDrives;
}

void FilePanel::CreateSourceButtons()
{
    {
        btnLocal = new ButtonBitmap(this, { 64, 64 }, "computer.bmp", "");

        btnLocal->SetToolTip("Локальное хранилище");

        btnLocal->Bind(wxEVT_BUTTON, [this](wxCommandEvent &)
            {
                sourceType = SOURCE_LOCAL;
                panelState = STATE_BROWSING;  // Переходим в режим просмотра каталога
                UpdateStatus("Переключено на: Локальное хранилище");
                UpdatePanelState();
                RefreshFileList();
            });
    }

    {
        btnFTP = new ButtonBitmap(this, { 64, 64 }, "ftp.bmp", "");

        btnFTP->SetToolTip("FTP соединение");

        btnFTP->Bind(wxEVT_BUTTON, [this](wxCommandEvent &)
            {
                // Показываем диалог подключения к FTP
                FTPConnectionDialog dlg(this);
                if (dlg.ShowModal() == wxID_OK)
                {
                    wxString server = dlg.GetServer();
                    int port = dlg.GetPort();
                    wxString username = dlg.GetUsername();
                    wxString password = dlg.GetPassword();

                    // Создаем FTP контроллер если его еще нет
                    if (!ftpController)
                    {
                        ftpController = std::make_unique<FTPController>(this);
                    }

                    // Пытаемся подключиться
                    bool connected = ftpController->Connect(server, username, password, port);

                    // Очищаем пароль из памяти сразу после использования
                    dlg.ClearPassword();
                    for (size_t i = 0; i < password.length(); ++i)
                    {
                        password[i] = '\0';
                    }
                    password.Clear();

                    if (connected)
                    {
                        sourceType = SOURCE_FTP;
                        panelState = STATE_BROWSING;  // Переходим в режим просмотра каталога

                        // Формируем путь для отображения
                        wxString ftpPath = wxString::Format("ftp://%s:%d/", server, port);
                        pathCtrl->SetValue(ftpPath);

                        UpdateStatus(wxString::Format("Подключено к FTP: %s", server));
                        UpdatePanelState();
                        RefreshFileList();
                    }
                    else
                    {
                        wxMessageBox("Не удалось подключиться к FTP серверу", "Ошибка подключения", wxOK | wxICON_ERROR, this);
                    }
                }
            });
    }
}


void FilePanel::UpdatePanelState()
{
    if (displayMode != MODE_BUTTONS)
    {
        return;  // Этот метод только для режима кнопок
    }

    bool showButtons = (panelState == STATE_SELECTION);
    bool showBrowser = (panelState == STATE_BROWSING);

    // Показываем/скрываем кнопки выбора источника
    if (buttonsSizer)
    {
        buttonsSizer->Show(showButtons);
    }

    // Показываем/скрываем элементы браузера файлов
    if (btnBack) btnBack->Show(showBrowser);
    if (pathCtrl) pathCtrl->Show(showBrowser);
    if (browseBtn) browseBtn->Show(showBrowser);
    if (fileList) fileList->Show(showBrowser);

    // Обновляем layout
    Layout();
    Refresh();
}
