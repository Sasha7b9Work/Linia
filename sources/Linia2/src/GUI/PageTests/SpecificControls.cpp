// 2026/10/08 13:09:54 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "GUI/PageTests/SpecificControls.h"
#include "GUI/PageTests/Entities/MeasurerSourcer.h"
#include "GUI/PageTests/Entities/OStT/OStT.h"
#include "GUI/PageTests/Entities/OStT/BJT.h"
#include "GUI/PageTests/Entities/OStT/FET.h"
#include "GUI/PageTests/Entities/OStT/DIOD.h"
#include "GUI/PageTests/Entities/OStT/THYR.h"


void SpecificControls::Create(wxWindow *parent)
{
    {
        wxArrayString labels
        {
            "npn",
            "pnp"
        };

        wxArrayString tooltips{ L("Проводимость транзистора") };

        bcTypeBJT = new ButtonsCombo(parent, L("Тип"), MeasurerSourcer::WIDTH_CONTROL, labels, tooltips, 1, L("Проводимость транзистора"));
        bcTypeBJT->SetPosition(Coordinate());
        bcTypeBJT->Bind(wxEVT_COMBOBOX, [](wxCommandEvent &event)
            {
                OStT::current->ToBJT()->SetType((TypeBJT::E)event.GetInt());
            });
        bcTypeBJT->Hide();
    }

    {
        wxArrayString labels
        {
            "p",
            "n"
        };

        wxArrayString tooltips{ L("Проводимость канала") };

        bcTypeFET = new ButtonsCombo(parent, L("Канал"), MeasurerSourcer::WIDTH_CONTROL, labels, tooltips, 1, L("Проводимость канала"));
        bcTypeFET->SetPosition(Coordinate());
        bcTypeFET->Bind(wxEVT_COMBOBOX, [this](wxCommandEvent &event)
            {
                OStT::current->ToFET()->SetType((TypeFET::E)event.GetInt());
            });
    }

    {
        wxArrayString labels
        {
            L("Анод"),
            L("Катод")
        };

        wxArrayString tooltips{ L("Общий электрод") };

        bcCommonElectrodeDIOD = new ButtonsCombo(parent, L("Общий электрод"), MeasurerSourcer::WIDTH_CONTROL, labels, tooltips, 1, L("Общий электрод"));
        bcCommonElectrodeDIOD->SetPosition(Coordinate());
        bcCommonElectrodeDIOD->Bind(wxEVT_COMBOBOX, [this](wxCommandEvent &event)
            {
                OStT::current->ToDIOD()->SetCommonElectrode((CommonElectrode::E)event.GetInt());
            });
    }

    {
        wxArrayString labels
        {
            L("Анод"),
            L("Катод")
        };

        wxArrayString tooltips{ L("Управляющий электрод") };

        bcControlElectrodeTHYR = new ButtonsCombo(parent, L("Управление"), MeasurerSourcer::WIDTH_CONTROL, labels, tooltips, 1, L("Управляющий электрод"));
        wxPoint coord = Coordinate();
        bcControlElectrodeTHYR->SetPosition({ coord.x, coord.y + 30 });
        bcControlElectrodeTHYR->Bind(wxEVT_COMBOBOX, [this](wxCommandEvent &event)
            {
                OStT::current->ToTHYR()->SetControlElectrode((ControlElectrode::E)event.GetInt());
            });
    }

    Hide();
}


void SpecificControls::Hide()
{
    bcTypeBJT->Hide();
    bcTypeFET->Hide();
    bcCommonElectrodeDIOD->Hide();
    bcControlElectrodeTHYR->Hide();
}


void SpecificControls::Tune()
{
    if (Test::current->IsBJT() ||
        Test::current->IsBJTS() ||
        Test::current->IsDARL())
    {
        bcTypeBJT->Show();
    }
    else if (Test::current->IsFET() ||
        Test::current->IsFETS())
    {
        bcTypeFET->Show();
    }
    else if (Test::current->IsDIOD() ||
        Test::current->IsTHYR())
    {
        bcCommonElectrodeDIOD->Show();
    }

    if (Test::current->IsTHYR())
    {
        bcControlElectrodeTHYR->Show();
    }
}


wxPoint SpecificControls::Coordinate() const
{
    return { PanelViewTest::self->CoordinateCollector().x - 280, PanelViewTest::self->CoordinateCollector().y + 20 };
}
