// 2026/04/29 15:56:28 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "GUI/PageTests/PanelTests.h"
#include "GUI/Controls/Sizers.h"
#include "Settings/Tests/Library/Library.h"
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

    listView = new wxListView(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_LIST | wxLC_SINGLE_SEL);

    imageList = new wxImageList(16, 16, true, 0);

    listView->SetImageList(imageList, wxIMAGE_LIST_SMALL);

    main_sizer->Add(listView, 1, wxEXPAND | wxALL, 0);

    Panel::SetSizer(main_sizer);

    listView->Bind(wxEVT_LIST_ITEM_RIGHT_CLICK, &PanelTests::OnEventRightClickListItem, this);
    listView->Bind(wxEVT_LEFT_DOWN, &PanelTests::OnEventLeftClickListItem, this);
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


void PanelTests::AddItem(const wxString &text, int iconIndex)
{
    if (!listView)
    {
        return;
    }

    listView->InsertItem(listView->GetItemCount(), text, iconIndex);
}


void PanelTests::AddItem(const wxString &text)
{
    AddItem(text, -1);
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
        for (auto test : lib->tests)
        {
            AddItem(lib->UGO + " : " + test.name);
        }
    }
}


void PanelTests::OnEventRightClickListItem(wxListEvent &event)
{
    long selected = listView->GetFirstSelected();
    if (selected == wxNOT_FOUND)
    {
        return;  // Нет выделенного элемента — меню не показываем
    }

    // Получаем индекс элемента, на котором кликнули
    int index = event.GetIndex();

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
