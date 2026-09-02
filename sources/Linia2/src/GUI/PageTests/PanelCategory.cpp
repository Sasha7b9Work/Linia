// 2026/04/29 11:34:17 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "GUI/PageTests/PanelCategory.h"
#include "GUI/PageTests/PageTests.h"
#include "GUI/Controls/Sizers.h"
#include "GUI/Controls/CheckBox.h"
#include "GUI/PageTests/PanelLibrary.h"
#pragma warning(push, 0)
#include <wx/checkbox.h>
#pragma warning(pop)


PanelCategory *ThePanelCategory = nullptr;


PanelCategory::PanelCategory(wxWindow *parent, PanelCategory *&global) : ScrolledPanel(parent)
{
    global = this;

    BoxSizerVert *main_sizer = new BoxSizerVert();

    chbStandardTests = new CheckBox(this, L("Стандартные тесты"));
    chbUserTests = new CheckBox(this, L("Пользовательские тесты"));

    main_sizer->Add(chbStandardTests);
    main_sizer->Add(chbUserTests);

    ScrolledPanel::SetSizer(main_sizer);

    Bind(wxEVT_CHECKBOX, [this](wxCommandEvent &event)
        {
            BuildListTests();

            event.Skip();
        });
}


void PanelCategory::UpdateState(const Library &lib)
{
    return;
    wxSizer *sizer = GetSizer();

    for (size_t i = 0; i < categories.size(); i++)
    {
        CheckBox *checkbox = categories[i].checkbox;

        sizer->Detach(checkbox);
        checkbox->Destroy();

        sizer->Layout();
        Layout();
    }

    sizer->Clear();

    categories.clear();

    for (size_t i = 0; i < lib.categories.size(); i++)
    {
        const LibraryCategory &cat = lib.categories[i];

        CheckBox *checkbox = new CheckBox(this, cat.name);
        sizer->Add(checkbox);
        categories.emplace_back(Category{ checkbox, cat });
    }

    sizer->AddStretchSpacer();

    sizer->Layout();

    Layout();
}


void PanelCategory::BuildListTests()
{
    std::vector<const LibraryCategory *> cats;

    for (const Category &cat : categories)
    {
        if (cat.checkbox->IsChecked())
        {
            cats.push_back(&cat.category);
        }
    }

    ThePanelLibrary->BuildListTests(cats);
}
