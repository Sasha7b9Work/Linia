// 2024/11/01 14:17:12 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Dialogs/SettingsSerialPort.h"
#include "Settings/Settings.h"
#include "Utils/GlobalFunctions.h"
#include "Communicator/ComPort.h"
#include "Utils/StringUtils.h"
#include "Utils/SystemDepend.h"


SettingsSerialPort *SettingsSerialPort::self = nullptr;


void SettingsSerialPort::Create()
{
    self = new SettingsSerialPort();
}


void SettingsSerialPort::Delete()
{
    SAFE_DELETE(self);
}


SettingsSerialPort::SettingsSerialPort() :
    AutoMovedDialog(_L("Настройки COM-порта"))
{
    wxWindow *window = new wxWindow(this, wxID_ANY);

    const int width = BUTTON_SIZE_SMALL.GetWidth() * 2 + 10;

    const int d = 3;

    new CheckButton(window, ID_CHECKBUTTON_COMPORT_AUTOFIND, _L("Автоматический поиск"), { 10, 10 }, { width, BUTTON_SIZE_SMALL.GetHeight() });
    GF::FindCheckButton(this, ID_CHECKBUTTON_COMPORT_AUTOFIND)->SetToolTip(_L("Hint auto find port"));

    new wxComboBox(window, ID_COMBOBOX_NUMBER_PORT, "", { 10, 55 - d }, { 80, BUTTON_SIZE_SMALL.GetHeight() });

    new wxStaticText(window, ID_STATICTEXT_NUMBER_PORT, _L("Номер порта"), { 10 + 100, 55 + 3 - d }, { width - 100, BUTTON_SIZE_SMALL.GetHeight() });

    new wxButton(window, ID_BUTTON_FIND_PORTS, _L("Найти порты"), { 10, 90 }, { width, BUTTON_SIZE_SMALL.GetHeight() });

    const int y = 135;

    new wxButton(window, wxID_CANCEL, _L_CLOSE, { 10, y }, BUTTON_SIZE_SMALL);

    new wxButton(window, wxID_OK, _L_SAVE, { 10 + BUTTON_SIZE_SMALL.GetWidth() + 10, y }, BUTTON_SIZE_SMALL);

    Bind(wxEVT_TOGGLEBUTTON, &SettingsSerialPort::OnEventToggleButton, this);
    Bind(wxEVT_COMBOBOX, &SettingsSerialPort::OnEventComboBox, this);
    Bind(wxEVT_BUTTON, &SettingsSerialPort::OnEventButton, this);

    AutoMovedDialog::SetFixedSize({ 30 + 2 * BUTTON_SIZE_SMALL.GetWidth(), 185 });
}


int SettingsSerialPort::ShowModal()
{
    GF::FindCheckButton(this, ID_CHECKBUTTON_COMPORT_AUTOFIND)->SetValue(SET::GUI::serial_port_auto_find.Get());

    GF::SendCommandEvent(this, wxEVT_TOGGLEBUTTON, ID_CHECKBUTTON_COMPORT_AUTOFIND, SET::GUI::serial_port_auto_find.Get() ? 1 : 0);

    return AutoMovedDialog::ShowModal();
}


void SettingsSerialPort::OnEventToggleButton(wxCommandEvent &event)
{
    const bool checked = event.IsChecked();

    if (event.GetId() == ID_CHECKBUTTON_COMPORT_AUTOFIND)
    {
        FindWindow(ID_COMBOBOX_NUMBER_PORT)->Enable(!checked);
        FindWindow(ID_STATICTEXT_NUMBER_PORT)->Enable(!checked);
        FindWindow(ID_BUTTON_FIND_PORTS)->Enable(!checked);

        if (!checked)
        {
            GF::SendCommandEvent(this, wxEVT_BUTTON, ID_BUTTON_FIND_PORTS);
        }
    }
}


void SettingsSerialPort::OnEventButton(wxCommandEvent &event)
{
    int id = event.GetId();

    if (id == ID_BUTTON_FIND_PORTS)
    {
        wxComboBox *box = GF::FindComboBox(this, ID_COMBOBOX_NUMBER_PORT);

        box->Clear();

        ComPort::GetComports(ports);

        for(uint i = 0; i < ports.size(); i++)
        {
            if (ports[i])
            {
                box->Append(wxString::Format("COM%u", i + 1));
            }
        }

        if (box->GetCount())
        {
            box->SetSelection(0);
        }
    }
    else if (id == wxID_OK)
    {
        SET::GUI::serial_port_auto_find.SetFromControl();

        if (!SET::GUI::serial_port_auto_find.Get())
        {
            SaveNumberPort();
        }
    }
    else if (id == wxID_CANCEL)
    {
        SET::GUI::serial_port_auto_find.LoadToControl();
    }

    event.Skip();
}


void SettingsSerialPort::OnEventComboBox(wxCommandEvent &event)
{
    if (event.GetId() == ID_COMBOBOX_NUMBER_PORT)
    {
        SaveNumberPort();
    }
}


void SettingsSerialPort::SaveNumberPort()
{
    wxComboBox *box = GF::FindComboBox(this, ID_COMBOBOX_NUMBER_PORT);

    wxString text = box->GetValue();

    SET::GUI::serial_port_num.Set(10000);

    if (text.Length() > 3)
    {
        uint port = SU::UIntFromString(text.c_str() + 3);

        if (port > 0)
        {
            SET::GUI::serial_port_num.Set((int)(port - 1));
        }
    }
}
