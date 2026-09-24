// 2025/6/3 13:44:28 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "GUI/PageDebug/PageDebug.h"
#include "MainWindow.h"
#include "GUI/PageDebug/Notebook/NotebookDebug.h"
#include "GUI/PageDebug/PanelRight.h"
#pragma warning(push, 0)
#include <wx/sizer.h>
#pragma warning(pop)


PageDebug *PageDebug::self = nullptr;


PageDebug::PageDebug(Notebook *board) : PageNotebook(board, L("Отладка"))
{
    self = this;

    wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);

    sizer->Add(new NotebookDebug(this));

    sizer->Add(new PanelRight(this));

    SetSizer(sizer);

    PageNotebook::Layout();

    labelButtonStart = new wxStaticText(this, wxID_ANY, "Start", { 10, 700 }, { 100, 20 });
    labelButtonStop = new wxStaticText(this, wxID_ANY, "Stop", { 210, 700 }, { 100, 20 });
    labelEncoder = new wxStaticText(this, wxID_ANY, "0", { 410, 700 }, { 100, 20 });

    labelIP = new wxStaticText(this, wxID_ANY, wxString{ "IP : " } + GF::GetSelfIP(), { 10, 670 }, { 100, 20 });
    labelSTM32 = new wxStaticText(this, wxID_ANY, "", { 200, 670 }, { 100, 20 });
}


bool PageDebug::Show(bool show)
{
    show ? NotebookDebug::self->Init() : NotebookDebug::self->DeInit();

    return wxPanel::Show(show);
}
