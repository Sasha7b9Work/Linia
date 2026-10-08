// 2026/04/29 16:03:58 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "GUI/PageTests/PanelViewTest.h"
#include "Utils/GlobalFunctions.h"
#include "GUI/Controls/Painter.h"
#include "GUI/Controls/StaticBox.h"
#include "Utils/SystemDepend.h"
#include "GUI/PageTests/Entities/OStT/BJT.h"
#include "GUI/PageTests/Entities/OStT/BJTS.h"
#include "GUI/PageTests/Entities/OStT/FET.h"
#include "GUI/PageTests/Entities/OStT/FETS.h"
#include "GUI/PageTests/Entities/OStT/DARL.h"
#include "GUI/PageTests/Entities/OStT/THYR.h"
#include "GUI/PageTests/Entities/OStT/DIOD.h"
#include "GUI/PageTests/Entities/OStT/RES.h"
#include "GUI/PageTests/Entities/OStT/CAP.h"


PanelViewTest *PanelViewTest::self = nullptr;


PanelViewTest::PanelViewTest(wxWindow *parent) : Panel(parent, wxSIMPLE_BORDER)
{
    self = this;

    Bind(wxEVT_PAINT, &PanelViewTest::OnEventPaint, this);

    SetBackgroundStyle(wxBG_STYLE_PAINT);

    com_controls.Create(this);

    CreateSpecificControls();

    com_controls.Hide();

    meas_src.Create();
}


void PanelViewTest::SetTest(Test *_test)
{
    Test::current = _test;

    if (OStT::current)
    {
        delete OStT::current;

        meas_src.HideAll();

        HideSpecificControls();
    }

    OStT::current = CreateOStT();

    com_controls.Show();

    TuneSpecificControls();

    meas_src.Tune();

    Refresh();
}


void PanelViewTest::OnEventPaint(wxPaintEvent &event)
{
    if (Test::current)
    {
        AutoBufferedPaintDC dc{ this };

        dc.SetBackground(wxBrush(GetBackgroundColour()));
        dc.Clear();

        dc.SetBrush(wxBrush(GetBackgroundColour()));

        dc.SetPen(wxPen(*wxBLACK, 1));

        DrawScheme(dc);

        // Устанавливаем цвет текста
        dc.SetTextForeground(*wxBLACK);

        // Устанавливаем шрифт (опционально)
        dc.SetFont(wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));

        // Рисуем текст в левом верхнем углу
        dc.DrawText(Test::current->lib->name + " : " + Test::current->name, 5, 5);

        com_controls.Refresh();
    }

    event.Skip();
}


wxPoint PanelViewTest::CoordinateCollector() const
{
    return { 570, 170 };
}


void PanelViewTest::DrawScheme(AutoBufferedPaintDC &dc)
{
    OStT::current->Draw(dc, GetCenter());

    const int DX = 300;
    const int X_B = 100;
    const int X_S = X_B + DX;
    const int X_C = CoordinateCollector().x;
    const int Y_C = CoordinateCollector().y;

    {
        // Рисуем от эмиттера

        wxPoint point = OStT::current->GetPoint_E();

        dc.MoveTo(point);

        dc.LineOnDY(50);

        DrawGround(dc);
    }

    {
        // Рисуем от коллектора

        wxPoint point = OStT::current->GetPoint_C();

        dc.MoveTo(point);

        dc.LineToY(Y_C);

        dc.LineToX(X_C);

        dc.LineToY(Y_GROUND);

        DrawGround(dc);

        meas_src.Draw(dc, Chan::C, X_C);
    }

    {
        // Рисуем от базы

        wxPoint point;

        if (OStT::current->GetPoint_B(point))
        {
            dc.MoveTo(point);

            dc.LineToX(X_B);

            dc.LineToY(Y_GROUND);

            DrawGround(dc);

            meas_src.Draw(dc, Chan::B, X_B);
        }
    }

    {
        // Рисуем от подложки

        wxPoint point;

        if (OStT::current->GetPoint_S(point))
        {
            dc.MoveTo(point);

            dc.LineToX(X_S);

            dc.LineToY(Y_GROUND);

            DrawGround(dc);

            meas_src.Draw(dc, Chan::S, X_S);
        }
    }

    OStT::current->FuncAfterDraw(dc);
}


OStT *PanelViewTest::CreateOStT()
{
    if (Test::current->IsBJT())
    {
        return new BJT();
    }
    else if (Test::current->IsBJTS())
    {
        return new BJTS();
    }
    else if (Test::current->IsFET())
    {
        return new FET();
    }
    else if (Test::current->IsFETS())
    {
        return new FETS();
    }
    else if (Test::current->IsDARL())
    {
        return new DARL();
    }
    else if (Test::current->IsTHYR())
    {
        return new THYR();
    }
    else if (Test::current->IsDIOD())
    {
        return new DIOD();
    }
    else if (Test::current->IsRES())
    {
        return new RES();
    }
    else if (Test::current->IsCAP())
    {
        return new CAP();
    }

    LOG_ERROR("Incorrect type OStT");

    return nullptr;
}


wxPoint PanelViewTest::GetCenter() const
{
    return { 230, 300 };
}


wxPoint PanelViewTest::CoordinateSpecificControl() const
{
    return { CoordinateCollector().x - 280, CoordinateCollector().y + 20 };
}


void PanelViewTest::CreateSpecificControls()
{
    {
        wxArrayString labels
        {
            "npn",
            "pnp"
        };

        wxArrayString tooltips{ L("Проводимость транзистора") };

        bcTypeBJT = new ButtonsCombo(this, L("Тип"), MeasurerSourcer::WIDTH_CONTROL, labels, tooltips, 1, L("Проводимость транзистора"));
        bcTypeBJT->SetPosition(CoordinateSpecificControl());
        bcTypeBJT->Bind(wxEVT_COMBOBOX, [this](wxCommandEvent &event)
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

        bcTypeFET = new ButtonsCombo(this, L("Канал"), MeasurerSourcer::WIDTH_CONTROL, labels, tooltips, 1, L("Проводимость канала"));
        bcTypeFET->SetPosition(CoordinateSpecificControl());
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

        bcCommonElectrodeDIOD = new ButtonsCombo(this, L("Общий электрод"), MeasurerSourcer::WIDTH_CONTROL, labels, tooltips, 1, L("Общий электрод"));
        bcCommonElectrodeDIOD->SetPosition(CoordinateSpecificControl());
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

        bcControlElectrodeTHYR = new ButtonsCombo(this, L("Управление"), MeasurerSourcer::WIDTH_CONTROL, labels, tooltips, 1, L("Управляющий электрод"));
        wxPoint coord = CoordinateSpecificControl();
        bcControlElectrodeTHYR->SetPosition({ coord.x, coord.y + 30 });
        bcControlElectrodeTHYR->Bind(wxEVT_COMBOBOX, [this](wxCommandEvent &event)
            {
                OStT::current->ToTHYR()->SetControlElectrode((ControlElectrode::E)event.GetInt());
            });
    }

    HideSpecificControls();
}


void PanelViewTest::HideSpecificControls()
{
    bcTypeBJT->Hide();
    bcTypeFET->Hide();
    bcCommonElectrodeDIOD->Hide();
    bcControlElectrodeTHYR->Hide();
}


void PanelViewTest::TuneSpecificControls()
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


void PanelViewTest::OnChangedTypeSemiconductor(wxCommandEvent &)
{

}


void PanelViewTest::OnChangedBaseModeControl(wxCommandEvent &)
{

}


void PanelViewTest::OnChangedBaseStartValueI(wxCommandEvent &)
{

}


void PanelViewTest::OnChangedBaseDeltaValueI(wxCommandEvent &)
{

}


void PanelViewTest::OnChangedBaseNumMeasures(wxCommandEvent &)
{

}


void PanelViewTest::OnChangedBaseMeasureRangeU(wxCommandEvent &)
{

}


void PanelViewTest::OnChangedBaseMeasureLimitU(wxCommandEvent &)
{

}


void PanelViewTest::OnChangedCollectorModeSource(wxCommandEvent &)
{

}


void PanelViewTest::OnChangedCollectorValueStart(wxCommandEvent &)
{

}


void PanelViewTest::OnChangedCollectorValueFinish(wxCommandEvent &)
{

}


void PanelViewTest::OnChangedCollectorMeasureRangeI(wxCommandEvent &)
{

}


void PanelViewTest::OnChangedCollectorMeasureLimitI(wxCommandEvent &)
{

}


void PanelViewTest::OnChangedCollectorMeasureRangeU(wxCommandEvent &)
{

}


void PanelViewTest::OnChangedCollectorMeasureLimitU(wxCommandEvent &)
{

}


int PanelViewTest::CalculateCombos(ComboInput **c1, ComboInput **c2, ComboInput **c3, ComboInput **c4)
{
    if (c4)
    {
        return 4;
    }
    else if (c3)
    {
        return 3;
    }
    else if (c2)
    {
        return 2;
    }
    else if (c1)
    {
        return 1;
    }

    return 0;
}


void PanelViewTest::DrawGround(AutoBufferedPaintDC &dc)
{
    const wxPoint p{ dc.GetCoord() };

    dc.DrawLine(p.x - 10, p.y, p.x + 10, p.y);
}
