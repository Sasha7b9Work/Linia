// 2025/10/07 12:29:58 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "GUI/PageDebug/Notebook/PageSource50V.h"
#include "GUI/PageDebug/Notebook/Register.h"
#include "IPPP/IDevice.h"


/*
    Регистр управления      8 разрядов
*/


PageSource50V *PageSource50V::self = nullptr;


PageSource50V::PageSource50V(wxNotebook *notebook) :
    PageChip(notebook, L("Источник 50 В"))
{
    self = this;

    RegAD5443 *regDAC7 = new RegAD5443(this, dacs[DAC::_6_Source_50V], L("Управление с Orange Pi"));

    AppendRegister(regDAC7);

    wxArrayString labels =
    {
        L("Откл"),
        L("Вкл")
    };

    wxArrayString tooltips = { "" };

    const int w = 100;

    {
        combo50Plus = new ButtonsCombo(this, L("50E+"), w, labels, tooltips, 1, L("50E+"));

        combo50Plus->SetPosition({ 10, 200 });

        combo50Plus->Bind(wxEVT_COMBOBOX, [this](wxCommandEvent &event)
            {
                IDevice::impl->SendCommand(wxString::Format(":SOURCE50V:PLUS %d", event.GetInt()));
            });
    }

    {
        combo50Minus = new ButtonsCombo(this, L("50E-"), w, labels, tooltips, 1, L("50E-"));

        combo50Minus->SetPosition({ 10, 230 });

        combo50Minus->Bind(wxEVT_COMBOBOX, [this](wxCommandEvent &event)
            {
                IDevice::impl->SendCommand(wxString::Format(":SOURCE50V:MINUS %d", event.GetId()));
            });
    }
}
