// 2025/6/1 17:14:36 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PanelUpper.h"
#include "MainWindow.h"
#include "Utils/GlobalFunctions.h"
#include "Communicator/ComPort/ComPort.h"
#include "Settings/Settings.h"


PanelUpper *PanelUpper::self = nullptr;


PanelUpper::PanelUpper(wxWindow* parent) :
    Panel(parent, MainWindow::WIDTH1, 0, MainWindow::WIDTH_DRAW - MainWindow::WIDTH1, MainWindow::HEIGHT_HI)
{
    self = this;

    new wxStaticText(this, wxID_ANY, wxString::Format("ver. %d : %s", VERSION_BUILD, DATE_BUILD), { 460, 56 });

    new wxStaticText(this, wxID_ANY, wxString::Format("IP : %s", GF::GetSelfIP().c_str().AsChar()), {10, 56});

#ifdef WIN32

    new wxStaticText(this, wxID_ANY, "COM-порт", { 150, 56 });

    comboPorts = new wxComboBox(this, wxID_ANY, "", { 220, 45 });

    std::vector<bool> ports;

    ComPort::GetComports(ports);

    for (size_t i = 0; i < ports.size(); i++)
    {
        if (ports[i])
        {
            comboPorts->Append(wxString::Format("COM%d", (int)i + 1));
        }
    }

    SetTunedPort();

    Bind(wxEVT_COMBOBOX, &PanelUpper::OnEventComboBox, this);

#endif
}


#ifdef WIN32

void PanelUpper::OnEventComboBox(wxCommandEvent &event)
{
    if (event.GetId() == comboPorts->GetId())
    {

    }
}


void PanelUpper::SetTunedPort()
{
    wxString pattern_end = wxString::Format("%d", SET::GUI::serial_port_num.Get());

    for (uint i = 0; i < comboPorts->GetCount(); i++)
    {
        wxString item = comboPorts->GetString(i);

        if (item.EndsWith(pattern_end))
        {
            comboPorts->SetSelection((int)i);
            return;
        }
    }

    if (comboPorts->GetCount())
    {
        comboPorts->SetSelection(0);
    }
}

#endif
