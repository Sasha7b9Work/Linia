#pragma once

#include "ClipboardManager.h"
#include <wx/event.h>

class FilePanel;

class FilePanelOperations {
public:
    explicit FilePanelOperations(FilePanel* panel);

    void HandleCopyOperation(wxCommandEvent& event);
    void HandleMoveOperation(wxCommandEvent& event);
    void HandlePasteOperation(wxCommandEvent& event);
    void HandlePasteOperationToTarget(FilePanel* targetPanel);
    void HandleFTPPasteOperation(const ClipboardManager::ClipboardData& data,
                                 FilePanel* destPanel, bool fromFTP, bool toFTP);
    void HandleDeleteOperation(wxCommandEvent& event);
    void HandleCreateFolder(wxCommandEvent& event);
    void HandleRefresh(wxCommandEvent& event);
    void HandleUndo();
    void HandleRedo();

private:
    FilePanel* m_panel;
};
