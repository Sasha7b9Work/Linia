// 2026/04/29 15:56:28 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "GUI/PageTests/PanelTests.h"
#include "GUI/Controls/Sizers.h"
#include "Settings/Tests/Library/Library.h"
#include "GUI/PageTests/PanelViewTest.h"
#pragma warning(push, 0)
    #include <wx/checkbox.h>
    #include <wx/listctrl.h>
    #include <wx/menu.h>
    #include <wx/msgdlg.h>
#pragma warning(pop)


PanelTests *ThePanelTests = nullptr;


PanelTests::PanelTests(wxWindow *parent, PanelTests *&global) : Panel(parent, wxSIMPLE_BORDER)
{
    global = this;

    BoxSizerVert *main_sizer = new BoxSizerVert();

    listView = new ListView(this);

    imageList = new wxImageList(16, 16, true, 0);

    listView->SetImageList(imageList, wxIMAGE_LIST_SMALL);

    main_sizer->Add(listView, 1, wxEXPAND | wxALL, 0);

    Panel::SetSizer(main_sizer);

    listView->Bind(wxEVT_LIST_ITEM_RIGHT_CLICK, &PanelTests::OnEventRightClickListItem, this);
    listView->Bind(wxEVT_MOTION, &PanelTests::OnEventMouseMove, this);
//    listView->Bind(wxEVT_LEFT_DOWN, &PanelTests::OnEventLeftClickListItem, this);
}


PanelTests::~PanelTests()
{
    if (imageList)
    {
        delete imageList;
    }
}


void PanelTests::ClearList()
{
    if (listView)
    {
        listView->DeleteAllItems();
    }
}


void PanelTests::AddItem(const wxString &text, int iconIndex, const Test *test)
{
    if (!listView)
    {
        return;
    }

    long index = listView->InsertItem(listView->GetItemCount(), text, iconIndex);

    listView->SetUserData(index, (void *)test);
}


int PanelTests::AddIcon(const wxString &iconPath)
{
    if (!imageList) return -1;

    wxBitmap bitmap(iconPath, wxBITMAP_TYPE_PNG);

    if (bitmap.IsOk())
    {
        return imageList->Add(bitmap);
    }
    return -1;
}


void PanelTests::SetItemIcon(long itemIndex, int iconIndex)
{
    if (listView && itemIndex >= 0 && itemIndex < listView->GetItemCount())
    {
        listView->SetItemImage(itemIndex, iconIndex);
    }
}


void PanelTests::BuildListTests(std::vector<const LibraryCategory *> &libraries)
{
    ClearList();

    for (auto lib : libraries)
    {
        for (const Test *test : lib->tests)
        {
            AddItem(lib->UGO + " : " + test->UGO + " : " + test->name, -1, test);
        }
    }
}


void PanelTests::OnEventRightClickListItem(wxListEvent &event)
{
    ListView *list = (ListView *)event.GetEventObject();

    if (list->GetFirstSelected() == wxNOT_FOUND)
    {
        return;
    }

    long index = event.GetIndex();

    Test *test = (Test *)list->GetUserData(index);

    wxMenu menu;

    wxMenuItem *header = new wxMenuItem(&menu, wxID_ANY, test->lib->UGO + " : " + test->name);
    menu.Append(header);
    header->Enable(false);

    menu.Append(1001, L("Применить"));

    menu.Bind(wxEVT_MENU, [this, test](wxCommandEvent &e)
        {
            switch (e.GetId())
            {
            case 1001:

                ThePanelViewTest->SetTest(test);

                break;
            }
        });

    PopupMenu(&menu);
}


void PanelTests::OnEventMouseMove(wxMouseEvent &event)
{
    wxPoint pos = event.GetPosition();
    int flags = 0;
    int index = listView->HitTest(pos, flags);

    if (index != wxNOT_FOUND)
    {
        ListView *list = (ListView *)event.GetEventObject();

        Test *test = (Test *)list->GetUserData(index);

        wxString tooltip = test->lib->UGO + " : " + test->UGO + " : " + test->name;
        listView->SetToolTip(tooltip);
    }
    else
    {
        listView->SetToolTip(wxEmptyString);
    }

    event.Skip();
}


void PanelTests::OnEventLeftClickListItem(wxMouseEvent &event)
{
    wxPoint pos = event.GetPosition();
    int flags = 0;
    int index = listView->HitTest(pos, flags);

    if (index == wxNOT_FOUND)
    {
        event.Skip();
        return;
    }

    listView->Select(index);

    wxMenu menu;
    menu.Append(1001, L("Активировать"));

    // Привязываем обработчики
    menu.Bind(wxEVT_MENU, [this, index](wxCommandEvent &e)
        {
            switch (e.GetId())
            {
            case 1001:
                wxMessageBox(wxString::Format("Действие 1 для элемента %d", index));
                break;
            }
        });

    // Показываем меню в позиции курсора
    PopupMenu(&menu);

    event.Skip();
}
