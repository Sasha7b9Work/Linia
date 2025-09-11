// 2025/6/1 16:45:24 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PanelName.h"
#include "MainWindow.h"


PanelName *PanelName::self = nullptr;


PanelName::PanelName(wxWindow *parent) :
    Panel(parent, 0, 0, MainWindow::WIDTH1, MainWindow::HEIGHT1)
{
    self = this;

    new wxStaticText(this, wxID_ANY, "ИППП 4", { 0, 10 }, { Panel::GetSize().x, 20 }, wxALIGN_CENTRE_HORIZONTAL);

    textDate = new wxStaticText(this, wxID_ANY, "", { 0, 30 }, { Panel::GetSize().x, 20 }, wxALIGN_CENTRE_HORIZONTAL);

    textTime = new wxStaticText(this, wxID_ANY, "", { 0, 50 }, { Panel::GetSize().x, 20 }, wxALIGN_CENTRE_HORIZONTAL);

    Bind(wxEVT_TIMER, &PanelName::OnTimer, this, timer.GetId());

    timer.SetOwner(this, timer.GetId());

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
    if (event.GetId() == timer.GetId())
    {
        WriteDateTime();
    }
}
