// 2025/6/1 16:45:24 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel01_Name.h"
#include "MainWindow.h"


PanelName *PanelName::self = nullptr;


PanelName::PanelName(wxWindow *parent) :
    Panel(parent, 0, 0, MainWindow::WIDTH1, MainWindow::HEIGTH1)
{
    self = this;

    new wxStaticText(this, wxID_ANY, _L("ИППП 4"), { 0, 10 }, { GetSize().x, 20 }, wxALIGN_CENTRE_HORIZONTAL);

    textDate = new wxStaticText(this, wxID_ANY, "", {0, 30}, {GetSize().x, 20}, wxALIGN_CENTRE_HORIZONTAL);

    textTime = new wxStaticText(this, wxID_ANY, "", {0, 50}, {GetSize().x, 20}, wxALIGN_CENTRE_HORIZONTAL);

    Bind(wxEVT_TIMER, &PanelName::OnTimer, this, ID_TIMER);

    timer.SetOwner(this, ID_TIMER);

    timer.Start(100);
}


void PanelName::WriteDateTime()
{
    wxDateTime d = wxDateTime::Now();

    textDate->SetLabel(wxString::Format("%02d-%02d-%d", d.GetDay(), d.GetMonth() + 1, d.GetYear()));

    textTime->SetLabel(wxString::Format("%02d:%02d:%02d", d.GetHour(), d.GetMinute(), d.GetSecond()));
}


void PanelName::OnTimer(wxTimerEvent &event)
{
    if (event.GetId() == ID_TIMER)
    {
        WriteDateTime();
    }
}
