// WindowMyComputer.h
#pragma once

#include "Controls/Dialog.h"
#include <wx/wx.h>
#include <wx/splitter.h>
#include "MyComputer/FilePanel.h"
#include "MyComputer/FilePanelEvents.h"

class WindowMyComputer : public Dialog {
public:
    static const int WIDTH = 1024;
    static const int HEIGHT = 768;

    WindowMyComputer();
    virtual ~WindowMyComputer();
    
    FilePanel* GetLeftPanel() const { return m_leftPanel; }
    FilePanel* GetRightPanel() const { return m_rightPanel; }
    wxStaticText* GetStatusText() const { return m_statusText; }
    void SetActivePanel(FilePanel* panel);
    void UpdateStatus(const wxString& message);

    // Обработчики событий
    void OnTabKey(wxKeyEvent& event);
    void OnClose(wxCommandEvent& event);
    void OnStatusUpdate(wxCommandEvent& event);
    void OnPanelActivate(wxCommandEvent& event);

private:
    void CreateControls();
    void SetupEventHandlers();

    FilePanel* GetActivePanel() const { return m_activePanel; }

    wxSplitterWindow* m_splitter;
    FilePanel* m_leftPanel;
    FilePanel* m_rightPanel;
    wxStaticText* m_statusText;
    FilePanel* m_activePanel;
};