// 2025/08/14 11:24:57 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel03_Config/PanelChannelB/PanelChannelB.h"
#include "Utils/SystemDepend.h"
#include "MainWindow.h"
#include "Panels/Panel03_Config/Panel03_Config.h"
#include "Device/SettingsDevice.h"


PanelChannelB *PanelChannelB::self = nullptr;


PanelChannelB::PanelChannelB(wxPanel *parent, int x, int w) :
    wxPanel(parent)
{
    self = this;

    SetSize({ MainWindow::WIDTH3, PanelConfig::HEIGHT - 40 });
    SetPosition({ 0, 40 });

    wxStaticBox *boxGenerator = new wxStaticBox(this, wxID_ANY, _L("Генератор ступенек"), { x, 0 }, { w, 300 });

    int width = 180;

    {
        int y = 25;

        wxArrayString choices;
        choices.Add("U");
        choices.Add("I");

        comboTypeGenerator = new ButtonsCombo(boxGenerator, "Тип", { 10, SD::Y_SB(y - 3) }, width, choices, choices, 1, false);

        y += 25;

        choices.Clear();
        choices.Add(_L("Вкл"));
        choices.Add(_L("Выкл"));

        new ButtonsCombo(boxGenerator, "Импульс", { 10, SD::Y_SB(y - 3) }, width, choices, choices, 1, false);

        y += 25;

        choices.Clear();
        choices.Add("-");

        comboStep = new ButtonsCombo(boxGenerator, "Амплитуда ступени", { 10, SD::Y_SB(y - 3) }, width, choices, choices, 3, true);

        y += 25;

        new wxCheckBox(boxGenerator, wxID_ANY, "x 0.1", { 10, SD::Y_SB(y) }, { 60, TEXTCNTRL_HEIGHT });

        y += 25;

        new wxStaticText(boxGenerator, wxID_ANY, _L("Число ступенек"), { 10, SD::Y_SB(y + 3) });

        new SpinBox(boxGenerator, { 120, SD::Y_SB(y) }, { 50, TEXTCNTRL_HEIGHT }, 5, 10);

        y += 25;

        choices.Clear();
        choices.Add(_L("прямая"));
        choices.Add(_L("обратная"));

        new ButtonsCombo(boxGenerator, "Полярность", { 10, SD::Y_SB(y) }, width, choices, choices, 1, false);

        y += 25;

        wxStaticBox *boxOffset = new wxStaticBox(boxGenerator, wxID_ANY, _L("Смещение"), { x, y }, { w - 10, 100 });

        {
            y = 25;

            sliderOffset = new SliderFloat(boxOffset, { 10, SD::Y_SB(y) }, width - 10, 0.0f, 0.0f);

            y += 40;

            choices.Clear();
            choices.Add(_L("прямая"));
            choices.Add(_L("обратная"));

            new ButtonsCombo(boxOffset, "Полярность", { 10, SD::Y_SB(y) }, width - 10, choices, choices, 1, false);
        }
    }

    wxStaticBox *boxLimitation = new wxStaticBox(this, wxID_ANY, _L("Ограничение"), { x, boxGenerator->GetSize().y + x }, { w, 100 });

    {
        int y = 30;

        wxArrayString choices;
        choices.Add("-");

        comboLimitRange = new ButtonsCombo(boxLimitation, "Диапазон", { 10, SD::Y_SB(y - 3) }, width, choices, choices, 3, true);

        y += 40;

        new SliderInt(boxLimitation, { 10, SD::Y_SB(y) }, width, 0, 100);
    }

    Bind(wxEVT_COMBOBOX, &PanelChannelB::OnEventComboBox, this);

    Tune();
}


void PanelChannelB::Tune()
{
    comboTypeGenerator->SetCurrentSelection(1);

    wxArrayString ranges;
    RangeI::FillArrayStrings(ranges, DSet::Type::ChanB_Limit, false);
    comboLimitRange->SetChoices(ranges, ranges);
}


void PanelChannelB::OnEventComboBox(wxCommandEvent &event)
{
    ButtonsCombo *combo = (ButtonsCombo *)event.GetEventObject();

    if (combo == comboTypeGenerator)
    {
        wxArrayString ranges;

        if (combo->GetCurrentSelection() == 0)              // Напряжение
        {
            RangeU::FillArrayStrings(ranges, DSet::Type::ChanB_Source, true);

            wxArrayString tooltips;
            RangeU::FillArrayStrings(tooltips, DSet::Type::ChanB_Source, false);

            for (auto &elem : tooltips)
            {
                elem = wxString("Диапазон : " + elem);
            }

            comboStep->SetChoices(ranges, tooltips);
        }
        else if (combo->GetCurrentSelection() == 1)         // Ток
        {
            RangeI::FillArrayStrings(ranges, DSet::Type::ChanB_Source, true);

            wxArrayString tooltips;
            RangeI::FillArrayStrings(tooltips, DSet::Type::ChanB_Source, false);

            for (auto &elem : tooltips)
            {
                elem = wxString("Диапазон : ") + elem;
            }

            comboStep->SetChoices(ranges, tooltips);
        }
    }
    else if (combo == comboStep)
    {
        if (comboTypeGenerator->GetCurrentSelection() == 0)     // Напряжение
        {

        }
    }

    event.Skip();
}
