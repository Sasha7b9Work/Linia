#pragma once
#pragma warning(push, 0)
#include <wx/arrstr.h>
#pragma warning(pop)


// Forward declarations
class FilePanel;
class FTPController;

// Объявляем enum вне класса для избежания циклических зависимостей
enum FilePanelSourceType {
    SOURCE_TYPE_LOCAL,
    SOURCE_TYPE_USB,
    SOURCE_TYPE_FTP
};

class ClipboardManager {
public:
    struct ClipboardData {
        wxArrayString files;
        wxString sourcePath;
        bool isCut = false;
        FilePanelSourceType sourceType = SOURCE_TYPE_LOCAL;
        FTPController *sourceController = nullptr;  // Типизированный указатель на FTPController

        bool IsEmpty() const
        {
            return files.IsEmpty();
        }
        void Clear()
        {
            files.Clear();
            sourcePath.Clear();
            isCut = false;
            sourceType = SOURCE_TYPE_LOCAL;
            sourceController = nullptr;
        }
    };

    static ClipboardManager &GetInstance();

    void SetData(const ClipboardData &_data)
    {
        data = _data;
    }
    ClipboardData GetData() const
    {
        return data;
    }
    bool HasData() const
    {
        return !data.IsEmpty();
    }
    void Clear()
    {
        data.Clear();
    }

    wxString GetOperationDescription() const;

private:
    ClipboardManager() = default;
    ~ClipboardManager() = default;

    ClipboardData data;
};
