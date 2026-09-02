// 2026/04/29 15:55:25 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once
#include "GUI/Controls/Panel.h"
#pragma warning(push, 0)
    #include <wx/listbase.h>
#pragma warning(pop)


class LibraryCategory;
class wxListView;
class wxImageList;


// Здесь находятся тесты из выбранных категорий

class PanelTests : public Panel
{
public:

    PanelTests(wxWindow *, PanelTests *&);
    ~PanelTests();

    void BuildListTests(std::vector<const LibraryCategory *> &libraries);

    void ClearList();                                   // Очистить весь список
    void AddItem(const wxString &);                     // Добавить элемент без иконки
    void AddItem(const wxString &, int iconIndex);      // Добавить элемент с иконкой
    int AddIcon(const wxString &iconPath);              // Загрузить иконку в imageList
    void SetItemIcon(long itemIndex, int iconIndex);    // Установить иконку существующему элементу

private:

    wxListView *listView = nullptr;
    wxImageList *imageList = nullptr;

    void OnEventRightClickListItem(wxListEvent &);
};
