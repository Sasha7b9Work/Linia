// 2025/6/3 22:44:48 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel12_Debug/PageAD9952.h"


PageAD9952 *PageAD9952::self = nullptr;


PageAD9952::PageAD9952(wxNotebook *parent) : PageChip(parent, "AD9952")
{
    self = this;

    wxArrayString names;

    {
        for (int i = 0; i < 32; i++)
        {
            names.push_back(wxString::Format("D%d", i));
        }

        PanelRegister *regCFR1 = new PanelRegister(this, "Control Function Register CFR1", 32, false, false);

        regCFR1->SetNamesBits(names);

        std::vector<StructDescription> desc0;

        desc0.push_back({ 0, 1, "Not Used" });
        desc0.push_back({ 1, 1, "SYNC_CLK Out Disable" });
        desc0.push_back({ 2, 1, "X" });
        desc0.push_back({ 3, 1, "External Power-Down Mode" });
        desc0.push_back({ 4, 1, "Clock Input Power-Down" });
        desc0.push_back({ 5, 1, "DAC Power-Down" });
        desc0.push_back({ 6, 1, "Comparator Power-Down" });
        desc0.push_back({ 7, 1, "Digital Power-Down" });

        desc0.push_back({ 8, 1, "LSB First" });
        desc0.push_back({ 9, 1, "SDIO Input Only" });
        desc0.push_back({ 10, 1, "Clear Phase Accum." });
        desc0.push_back({ 11, 1, "X" });
        desc0.push_back({ 12, 1, "Enable SINE Output" });
        desc0.push_back({ 13, 1, "AutoClr Phase Accum." });
        desc0.push_back({ 14, 1, "X" });
        desc0.push_back({ 15, 1, "X" });

        desc0.push_back({ 16, 6, "Not Used" });
        desc0.push_back({ 22, 1, "Software Manual Sync" });
        desc0.push_back({ 23, 1, "Automatic Sync Enable" });

        desc0.push_back({ 24, 1, "Auto OSK Keying" });
        desc0.push_back({ 25, 1, "OSK Enable" });
        desc0.push_back({ 26, 1, "Load ARR I/O UD" });
        desc0.push_back({ 27, 5, "Not Used" });

        regCFR1->SetDescriptionBits(0, desc0);

        AppendRegister(regCFR1);
    }

    {
        names.clear();

        for (int i = 0; i < 24; i++)
        {
            names.push_back(wxString::Format("B%d", i));
        }

        PanelRegister *regCFR2 = new PanelRegister(this, "Control Function Register CFR2", 24, false, false);

        regCFR2->SetNamesBits(names);

        std::vector<StructDescription> desc0;
        desc0.push_back({ 0, 2, "Charge Pump Current" });
        desc0.push_back({ 2, 1, "VCO Rang" });
        desc0.push_back({ 3, 5, "REFCLK Multiplier" });
        desc0.push_back({ 8, 1, "X" });
        desc0.push_back({ 9, 1, "CRYSTAL OUT Pin Active" });
        desc0.push_back({ 10, 1, "Hardware Manual Sync Enable" });
        desc0.push_back({ 11, 1, "High Speed Sync Enable" });
        desc0.push_back({ 12, 12, "Not Used" });

        regCFR2->SetDescriptionBits(0, desc0);

        AppendRegister(regCFR2);
    }

    {
        names.clear();

        for (int i = 0; i < 16; i++)
        {
            names.push_back(wxString::Format("D%d", i));
        }

        PanelRegister *regASF = new PanelRegister(this, "Amplitude Scale Factor ASF", 16, false, false);

        regASF->SetNamesBits(names);

        AppendRegister(regASF);
    }
}
