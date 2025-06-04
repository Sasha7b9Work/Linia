// 2025/6/3 22:44:48 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Debug/PageAD9952.h"


PageAD9952 *PageAD9952::self = nullptr;


PageAD9952::PageAD9952(wxNotebook *parent) : PageChip(parent, "AD9952")
{
    self = this;

    wxArrayString names;

    {
        names.push_back("X");
        names.push_back("SYNC_CLK Out Disable");
        names.push_back("X");
        names.push_back("External Power-Down Mode");
        names.push_back("Clock Input Power-Down");
        names.push_back("DAC Power-Down");
        names.push_back("Comparator Power-Down");
        names.push_back("Digital Power-Down");

        names.push_back("LSB First");
        names.push_back("SDIO Input Only");
        names.push_back("Clear Phase Accum.");
        names.push_back("X");
        names.push_back("Enable SINE Output");
        names.push_back("AutoClr Phase Accum.");
        names.push_back("X");
        names.push_back("X");

        for (int i = 0; i < 6; i++)
        {
            names.push_back("X");
        }
        names.push_back("Software Manual Sync");
        names.push_back("Automatic Sync Enable");

        names.push_back("Auto OSK Keying");
        names.push_back("OSK Enable");
        names.push_back("Load ARR I/O UD");
        for (int i = 0; i < 5; i++)
        {
            names.push_back("X");
        }

        PanelRegister *regCFR1 = new PanelRegister(this, "Control Function Register CFR1", 32, false);

        regCFR1->SetNamesBits(names);

        AppendRegister(regCFR1);
    }

    {
        names.clear();

//        names.push_back("Charge")
    }
}
