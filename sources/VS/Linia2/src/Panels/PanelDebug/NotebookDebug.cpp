// 2025/6/3 14:06:23 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PanelDebug/NotebookDebug.h"
#include "Panels/PanelDebug/PageTestsGPIO.h"
#include "Panels/PanelDebug/PageTestsFPGA.h"
#include "Panels/PanelDebug/PagesBlocks/PageSource3kV.h"
#include "Panels/PanelDebug/PagesBlocks/PageCommutator.h"
#include "Panels/PanelDebug/PagesBlocks/PageChannelC.h"
#include "Panels/PanelDebug/PagesBlocks/PageChannelB.h"
#include "Panels/PanelDebug/PagesBlocks/PageChannelS.h"
#include "Panels/PanelDebug/PagesBlocks/PageMeasCurrent.h"
#include "Panels/PanelDebug/PagesBlocks/PageSource50V.h"


NotebookDebug *NotebookDebug::self = nullptr;


NotebookDebug::NotebookDebug(wxWindow *parent) :
    wxNotebook(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBK_TOP)
{
    self = this;

    AppendNewPage(new PageTestsGPIO(this));
    AppendNewPage(new PageTestsFPGA(this));
    AppendNewPage(new PageCommutator(this));
    AppendNewPage(new PageSource3kV(this));
    AppendNewPage(new PageChannelC(this));
    AppendNewPage(new PageChannelB(this));
    AppendNewPage(new PageChannelS(this));
    AppendNewPage(new PageMeasCurrent(this));
    AppendNewPage(new PageSource50V(this));

    wxWindowBase::Layout();
}


void NotebookDebug::AppendNewPage(wxPanel *page)
{
    wxNotebook::AddPage(page, page->GetName());
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


void NotebookDebug::Pack()
{
    PageTestsGPIO::self->Pack();
    PageTestsFPGA::self->Pack();
    PageCommutator::self->Pack();
    PageSource3kV::self->Pack();
    PageChannelC::self->Pack();
    PageChannelB::self->Pack();
    PageChannelS::self->Pack();
    PageMeasCurrent::self->Pack();
    PageSource50V::self->Pack();
}


void NotebookDebug::Unpack()
{
    PageTestsGPIO::self->Unpack();
    PageTestsFPGA::self->Unpack();
    PageCommutator::self->Unpack();
    PageSource3kV::self->Unpack();
    PageChannelC::self->Unpack();
    PageChannelB::self->Unpack();
    PageChannelS::self->Unpack();
    PageMeasCurrent::self->Unpack();
    PageSource50V::self->Unpack();
}
