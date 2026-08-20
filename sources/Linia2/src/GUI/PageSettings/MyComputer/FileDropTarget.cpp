#include "defines.h"
#include "GUI/PageSettings/MyComputer/FileDropTarget.h"
#include "GUI/PageSettings/MyComputer/FilePanelController.h"
#include "GUI/PageSettings/MyComputer/FileOperations.h"
#include "GUI/PageSettings/MyComputer/ClipboardManager.h"


FileDropTarget::FileDropTarget(FilePanel *_panel) :
    panel(_panel)
{
}


bool FileDropTarget::OnDropFiles(wxCoord /*x*/, wxCoord /*y*/, const wxArrayString &filenames)
{
    if (filenames.IsEmpty())
    {
        return false;
    }

    // Получаем исходный путь из первого файла
    wxFileName firstFile(filenames[0]);
    wxString sourcePath = firstFile.GetPath();

    // Создаем данные для буфера обмена
    ClipboardManager::ClipboardData data;
    data.sourcePath = sourcePath;
    data.isCut = false;

    for (const wxString &fullPath : filenames)
    {
        if (fullPath.IsEmpty()) continue;
        wxFileName fn(fullPath);
        data.files.Add(fn.GetFullName());
    }

    // Временно помещаем в буфер и вставляем
    ClipboardManager::GetInstance().SetData(data);
    panel->HandlePasteOperationToTarget(panel);

    return true;
}
