#pragma once
#include "GUI/PageSettings/MyComputer/FilePanel.h"
#pragma warning(push, 0)
    #include <wx/dnd.h>
#pragma warning(pop)


class FileDropTarget : public wxFileDropTarget
{
public:

    FileDropTarget(FilePanel *);

    bool OnDropFiles(wxCoord x, wxCoord y, const wxArrayString &filenames) override;

private:

    FilePanel *panel;
};
