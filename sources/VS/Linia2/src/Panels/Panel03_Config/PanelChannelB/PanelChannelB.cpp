// 2025/08/14 11:24:57 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel03_Config/PanelChannelB/PanelChannelB.h"
#include "Utils/SystemDepend.h"
#include "MainWindow.h"
#include "Panels/Panel03_Config/Panel03_Config.h"
#include "Device/SettingsDevice.h"
#include "Utils/StringUtils.h"


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

        comboTypeGenerator = new ButtonsCombo(boxGenerator, "Тип", { 10, SD::Y_SB(y - 3) }, width, choices, choices, 1);

        y += 25;

        choices.Clear();
        choices.Add(_L("Вкл"));
        choices.Add(_L("Выкл"));

        new ButtonsCombo(boxGenerator, "Импульс", { 10, SD::Y_SB(y - 3) }, width, choices, choices, 1);

        y += 25;

        choices.Clear();
        choices.Add("-");

        comboStep = new ButtonsComboRange(boxGenerator, "Амплитуда ступени", { 10, SD::Y_SB(y - 3) }, width, choices, choices);

        y += 25;

        new wxCheckBox(boxGenerator, wxID_ANY, "x 0.1", { 10, SD::Y_SB(y) }, { 60, TEXTCNTRL_HEIGHT });

        y += 25;

        new wxStaticText(boxGenerator, wxID_ANY, _L("Число ступенек"), { 10, SD::Y_SB(y + 3) });

        new SpinBox(boxGenerator, { 120, SD::Y_SB(y) }, { 50, TEXTCNTRL_HEIGHT }, 5, 10);

        y += 28;

        choices.Clear();
        choices.Add(_L("прямая"));
        choices.Add(_L("обратная"));

        new ButtonsCombo(boxGenerator, "Полярность", { 10, SD::Y_SB(y) }, width, choices, choices, 1);

        y += 30;

        wxStaticBox *boxOffset = new wxStaticBox(boxGenerator, wxID_ANY, _L("Смещение"), { x, y }, { w - 10, 100 });

        {
            y = 25;

            sliderOffset = new SliderFloatOffset(boxOffset, { 10, SD::Y_SB(y) }, width - 10);

            y += 40;

            choices.Clear();
            choices.Add(_L("прямая"));
            choices.Add(_L("обратная"));

            new ButtonsCombo(boxOffset, "Полярность", { 10, SD::Y_SB(y) }, width - 10, choices, choices, 1);
        }
    }

    wxStaticBox *boxLimitation = new wxStaticBox(this, wxID_ANY, _L("Ограничение"), { x, boxGenerator->GetSize().y + x }, { w, 120 });

    {
        int y = 30;

        wxArrayString choices;
        choices.Add("-");

        comboLimitRange = new ButtonsComboRange(boxLimitation, "Диапазон", { 10, SD::Y_SB(y - 3) }, width, choices, choices);

        y += 40;

        sliderLimit = new SliderFloatLimit(boxLimitation, { 10, SD::Y_SB(y) }, width );
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
