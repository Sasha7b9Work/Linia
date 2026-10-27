// 2025/8/9 09:54:32 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once
#include "GUI/Controls/ButtonCombo.h"
#include "GUI/Controls/BmpButtonsCombo.h"
#include "GUI/Controls/Notebook.h"
#include "GUI/Controls/Splitter.h"
#include "Settings/Tests/SettingsTests.h"
#include "Settings/Tests/Library/Library.h"
#include "GUI/PageTests/PanelTests.h"
#include "GUI/PageTests/PanelViewTest.h"
#include "GUI/PageTests/PanelCategory.h"


class PageTests : public PageNotebook
{
public:

    PageTests(Notebook *);

    static PageTests *self;

    ~PageTests();

private:

    Splitter *hor_splitter = nullptr;           // Этот сплиттер в горизонтальной плоскости
    Splitter *vert_splitter = nullptr;          // Левый сплиттер (верх/низ)

    PanelCategory *panel_category = nullptr;
    PanelTests  *panel_library = nullptr;
    PanelViewTest *panel_view_test = nullptr;

    Library library;                            // Здесь находятся тесты, которые можно загрузить

    // Загружает главную библиотеку из json-файла
    void LoadStandardLibrary();
};
 