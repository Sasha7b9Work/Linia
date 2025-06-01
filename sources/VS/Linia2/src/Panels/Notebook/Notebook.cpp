// 2023/07/04 17:46:31 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Notebook/Notebook.h"
#include "Utils/Configurator.h"
#include "Panels/CommonPanel.h"
#include "Utils/GlobalFunctions.h"
#include "Settings/Settings.h"
#include "Panels/Notebook/PageUser/PageUser.h"
#include "Panels/Notebook/PageMaster/PageMaster.h"


Notebook *Notebook::self = nullptr;


Notebook::Notebook(wxWindow *parent) :
    wxNotebook(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBK_TOP)
{
    self = this;

    pageUser = new PageUser(this);
    pageMaster = new PageMaster(this);

    wxNotebook::AddPage(pageMaster, pageMaster->GetName());
    wxNotebook::AddPage(pageUser, pageUser->GetName());

    wxWindowBase::Layout();

    {
        uint current_page = (uint)SET::GUI::current_page_notebook.Get();

        wxNotebook::SetSelection(current_page < 2 ? current_page : 0);
    }

    Bind(wxEVT_NOTEBOOK_PAGE_CHANGED, &Notebook::OnEventNotebookPageChanged, this);
}


Notebook::~Notebook()
{
    SET::GUI::current_page_notebook.Set(wxNotebook::GetSelection());
}


void Notebook::Preprocess()
{
    PageMaster::self->OnChangeSettings();
}


bool Notebook::ShowAppendPages()
{
    if (GetPageCount() < 4)
    {
        return true;
    }
    else
    {
        wxNotebook::DeletePage(5);
        wxNotebook::DeletePage(4);
        wxNotebook::DeletePage(3);
        wxNotebook::DeletePage(2);

        return false;
    }
}


void Notebook::CallbackOnEventSize(const wxSize &size)
{
    PageUser::self->CallbackOnSizeEvent(size);
}


void Notebook::OnEventNotebookPageChanged(wxBookCtrlEvent &event)
{
    int page = event.GetSelection();

    if (page == 0)
    {
        GF::FindAnyButton(CommonPanel::self, ID_BUTTON_PASSWORD_MASTER_CARD)->Enable(Card::IsInserted());
    }
    else if (page == 1)
    {
        GF::FindAnyButton(CommonPanel::self, ID_BUTTON_PASSWORD_MASTER_CARD)->Enable(false);
        PageUser::self->OnEventEnterPage();
    }
}
