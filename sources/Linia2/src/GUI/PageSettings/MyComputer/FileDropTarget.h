#pragma once
#include "GUI/PageSettings/MyComputer/FilePanel.h"
#pragma warning(push, 0)
    #include <wx/dnd.h>
#pragma warning(pop)


class FileDropTarget : public wxFileDropTarget {
public:
    FileDropTarget(FilePanel *panel);
    bool OnDropFiles(wxCoord x, wxCoord y, const wxArrayString &filenames) override;

private:
    FilePanel *m_panel;
};
