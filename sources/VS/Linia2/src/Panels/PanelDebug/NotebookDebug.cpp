// 2025/6/3 14:06:23 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PanelDebug/NotebookDebug.h"
#include "Panels/PanelDebug/PageTestsGPIO.h"
#include "Panels/PanelDebug/PagesBlocks/PageSource3kV.h"
#include "Panels/PanelDebug/PagesBlocks/PageCommutator.h"


NotebookDebug *NotebookDebug::self = nullptr;


NotebookDebug::NotebookDebug(wxWindow *parent) :
    wxNotebook(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBK_TOP)
{
    self = this;

    new PageTestsGPIO(this);
    new PageSource3kV(this);
    new PageCommutator(this);

    wxNotebook::AddPage(PageTestsGPIO::self, PageTestsGPIO::self->GetName());
    wxNotebook::AddPage(PageSource3kV::self, PageSource3kV::self->GetName());
    wxNotebook::AddPage(PageCommutator::self, PageCommutator::self->GetName());

    wxWindowBase::Layout();
}


void NotebookDebug::Init()
{
    PageTestsGPIO::self->Init();
}


void NotebookDebug::DeInit()
{
    PageTestsGPIO::self->DeInit();
}


void NotebookDebug::Update()
{
    PageTestsGPIO::self->Update();
}

