// 2023/07/04 17:46:31 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Notebook/Pages.h"
#include "Reader/Reader.h"
#include "Communicator/ComPort.h"
#include "Panels/ConsoleRS232.h"


Page::Page(wxNotebook *parent, const wxString &title) :
    wxPanel(parent, wxID_ANY)
{
    wxPanel::SetName(title);

    wxPanel::SetMinClientSize({ 400, 1000 });

    wxPanel::SetBackgroundColour(parent->GetBackgroundColour());
}


int Page::PositionButton(int i)
{
    return 10 + (i - 1) * 130;
}


void Page::DisableProtection()
{
    Reader::Send("#write 41 04 00 00 FF");

    ComPort::WaitWord(1, "Task");
}


void Page::DrawDemarcationLine(int y)
{
    new wxStaticText(this, wxID_ANY, "..........................................................................................", { FIRST_X, y });
}
