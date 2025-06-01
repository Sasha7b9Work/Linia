// 2023/07/04 17:46:31 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Panels/Notebook/PageUser/PageUser.h"
#include "Panels/Notebook/PageMaster/PageMaster.h"
#pragma warning(push, 0)
#define WIN32_LEAN_AND_MEAN
#include <wx/notebook.h>
#pragma warning(pop)


class Notebook : public wxNotebook
{
public:

    Notebook(wxWindow *parent);
    virtual ~Notebook();

    static Notebook *self;

    void Preprocess();

    bool ShowAppendPages();

    void CallbackOnEventSize(const wxSize &);

private:

    PageUser *pageUser = nullptr;
    PageMaster *pageMaster = nullptr;

    void OnEventNotebookPageChanged(wxBookCtrlEvent &);
};
