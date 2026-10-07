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

    CreateCommonControls();

    CreateSpecificControls();

    HideCommonControls();

    CreateMeasurersSourcers();
}


void PanelViewTest::SetTest(Test *_test)
{
    Test::current = _test;

    if (OStT::current)
    {
        delete OStT::current;

        HideMeasurersSourcers();

        HideSpecificControls();
    }

    OStT::current = CreateOStT();

    ShowCommonControls();

    TuneSpecificControls();

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

        commutator->Refresh();
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

        ampCollector->Show(dc, {X_C, PosMeasurerSourcerY(0)});
        voltCollector->Show(dc, { X_C, PosMeasurerSourcerY(1) });
        srcVoltageCollector->Show(dc, { X_C, PosMeasurerSourcerY(2) });
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

            measBase->Show(dc, { X_B, PosMeasurerSourcerY(1) });
            srcVoltageCurrentBase->Show(dc, { X_B, PosMeasurerSourcerY(2) });
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

            measSubstrate->Show(dc, { X_S, PosMeasurerSourcerY(1) });
            srcVoltateCurrentSubstrate->Show(dc, { X_S, PosMeasurerSourcerY(2) });
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


void PanelViewTest::CreateMeasurersSourcers()
{
    measBase = new MeasurerVoltageCurrent(Chan::_B, Dir::Down);

    srcVoltageCurrentBase = new SourceVoltageCurrent(Chan::_B, Dir::Down);

    measSubstrate = new MeasurerVoltageCurrent(Chan::_S, Dir::Down);

    srcVoltateCurrentSubstrate = new SourceVoltageCurrent(Chan::_S, Dir::Down);

    ampCollector = new Ampermeter(Chan::_C, Dir::Down);

    voltCollector = new Voltmeter(Chan::_C, Dir::Down);

    srcVoltageCollector = new SourceVoltage(Chan::_C, Dir::Down);
}

wxPoint PanelViewTest::GetCenter() const
{
    return { 230, 300 };
}


void PanelViewTest::CreateCommonControls()
{
    commutator = new Commutator(this, { 10, 40 }, 171);

    int width = MeasurerSourcer::WIDTH_CONTROL;

#define CREATE_BUTTONS_COMBO(name, parent, title, num, func, _x, _y)                                        \
    name = new ButtonsCombo(parent, title, width, titles, tooltips, num, #name, ButtonsCombo::Type::Text);  \
    name->Bind(wxEVT_COMBOBOX, &PanelViewTest::func, this);                                                 \
    name->SetPosition( {_x, _y} );

    {
        boxScan = new StaticBox(this, L("Развёртка"), { width + 20, 100 });

        boxScan->SetPosition({ 200, 40 });

        if (!bcScanMode)
        {
            wxArrayString titles;
            titles.push_back("SIN+");
            titles.push_back("SIN-");
            titles.push_back("AC");
            titles.push_back("DC-");
            titles.push_back("DC+");
            titles.push_back("IMP+");
            titles.push_back("IMP-");
            titles.push_back("IMP_CVC");

            wxArrayString tooltips;
            tooltips.push_back("");

            CREATE_BUTTONS_COMBO(bcScanMode, boxScan, L("Тип"), 1, OnChangedScanMode, 10, SD::Y_SB(20));
        }

        if (!bcScanNumberPoints)
        {
            wxArrayString titles =
            {
                "20",
                "50",
                "100",
                "200"
            };

            wxArrayString tooltips =
            {
                ""
            };

            CREATE_BUTTONS_COMBO(bcScanNumberPoints, boxScan, L("Кол-во точек"), 1, OnChangedScanNumberPoints, 10, SD::Y_SB(20 + 30));
        }
    }

    {
        boxCover = new StaticBox(this, L("Крышка"), { width + 20, 100 });

        boxCover->SetPosition({ 400, 40 });

        txtCover = new StaticText(boxCover, L(""), { boxCover->GetSize().x - 20, boxCover->GetSize().y - 30 }, wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER_HORIZONTAL);

        txtCover->SetPosition({ 10, SD::Y_SB(20) });

        wxFont font = txtCover->GetFont();
        font.SetPointSize(23);
        txtCover->SetFont(font);

        OpenCover();
    }

    {
        CreateButton(&btnEditSave, this, L("Сохранить"), { 600, 40 }, bcScanNumberPoints->GetSize(), [this](wxCommandEvent &)
            {
                if (InModeEdit())
                {
                    btnEditSave->Hide();
                    btnEditExit->Hide();
                }
                else
                {

                }
            });

        btnEditSave->Hide();

        CreateButton(&btnEditExit, this, L("Выход"), { btnEditSave->GetPosition().x, 80 }, btnEditSave->GetSize(), [this](wxCommandEvent &)
            {
                if (InModeEdit())
                {
                    btnEditSave->Hide();
                    btnEditExit->Hide();
                }
                else
                {

                }
            });

        btnEditExit->Hide();
    }

#undef CREATE_BUTTONS_COMBO
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


void PanelViewTest::HideSpecificControls()
{
    bcTypeBJT->Hide();
    bcTypeFET->Hide();
    bcCommonElectrodeDIOD->Hide();
    bcControlElectrodeTHYR->Hide();
}


void PanelViewTest::HideCommonControls()
{
    commutator->Hide();
    boxScan->Hide();
    boxCover->Hide();
}


void PanelViewTest::ShowCommonControls()
{
    commutator->Show();
    boxScan->Show();
    boxCover->Show();
}


void PanelViewTest::CreateButton(Button **btn, wxWindow *parent,
    const wxString &label, const wxPoint &pos,
    const wxSize &size, std::function<void(wxCommandEvent &)> onClick)
{
    *btn = new Button(parent, label, size);
    (*btn)->SetPosition(pos);
    (*btn)->Bind(wxEVT_BUTTON, [onClick](wxCommandEvent &event)
        {
            onClick(event);
        });
}


void PanelViewTest::OnChangedScanMode(wxCommandEvent &)
{

}


void PanelViewTest::OnChangedScanNumberPoints(wxCommandEvent &)
{

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


void PanelViewTest::OpenCover()
{
    cover_is_opened = true;

    txtCover->SetLabel(L("Открыта"));

    txtCover->SetBackgroundColour(*wxRED);

    txtCover->Refresh();
}


void PanelViewTest::CloseCover()
{
    cover_is_opened = false;

    txtCover->SetLabel(L("Закрыта"));

    txtCover->SetBackgroundColour(GetBackgroundColour());

    txtCover->Refresh();
}


bool PanelViewTest::InModeEdit() const
{
    return !btnEditSave->IsShown();
}


void PanelViewTest::DrawGround(AutoBufferedPaintDC &dc)
{
    const wxPoint p{ dc.GetCoord() };

    dc.DrawLine(p.x - 10, p.y, p.x + 10, p.y);
}


void PanelViewTest::HideMeasurersSourcers()
{
    measBase->Hide();
    srcVoltageCurrentBase->Hide();

    measSubstrate->Hide();
    srcVoltateCurrentSubstrate->Hide();

    ampCollector->Hide();
    voltCollector->Hide();
    srcVoltageCollector->Hide();
}


int PanelViewTest::PosMeasurerSourcerY(int num)
{
    if (num == 0)
    {
        return 250;
    }
    else if (num == 1)
    {
        return 380;
    }
    else if (num == 2)
    {
        return 510;
    }

    return 100;
}
