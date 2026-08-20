#pragma once
#include "GUI/PageSettings/MyComputer/FilePanelController.h"
#include "GUI/PageSettings/MyComputer/FilePanelEvents.h"
#include "GUI/PageSettings/MyComputer/ClipboardManager.h"
#pragma warning(push, 0)
    #include <wx/panel.h>
    #include <wx/textctrl.h>
    #include <wx/listctrl.h>
    #include <wx/combobox.h>
    #include <wx/bmpbuttn.h>
    #include <wx/sizer.h>
#pragma warning(pop)


class FTPController;
class FilePanelOperations;


class FilePanel : public wxPanel
{
public:
    // Типы источников данных (переименовано для совместимости)
    enum FileSystemType
    {
        FS_LOCAL,      // Локальная файловая система
        FS_USB,        // USB флешка
        FS_FTP         // FTP соединение
    };

    // Алиасы для обратной совместимости
    enum SourceType
    {
        SOURCE_LOCAL = FS_LOCAL,
        SOURCE_USB = FS_USB,
        SOURCE_FTP = FS_FTP
    };

    // Режим отображения источников
    enum DisplayMode
    {
        MODE_NONE,         // Без выбора источника (для левой панели)
        MODE_COMBOBOX,     // Выпадающий список
        MODE_BUTTONS       // Кнопки (для правой панели)
    };

    // Состояние панели (для правой панели с кнопками)
    enum PanelState
    {
        STATE_SELECTION,   // Показываем только кнопки выбора источника
        STATE_BROWSING     // Показываем каталог и кнопку возврата
    };

    FilePanel(wxWindow *, DisplayMode mode = MODE_NONE);
    virtual ~FilePanel();

    // Удаляем идентификаторы кнопок, оставляем только меню
    enum
    {
        ID_COPY = wxID_HIGHEST + 1,
        ID_MOVE,
        ID_DELETE,
        ID_CREATE_FOLDER,
        ID_REFRESH,
        ID_PASTE,
        ID_SOURCE_TYPE,
        ID_PATH_CTRL,
        ID_FILE_LIST
    };

    void RefreshFileList()
    {
        controller->RefreshFileList();
    }
    void SetPath(const wxString &path)
    {
        controller->SetPath(path);
    }
    wxString GetCurrentPath() const
    {
        return controller->GetCurrentPath();
    }
    wxArrayString GetSelectedFiles() const
    {
        return controller->GetSelectedFiles();
    }
    bool HasSelectedFiles() const
    {
        return controller->HasSelectedFiles();
    }

    void SetActive(bool active);
    bool IsActive() const
    {
        return is_active;
    }

    // Методы для работы с типом источника
    SourceType GetSourceType() const
    {
        return sourceType;
    }
    FileSystemType GetFileSystemType() const
    {
        return static_cast<FileSystemType>(sourceType);
    }
    void SetSourceType(SourceType type);
    void SetFileSystemType(FileSystemType type)
    {
        SetSourceType(static_cast<SourceType>(type));
    }
    wxString GetSourceTypeString() const;

    // Новые методы для работы с FTP
    bool ConnectToFTP(const wxString &host, int port, const wxString &user, const wxString &pass);
    void DisconnectFTP();
    bool IsFTPConnected() const;
    wxString GetFTPInitialDirectory() const
    {
        return ftpInitialDirectory;
    }

    wxTextCtrl *GetPathCtrl() const
    {
        return pathCtrl;
    }
    wxListCtrl *GetFileList() const
    {
        return fileList;
    }
    void UpdateStatus(const wxString &status) const;

    // Доступ к FTP контроллеру
    FTPController *GetFTPController() const
    {
        return ftpController.get();
    }

    // Обработчики операций (делегируют в FilePanelOperations)
    void HandleCopyOperation(wxCommandEvent &);
    void HandleMoveOperation(wxCommandEvent &);
    void HandlePasteOperation(wxCommandEvent &);
    void HandlePasteOperationToTarget(FilePanel *targetPanel);
    void HandleDeleteOperation(wxCommandEvent &);
    void HandleCreateFolder(wxCommandEvent &);
    void HandleRefresh(wxCommandEvent &);
    void HandleUndo();
    void HandleRedo();

    // Доступ к объекту операций
    FilePanelOperations *GetOperations() const
    {
        return operations.get();
    }

    // Обработчики событий для контекстного меню
    void OnCopy(wxCommandEvent &event)
    {
        HandleCopyOperation(event);
    }
    void OnMove(wxCommandEvent &event)
    {
        HandleMoveOperation(event);
    }
    void OnPaste(wxCommandEvent &event)
    {
        HandlePasteOperation(event);
    }
    void OnDelete(wxCommandEvent &event)
    {
        HandleDeleteOperation(event);
    }
    void OnCreateFolder(wxCommandEvent &event)
    {
        HandleCreateFolder(event);
    }
    void OnRefresh(wxCommandEvent &event)
    {
        HandleRefresh(event);
    }

    // Обработчики событий
    void OnSourceTypeChanged(wxCommandEvent &);
    void OnPathChanged(wxCommandEvent &);
    void OnItemActivated(wxListEvent &);
    void OnItemSelected(wxListEvent &);
    void OnItemRightClick(wxListEvent &);
    void OnBeginDrag(wxListEvent &);
    void OnPanelClick(wxMouseEvent &);
    void OnPanelFocus(wxFocusEvent &);
    void OnKeyDown(wxKeyEvent &);
    void OnColumnClick(wxListEvent &);

private:
    // Удаляем методы CreateButtonPanel и связанные с кнопками элементы
    void CreateControls();
    void CreateSourceButtons();
    void BindEvents();
    void UpdateVisualState();
    void UpdateControlsForSourceType();
    void UpdatePanelState();  // Обновление видимости элементов в зависимости от состояния
    wxArrayString DetectUSBDrives();

    // Внутренние методы для абстракции операций с файловой системой
    bool ChangeDirectoryInternal(const wxString &path);
    wxString GetCurrentDirectoryInternal() const;
    bool GetDirectoryContentsInternal(wxArrayString &files, wxArrayString &dirs);
    bool CreateDirectoryInternal(const wxString &name);
    bool DeleteFileInternal(const wxString &path);
    bool DeleteDirectoryInternal(const wxString &path);
    bool RenameFileInternal(const wxString &oldPath, const wxString &newPath);
    bool FileExistsInternal(const wxString &path) const;
    bool IsDirectoryInternal(const wxString &path) const;
    bool CopyFileBetweenSystems(const wxString &sourcePath, FileSystemType, const wxString &destPath, FileSystemType destType, wxWindow *parent = nullptr);

    DisplayMode displayMode;
    PanelState panelState{ STATE_SELECTION };   // Текущее состояние панели
    wxComboBox *sourceTypeCombo = nullptr;
    wxBitmapButton *btnLocal = nullptr;
    wxBitmapButton *btnFTP = nullptr;
    wxButton *btnBack = nullptr;                // Кнопка возврата
    wxBoxSizer *buttonsSizer = nullptr;         // Сайзер для кнопок выбора источника
    wxTextCtrl *pathCtrl = nullptr;
    wxButton *browseBtn = nullptr;
    wxListCtrl *fileList = nullptr;
    // Удаляем указатели на кнопки
    std::unique_ptr<FilePanelController> controller;
    std::unique_ptr<FilePanelOperations> operations;
    std::unique_ptr<FTPController> ftpController = nullptr;
    wxString ftpInitialDirectory;  // Начальный каталог FTP для ограничения навигации
    bool is_active{ false };
    SourceType sourceType{ SOURCE_LOCAL };
    int sortColumn = 0;
    bool sortAscending = true;

    wxDECLARE_EVENT_TABLE();
};