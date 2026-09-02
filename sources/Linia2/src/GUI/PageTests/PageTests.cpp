// 2025/8/9 09:54:44 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "GUI/Controls/Notebook.h"
#include "GUI/PageTests/PageTests.h"
#include "GUI/PageTests/PanelCategory.h"
#include "GUI/PageTests/PanelTests.h"
#include "GUI/PageTests/PanelViewTest.h"
#include "MainWindow.h"
#include "Utils/SystemDepend.h"
#include "GUI/Controls/Bitmap.h"
#include "GUI/Controls/StaticBox.h"
#include "System/Events.h"
#include "GUI/Controls/Splitter.h"
#include "GUI/Controls/Panel.h"
#include "Settings/FileJSON.h"
#include "Settings/Settings.h"
#pragma warning(push, 0)
    #include <wx/sizer.h>
    #include <wx/msgdlg.h>
#pragma warning(pop)


PageTests *ThePageTests = nullptr;


PageTests::PageTests(Notebook *board, PageTests *&global) :
    PageNotebook(board, L("Тесты"))
{
#define SASH_POSITION_VERTICAL "page_tests_sash_postion_vertical"
#define SASH_POSITION_HORIZONTAL "page_tests_sash_postion_horizontal"

    REGISTER_AND_LOAD(int, SASH_POSITION_VERTICAL, 200);
    REGISTER_AND_LOAD(int, SASH_POSITION_HORIZONTAL, 100);

    global = this;

    wxPanel::SetName("PageTests");

    hor_splitter = new Splitter(this, wxSP_3D | wxSP_LIVE_UPDATE);
    hor_splitter->SetMinimumPaneSize(50);

    vert_splitter = new Splitter(hor_splitter, wxSP_3D | wxSP_LIVE_UPDATE);
    vert_splitter->SetMinimumPaneSize(50);

    panel_category = new PanelCategory(vert_splitter, ThePanelCategory);

    panel_library = new PanelTests(vert_splitter, ThePanelTests);

    panel_view_test = new PanelViewTest(hor_splitter, ThePanelViewTest);

    vert_splitter->SplitHorizontally(panel_category, panel_library, SETTING(SASH_POSITION_VERTICAL)->GetInt());

    hor_splitter->SplitVertically(vert_splitter, panel_view_test, SETTING(SASH_POSITION_HORIZONTAL)->GetInt());

    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(hor_splitter, 1, wxEXPAND | wxALL);
    SetSizer(sizer);

    Layout();

    LoadStandardLibrary();
}


PageTests::~PageTests()
{
    SETTING(SASH_POSITION_VERTICAL)->Set(vert_splitter->GetSashPosition());
    SETTING(SASH_POSITION_HORIZONTAL)->Set(hor_splitter->GetSashPosition());
}


void PageTests::LoadStandardLibrary()
{
    FileJSON file;

    if (file.Load("library_std.json"))
    {
        if (library.Read(&file))
        {
            panel_category->UpdateState(library);
        }
    }
    else
    {
        wxMessageBox(wxString::Format(L("Нет файла со стандартными тестами \"library_std.json\""), L("Ошибка"), wxOK | wxCENTRE | wxICON_WARNING));
    }
}
