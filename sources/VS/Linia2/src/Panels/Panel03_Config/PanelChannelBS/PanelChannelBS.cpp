// 2025/08/14 11:24:57 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel03_Config/PanelChannelBS/PanelChannelBS.h"
#include "Utils/SystemDepend.h"
#include "MainWindow.h"
#include "Panels/Panel03_Config/Panel03_Config.h"
#include "Device/SettingsDevice.h"
#include "Utils/StringUtils.h"
#include "Utils/GlobalFunctions.h"
#include "Tests/Tests.h"
#include "Controls/StaticBox.h"


PanelChannelB *PanelChannelB::self = nullptr;
PanelChannelS *PanelChannelS::self = nullptr;


PanelChannelBS::PanelChannelBS(wxPanel *parent, int x, int w, int h) :
    wxPanel(parent)
{
    SetSize({ MainWindow::WIDTH3, PanelConfig::HEIGHT - 40 });
    SetPosition({ 0, 40 });

    StaticBox *boxGenerator = new StaticBox(this, _L("Генератор ступенек"), { x, SD::DSBY() }, { w, 300 });

    {
        int y = 22;

        wxArrayString choices;
        choices.Add("U");
        choices.Add("I");

        comboTypeGenerator = new ButtonsCombo(boxGenerator, "Тип", SD::XY0(), PanelConfig::WIDTH_COMBO, choices, choices, 1);

        y = SD::XY0().y;

        y += PanelConfig::DYC;

        choices.Clear();
        choices.Add(_L("Вкл"));
        choices.Add(_L("Выкл"));

        new ButtonsCombo(boxGenerator, "Импульс", { SD::XY0().x, y }, PanelConfig::WIDTH_COMBO, choices, choices, 1);

        y += PanelConfig::DYC;

        choices.Clear();
        choices.Add("-");

        comboStep = new ButtonsComboRange(boxGenerator, "Амплитуда ступени", { SD::XY0().x, y }, PanelConfig::WIDTH_COMBO, choices, choices);

        y += PanelConfig::DYC;

        choices = { "x 1", "x 0.1" };

        comboAmpitudeDecrease = new ButtonsCombo(boxGenerator, "Амплитуда", { SD::XY0().x, y }, PanelConfig::WIDTH_COMBO, choices, choices, 1);

        y += PanelConfig::DYC;

        choices.Clear();

        for (int i = 0; i <= 10; i++)
        {
            choices.push_back(wxString::Format("%d", i));
        }

        new ButtonsCombo(boxGenerator, "Число ступенек", { SD::XY0().x, y }, PanelConfig::WIDTH_COMBO, choices, choices, 3);

        y += PanelConfig::DYC;

        choices.Clear();
        choices.Add(_L("прямая"));
        choices.Add(_L("обратная"));

        new ButtonsCombo(boxGenerator, "Полярность", { SD::XY0().x, y }, PanelConfig::WIDTH_COMBO, choices, choices, 1);

        y += 30;

        StaticBox *boxOffset = new StaticBox(boxGenerator, _L("Смещение"), { x, y }, { w - 10, 100 });

        {
            y = 25;

            sliderOffset = new SliderFloatOffset(boxOffset, { SD::XY0().x, SD::Y_SB(y) }, PanelConfig::WIDTH_COMBO - 10);

            y += 40;

            choices.Clear();
            choices.Add(_L("прямая"));
            choices.Add(_L("обратная"));

            new ButtonsCombo(boxOffset, "Полярность", { PanelConfig::X, SD::Y_SB(y) }, PanelConfig::WIDTH_COMBO - 10, choices, choices, 1);
        }

        boxOffset->SetFont(StaticBox::TitleFont());

        wxPoint pos = boxOffset->GetPosition();
        pos.y = SD::Y_SB(boxGenerator->GetSize().y - boxOffset->GetSize().y - 8);
        boxOffset->SetPosition(pos);
    }

    boxGenerator->SetFont(StaticBox::TitleFont());

    StaticBox *boxLimitation = new StaticBox(this, _L("Ограничение"),
        { x, boxGenerator->GetPosition().y + boxGenerator->GetSize().y + SD::DSBY() },
        { w, h - boxGenerator->GetPosition().y - boxGenerator->GetSize().y - SD::DSBY() });

    {
        int y = 30;

        wxArrayString choices;
        choices.Add("-");

        comboLimitRange = new ButtonsComboRange(boxLimitation, "Диапазон", SD::XY0(), PanelConfig::WIDTH_COMBO, choices, choices);

        y += 40;

        sliderLimit = new SliderFloatLimit(boxLimitation, { PanelConfig::X, SD::Y_SB(y) }, PanelConfig::WIDTH_COMBO );
    }

    boxLimitation->SetFont(StaticBox::TitleFont());

    Bind(wxEVT_COMBOBOX, &PanelChannelB::OnEventComboBox, this);

    Tune();
}


void PanelChannelBS::Tune()
{
    comboTypeGenerator->SetCurrentSelection(1);

    wxArrayString ranges;
    RangeI::FillArrayStrings(ranges, DSet::Type::ChanB_Limit, false);
    comboLimitRange->SetChoices(ranges, ranges);

    {
        comboStep->SetLastSelection();

        GF::SendCommandEvent(comboStep, wxEVT_COMBOBOX, comboStep->GetCurrentSelection());
    }

    {
        comboLimitRange->SetLastSelection();

        GF::SendCommandEvent(comboLimitRange, wxEVT_COMBOBOX, comboLimitRange->GetCurrentSelection());
    }
}


void PanelChannelBS::OnEventComboBox(wxCommandEvent &event)
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
    else if (combo == comboLimitRange)
    {
        sliderLimit->CalculateAndSetRange(comboLimitRange->GetCurrentString());
    }

    if (comboTypeGenerator->GetCurrentSelection() == 0)             // Напряжение
    {
        sliderOffset->CalculateAndSetRange(comboStep->GetCurrentString(), 10.0);
    }
    else if (comboTypeGenerator->GetCurrentSelection() == 1)        // Ток
    {
        sliderOffset->CalculateAndSetRange(comboStep->GetCurrentString(), 10.0);
    }

    event.Skip();
}


bool PanelChannelB::IsEnabled() const
{
    return Channel(Channel::_B).IsVisible();
}


bool PanelChannelS::IsEnabled() const
{
    return Channel(Channel::_S).IsVisible();
}
