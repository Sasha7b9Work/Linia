// 2025/08/14 11:24:57 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel03_Config/PanelChannelBS/PanelChannelBS.h"
#include "Utils/SystemDepend.h"
#include "MainWindow.h"
#include "Panels/Panel03_Config/Panel03_Config.h"
#include "Device/SettingsDevice.h"
#include "Utils/StringUtils.h"


PanelChannelB *PanelChannelB::self = nullptr;
PanelChannelS *PanelChannelS::self = nullptr;


PanelChannelBS::PanelChannelBS(wxPanel *parent, int x, int w, int h) :
    wxPanel(parent)
{
    SetSize({ MainWindow::WIDTH3, PanelConfig::HEIGHT - 40 });
    SetPosition({ 0, 40 });

    wxStaticBox *boxGenerator = new wxStaticBox(this, wxID_ANY, _L("Генератор ступенек"), { x, 0 }, { w, 300 });

    {
        int y = 22;

        wxArrayString choices;
        choices.Add("U");
        choices.Add("I");

        comboTypeGenerator = new ButtonsCombo(boxGenerator, "Тип", { 10, SD::Y_SB(y) }, PanelConfig::WIDTH_COMBO, choices, choices, 1);

        y += PanelConfig::DYC;

        choices.Clear();
        choices.Add(_L("Вкл"));
        choices.Add(_L("Выкл"));

        new ButtonsCombo(boxGenerator, "Импульс", { 10, SD::Y_SB(y) }, PanelConfig::WIDTH_COMBO, choices, choices, 1);

        y += PanelConfig::DYC;

        choices.Clear();
        choices.Add("-");

        comboStep = new ButtonsComboRange(boxGenerator, "Амплитуда ступени", { 10, SD::Y_SB(y) }, PanelConfig::WIDTH_COMBO, choices, choices);

        y += PanelConfig::DYC;

        btnAmpitudeDecrease = new CheckButton(boxGenerator, "Амплитуда / 10", { 10, SD::Y_SB(y) }, PanelConfig::WIDTH_COMBO);

        y += 28;

        new wxStaticText(boxGenerator, wxID_ANY, _L("Число ступенек"), { 10, SD::Y_SB(y + 3) });

        new SpinBox(boxGenerator, { 120, SD::Y_SB(y) }, { 50, TEXTCNTRL_HEIGHT }, 5, 10);

        y += 28;

        choices.Clear();
        choices.Add(_L("прямая"));
        choices.Add(_L("обратная"));

        new ButtonsCombo(boxGenerator, "Полярность", { 10, SD::Y_SB(y) }, PanelConfig::WIDTH_COMBO, choices, choices, 1);

        y += 30;

        wxStaticBox *boxOffset = new wxStaticBox(boxGenerator, wxID_ANY, _L("Смещение"), { x, y }, { w - 10, 100 });

        {
            y = 25;

            sliderOffset = new SliderFloatOffset(boxOffset, { 10, SD::Y_SB(y) }, PanelConfig::WIDTH_COMBO - 10);

            y += 40;

            choices.Clear();
            choices.Add(_L("прямая"));
            choices.Add(_L("обратная"));

            new ButtonsCombo(boxOffset, "Полярность", { 10, SD::Y_SB(y) }, PanelConfig::WIDTH_COMBO - 10, choices, choices, 1);
        }

        wxPoint pos = boxOffset->GetPosition();
        pos.y = SD::Y_SB(boxGenerator->GetSize().y - boxOffset->GetSize().y - 8);
        boxOffset->SetPosition(pos);
    }

    wxStaticBox *boxLimitation = new wxStaticBox(this, wxID_ANY, _L("Ограничение"),
        { x, boxGenerator->GetPosition().y + boxGenerator->GetSize().y },
        { w, h - boxGenerator->GetPosition().y - boxGenerator->GetSize().y });

    {
        int y = 30;

        wxArrayString choices;
        choices.Add("-");

        comboLimitRange = new ButtonsComboRange(boxLimitation, "Диапазон", { 10, SD::Y_SB(y - 3) }, PanelConfig::WIDTH_COMBO, choices, choices);

        y += 40;

        sliderLimit = new SliderFloatLimit(boxLimitation, { 10, SD::Y_SB(y) }, PanelConfig::WIDTH_COMBO );
    }

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

        wxCommandEvent event(wxEVT_COMBOBOX, comboStep->GetId());
        event.SetEventObject(comboStep);
        event.SetInt(comboStep->GetCurrentSelection());
        wxPostEvent(GetEventHandler(), event);
    }

    {
        comboLimitRange->SetLastSelection();

        wxCommandEvent event(wxEVT_COMBOBOX, comboLimitRange ->GetId());
        event.SetEventObject(comboLimitRange);
        event.SetInt(comboLimitRange->GetCurrentSelection());
        wxPostEvent(GetEventHandler(), event);
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
