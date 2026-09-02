// 2026/04/29 15:56:28 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "GUI/PageTests/PanelTests.h"
#include "GUI/Controls/Sizers.h"
#include "Settings/Tests/Library/Library.h"
#pragma warning(push, 0)
#include <wx/checkbox.h>
#include <wx/listctrl.h>
#pragma warning(pop)


PanelLibrary *ThePanelLibrary = nullptr;


PanelLibrary::PanelLibrary(wxWindow *parent, PanelLibrary *&global) : Panel(parent, wxSIMPLE_BORDER)
{
    global = this;

    BoxSizerVert *main_sizer = new BoxSizerVert();

    listView = new wxListView(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_LIST | wxLC_SINGLE_SEL);

    imageList = new wxImageList(16, 16, true, 0);

    listView->SetImageList(imageList, wxIMAGE_LIST_SMALL);

    main_sizer->Add(listView, 1, wxEXPAND | wxALL, 0);

    Panel::SetSizer(main_sizer);
}


PanelLibrary::~PanelLibrary()
{
    if (imageList)
    {
        delete imageList;
    }
}


void PanelLibrary::ClearList()
{
    if (listView)
    {
        listView->DeleteAllItems();
    }
}


void PanelLibrary::AddItem(const wxString &text, int iconIndex)
{
    if (!listView) return;

    listView->InsertItem(listView->GetItemCount(), text, iconIndex);
}


void PanelLibrary::AddItem(const wxString &text)
{
    AddItem(text, -1);
}


int PanelLibrary::AddIcon(const wxString &iconPath)
{
    if (!imageList) return -1;

    wxBitmap bitmap(iconPath, wxBITMAP_TYPE_PNG);

    if (bitmap.IsOk())
    {
        return imageList->Add(bitmap);
    }
    return -1;
}


void PanelLibrary::SetItemIcon(long itemIndex, int iconIndex)
{
    if (listView && itemIndex >= 0 && itemIndex < listView->GetItemCount())
    {
        listView->SetItemImage(itemIndex, iconIndex);
    }
}


void PanelLibrary::BuildListTests(std::vector<const LibraryCategory *> &libraries)
{
    ClearList();

    for (auto lib : libraries)
    {
        for (auto test : lib->tests)
        {
            AddItem(lib->UGO + " : " + test.name);
        }
    }
}
