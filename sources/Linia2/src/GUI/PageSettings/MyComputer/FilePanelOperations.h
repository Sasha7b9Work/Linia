#pragma once
#include "GUI/PageSettings/MyComputer/ClipboardManager.h"
#pragma warning(push, 0)
    #include <wx/event.h>
#pragma warning(pop)


class FilePanel;


class FilePanelOperations
{
public:
    explicit FilePanelOperations(FilePanel *);

    void HandleCopyOperation(wxCommandEvent &);
    void HandleMoveOperation(wxCommandEvent &);
    void HandlePasteOperation(wxCommandEvent &);
    void HandlePasteOperationToTarget(FilePanel *targetPanel);
    void HandleFTPPasteOperation(const ClipboardManager::ClipboardData &, FilePanel *destPanel, bool fromFTP, bool toFTP);
    void HandleDeleteOperation(wxCommandEvent &);
    void HandleCreateFolder(wxCommandEvent &);
    void HandleRefresh(wxCommandEvent &);
    void HandleUndo();
    void HandleRedo();

private:
    FilePanel *panel;
};
