// 2026/04/29 16:03:58 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "GUI/PageTests/PanelViewTest.h"
#include "Utils/GlobalFunctions.h"
#include "Utils/LineDrawer.h"
#include "GUI/Controls/Painter.h"


PanelViewTest *ThePanelViewTest = nullptr;


PanelViewTest::PanelViewTest(wxWindow *parent, PanelViewTest *&global) : Panel(parent, wxSIMPLE_BORDER)
{
    global = this;

    Bind(wxEVT_PAINT, &PanelViewTest::OnEventPaint, this);
}


void PanelViewTest::SetTest(Test *_test)
{
    test = _test;

    Refresh();
}


void PanelViewTest::OnEventPaint(wxPaintEvent &event)
{
    if (test)
    {
        dc = new wxPaintDC(this);

        dc->SetBrush(wxBrush(GetBackgroundColour()));

        dc->SetPen(wxPen(*wxBLACK, 1));

        // Устанавливаем цвет текста
        dc->SetTextForeground(*wxBLACK);

        CreateControls();

        CreateElement();

        // Устанавливаем шрифт (опционально)
        dc->SetFont(wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));

        // Рисуем текст в левом верхнем углу
        dc->DrawText(test->lib->name + " : " + test->name, 5, 5);

        delete dc;
        dc = nullptr;
    }

    event.Skip();
}


void PanelViewTest::CreateElement()
{
    if (test->lib->UGO == "BJT" ||
        test->lib->UGO == "BJTS")
    {
        wxPoint point_base;
        wxPoint point_collector;
        wxPoint point_substrate;
        wxPoint point_emitter;

        CreateBJT("npn", GetCenter(), point_base, point_collector, point_substrate, point_emitter);
    }
}


void PanelViewTest::CreateBJT(const wxString &type, const wxPoint &c, wxPoint &point_base, wxPoint &point_collector, wxPoint &point_substrate, wxPoint &point_emitter)
{
    int r = 5;

    int x_col = c.x + radius_trans / 2;   // / Координаты точки коммутации
    int y_col = c.y - 2 * radius_trans;   // / с коллектором

    LineDriwer driwer(*dc, x_col, y_col);
    driwer.LineTo(c.x + radius_trans / 2, c.y + 2 * radius_trans);          // Вертикальная линия, которая выходит из коллектора и эмиттера
    DrawGround(driwer.GetX(), driwer.GetY());
    driwer.MoveOnDY(-20);
    point_emitter = driwer.GetCoord();
    dc->DrawCircle(point_emitter, r);
    dc->DrawText("E", { point_emitter.x + 7, point_emitter.y - 7 });
    dc->DrawCircle(c, radius_trans);
    const int x_vert = c.x - radius_trans * 10 / 18;                        // Здесь заканчивается линия базы внутри окружности
    wxPoint coord_base{ 90, c.y };
    driwer.MoveTo(90, c.y);
    driwer.LineTo(x_vert, c.y);                                             // База
    driwer.MoveOnDX(-50);
    point_base = driwer.GetCoord();
    dc->DrawCircle(point_base, r);
    dc->DrawText("B", { point_base.x - 3, point_base.y - 20 });

    int y0 = 290;
    int y1 = 410;
    int y2 = 530;
    int y_ground = 720;

    {
        // Рисуем транзистор

        {
            // Наклонные линии

            int dy = radius_trans * 4 / 18;

            int y_top = c.y - radius_trans * 100 / 115;
            int y_bottom = c.y + radius_trans * 100 / 115;

            int xx = c.x + radius_trans * 10 / 20;                      // В этом иксе - пересечение коллектора и эмиттера с окружностью.

            dc->DrawLine(x_vert, c.y - dy, xx, y_top);                  // Верхняя наклонная линия (коллектор)
            dc->DrawLine(x_vert, c.y + dy, xx, y_bottom);               // Нижняя наклонная линия (эмиттер)

            {
                // Стрелка эмиттера

                double length = radius_trans * 10 / 40;

                if (type == "npn")
                {
                    DrawLineWithAngle({ xx, y_bottom }, length, 125);
                    DrawLineWithAngle({ xx, y_bottom }, length, 170);
                }
                else if (type == "pnp")
                {
                    DrawLineWithAngle({ x_vert, c.y + dy }, length, -8);
                    DrawLineWithAngle({ x_vert, c.y + dy }, length, -53);
                }
                else
                {
                    LOG_ERROR("Unknown type transistor");
                }
            }
        }

        {
            // Вертикальная линия базы

            int dy = radius_trans * 4 / 9;

            dc->DrawLine(x_vert, c.y - dy, x_vert, c.y + dy);

            {
                // Рисуем измеритель базы

                driwer.MoveTo(coord_base.x, coord_base.y);

                driwer.LineToY(y_ground);

                DrawGround(driwer.GetX(), driwer.GetY());

                if (!measurerBase)
                {
                    measurerBase = new MeasurerVoltageCurrent(Chan::_B, { driwer.GetX(), y1 }, Dir::Down);
                }

                measurerBase->Draw(*dc);

                if (!sourceVoltageCurrentBase)
                {
                    sourceVoltageCurrentBase = new SourceVoltageCurrent(Chan::_B, { driwer.GetX(), y2 }, Dir::Down);
                }

                sourceVoltageCurrentBase->Draw(*dc);
            }
        }

        {
            // Подложка

            int dy = radius_trans * 4 / 16;
            int x = c.x + (c.x - x_vert) + radius_trans / 10;
            driwer.MoveTo({ x, c.y - dy });
            driwer.LineToY(c.y + dy);                                   // Вертикальная линия подложки

            {
                // Измеритель подложки

                driwer.MoveTo(x, c.y);
                driwer.LineOnDX(150);

                driwer.MoveOnDX(-100);
                point_substrate = driwer.GetCoord();
                dc->DrawCircle(point_substrate, r);
                dc->DrawText("Substr", { point_substrate.x - 20, point_substrate.y - 23 });
                driwer.Restore();

                driwer.LineToY(y_ground);
                DrawGround(driwer.GetX(), driwer.GetY());

                driwer.MoveOnDY(-470);

                if (!measurerSubstrate)
                {
                    measurerSubstrate = new MeasurerVoltageCurrent(Chan::_S, { driwer.GetX(), y1 }, Dir::Down);
                }

                measurerSubstrate->Draw(*dc);

                if (!sourceVoltateCurrentSubstrate)
                {
                    sourceVoltateCurrentSubstrate = new SourceVoltageCurrent(Chan::_S, { driwer.GetX(), y2 }, Dir::Down);
                }

                sourceVoltateCurrentSubstrate->Draw(*dc);
            }
        }
    }

    {
        // Рисуем цепь коллектора

        driwer.MoveTo(x_col, y_col);
        driwer.MoveOnDY(25);
        point_collector = driwer.GetCoord();
        dc->DrawCircle(point_collector, r);
        dc->DrawText("C", { point_collector.x + 7, point_collector.y - 9});
        driwer.Restore();
        driwer.LineOnDX(355);
        driwer.LineToY(y_ground);
        DrawGround(driwer.GetX(), driwer.GetY());

        if (!ampermeterCollector)
        {
            ampermeterCollector = new Ampermeter(Chan::_C, { driwer.GetX(), y0 }, Dir::Down);
        }

        ampermeterCollector->Draw(*dc);

        if (!voltmeterCollector)
        {
            voltmeterCollector = new Voltmeter(Chan::_C, { driwer.GetX(), y1 }, Dir::Down);
        }

        voltmeterCollector->Draw(*dc);

        if (!sourceVoltageCollector)
        {
            sourceVoltageCollector = new SourceVoltage(Chan::_C, { driwer.GetX(), y2 }, Dir::Down);
        }

        sourceVoltageCollector->Draw(*dc);
    }
}


void PanelViewTest::DrawGround(int x, int y)
{
    dc->DrawLine(x - 10, y, x + 10, y);
}


void PanelViewTest::DrawLineWithAngle(const wxPoint &start, double length, double angleDeg)
{
    double angleRad = angleDeg * M_PI / 180.0;

    int endX = start.x + (int)(length * cos(angleRad));
    int endY = start.y - (int)(length * sin(angleRad));  // минус, т.к. Y вниз

    dc->DrawLine(start.x, start.y, endX, endY);
}


wxPoint PanelViewTest::GetCenter() const
{
    return { 210, 300 };
}


void PanelViewTest::CreateControls()
{
    static Test *prev_test = nullptr;

    if (test == prev_test)
    {
        return;
    }

    prev_test = test;

    wxPoint c = GetCenter();

    int x_base = 20;
    int width = MeasurerSourcer::WIDTH_CONTROL;

#define CREATE_BUTTONS_COMBO(name, title, num, func, _x, _y)  \
    name = new ButtonsCombo(this, title, width, titles, tooltips, num, #name, ButtonsCombo::Type::Text);    \
    name->Bind(wxEVT_COMBOBOX, &PanelViewTest::func, this); \
    name->SetPosition( {_x, _y} );

#define CREATE_BUTTONS_COMBO_RANGE(name, title, func, _x, _y)  \
    name = new ComboInput(this, title, width, titles, tooltips, #name);    \
    name->Bind(wxEVT_COMBOBOX, &PanelViewTest::func, this); \
    name->SetPosition( {_x, _y} );

    {
        if (!bcModeScan)
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

            CREATE_BUTTONS_COMBO(bcModeScan, L("Развёртка"), 1, OnChangedModeScan, x_base, 50);
        }

        if (!bcTypeSemiconductor)
        {
            wxArrayString titles;
            titles.push_back("npn");
            titles.push_back("pnp");

            wxArrayString tooltips;
            tooltips.push_back("");

            int temp_width = width;
            width = 40;
            CREATE_BUTTONS_COMBO(bcTypeSemiconductor, L(""), 1, OnChangedTypeSemiconductor, c.x - 20, c.y - 10);
            width = temp_width;
        }
    }

#undef CREATE_BUTTONS_COMBO
#undef CREATE_BUTTONS_COMBO_RANGE
}


void PanelViewTest::OnChangedModeScan(wxCommandEvent &)
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
