// 2026/04/29 15:55:25 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once
#include "GUI/Controls/Panel.h"
#include "GUI/Controls/ListView.h"
#include "Settings/Tests/Library/Library.h"


class LibraryCategory;
class wxListView;
class wxImageList;


// Здесь находятся тесты из выбранных категорий

class PanelTests : public Panel
{
public:

    PanelTests(wxWindow *);
    ~PanelTests();

    static PanelTests *self;

    void BuildListTests(std::vector<const LibraryCategory *> &libraries);

    // Очистить весь список
    void ClearList();

    // Добавить элемент с иконкой
    // Через эту функцию добавляются тесты по галочке категории
    void AddItem(const wxString &, int iconIndex, const Test *);

    // Загрузить иконку в imageList
    int AddIcon(const wxString &iconPath);

    // Установить иконку существующему элементу
    void SetItemIcon(long itemIndex, int iconIndex);

private:

    ListView    *listView = nullptr;
    wxImageList *imageList = nullptr;

    void OnEventRightClickListItem(wxListEvent &);
    void OnEventLeftClickListItem(wxMouseEvent &);
    void OnEventMouseMove(wxMouseEvent &);
    void OnEventMenu(wxCommandEvent &);

    Test *test = nullptr;

    enum
    {
        MENU_APPLY = 10001    // "Применить"
    };
};
