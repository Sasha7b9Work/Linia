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

        DrawElement();

        // Устанавливаем шрифт (опционально)
        dc->SetFont(wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));

        // Рисуем текст в левом верхнем углу
        dc->DrawText(test->lib->name + " : " + test->name, 5, 5);

        delete dc;
        dc = nullptr;
    }

    event.Skip();
}


void PanelViewTest::DrawElement()
{
    CreateControls();

    if (test->lib->UGO == "BJT")
    {
        DrawBJT("npn", GetCenter());
    }
    else if (test->lib->UGO == "BJTS")
    {
        DrawBJTS("npn", GetCenter());
    }
}


void PanelViewTest::DrawBJT(const wxString &type, const wxPoint &c)
{
    int x_col = c.x + radius / 2;   // / Координаты точки коммутации
    int y_col = c.y - 2 * radius;   // / с коллектором

    dc->DrawLine(x_col, y_col, c.x + radius / 2, c.y + 2 * radius);    // Вертикальная линия, которая выходит из коллектора и эмиттера

    dc->DrawLine(c.x + radius / 2 - 10, c.y + 2 * radius, c.x + radius / 2 + 10, c.y + 2 * radius);      // Заземление эмиттера

    dc->DrawCircle(c, radius);

    const int x_vert = c.x - radius * 10 / 18;                    // Здесь заканчивается линия базы внутри окружности

    const wxPoint coord_base{ 80, c.y };

    dc->DrawLine(coord_base, { x_vert, c.y });               // База

    {
        // Рисуем транзистор

        {
            // Наклонные линии

            int dy = radius * 4 / 18;

            int y_top = c.y - radius * 100 / 115;
            int y_bottom = c.y + radius * 100 / 115;

            int xx = c.x + radius * 10 / 20;                    // В этом иксе - пересечение коллектора и эмиттера с окружностью.

            dc->DrawLine(x_vert, c.y - dy, xx, y_top);            // Верхняя наклонная линия (коллектор)
            dc->DrawLine(x_vert, c.y + dy, xx, y_bottom);         // Нижняя наклонная линия (эмиттер)

            {
                // Стрелка эмиттера

                double length = radius * 10 / 40;

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

            int dy = radius * 4 / 9;

            dc->DrawLine(x_vert, c.y - dy, x_vert, c.y + dy);

            {
                // Рисуем измеритель базы

                LineDriwer driwer(*dc, coord_base.x, coord_base.y);

                driwer.LineOnDY(500);

                DrawGround(driwer.GetX(), driwer.GetY());

                int y_ground = driwer.GetY();

                driwer.MoveOnDY(-450);

                CreateSourceBaseSubstrate(driwer.GetX(), driwer.GetY(), MeasurerSourcer::Type::SourceI, Dir::Down,
                    &bcBaseSourceStartI, L("Ib старт"),
                    &bcBaseSourceStepI, L("Ib шаг"),
                    &bcBaseNumMeasures, L("N кривых"));

                driwer.MoveOnDY(160);

                CreateSourceBaseSubstrate(driwer.GetX(), driwer.GetY(), MeasurerSourcer::Type::MeasI, Dir::Down,
                    &bcBaseMeasureRangeI, L("Ib диап"),
                    &bcBaseMeasureLimitI, L("Ib огр"));

                driwer.MoveOnDY(180);

                wxPoint coord_meas_u = driwer.GetCoord();

                CreateSourceBaseSubstrate(driwer.GetX(), driwer.GetY(), MeasurerSourcer::Type::MeasU, Dir::Down,
                    &bcBaseMeasureRangeU, L("Ub диап"),
                    &bcBaseMeasureLimitU, L("Ub огр"));

                driwer.MoveOnDY(-70);
                driwer.LineOnDX(140);
                driwer.LineToY(y_ground);

                CreateSourceBaseSubstrate(driwer.GetX(), coord_meas_u.y, MeasurerSourcer::Type::SourceU, Dir::Down,
                    &bcBaseSourceStartU, L("Ub старт"),
                    &bcBaseSourceStepU, L("Ub огр"));

                DrawGround(driwer.GetX(), driwer.GetY());
            }
        }

        {
            // Подложка

            int dy = radius * 4 / 16;

            int x = c.x + (c.x - x_vert) + radius / 10;

            dc->DrawLine(x, c.y - dy, x, c.y + dy);

            {
                // Измеритель подложки

                int dx = 50;
                int y = c.y + 500;

                dc->DrawLine(x, c.y, x + dx, c.y);
                dc->DrawLine(x + dx, c.y, x + dx, y);
                dc->DrawLine(x + dx - 10, y, x + dx + 10, y);

                CreateSourceBaseSubstrate(x + dx, y - 400, MeasurerSourcer::Type::SourceI, Dir::Down,
                    &bcSubstrateStartValueI, L("Isub старт"),
                    &bcSubstrateDeltaValueI, L("Isub шаг"),
                    &bcSubstrateNumMeasures, L("N кривых"));

                CreateSourceBaseSubstrate(x + dx, y - 200, MeasurerSourcer::Type::MeasU, Dir::Down,
                    &bcSubstrateRangeMeasureU, L("Usub диап"),
                    &bcSubstrateRangeLimitU, L("Usub огр"));
            }
        }
    }

    {
        // Рисуем цепь коллектора

        LineDriwer driwer{ *dc, x_col, y_col };

        int x = driwer.LineOnDX(150);
        int y = driwer.GetY() + 50;
        driwer.LineOnDY(500);

        CreateSourceBaseSubstrate(x, y, MeasurerSourcer::Type::MeasI, Dir::Down,
            &bcCollectorMeasureRangeI, L("Ic диап"),
            &bcCollectorMeasureLimitI, L("Ic огр"));

        CreateSourceBaseSubstrate(x, y + 300, MeasurerSourcer::Type::SourceU,  Dir::Down,
            &bcCollectorValueStartU, L("Uc старт"),
            &bcCollectorValueFinishU, L("Uc стоп"));

        DrawGround(x, driwer.GetY());

        driwer.MoveOnDY(-340);
        driwer.LineOnDX(100);
        y = driwer.LineOnDY(150);
        x = driwer.GetX();

        DrawGround(x, y);

        CreateSourceBaseSubstrate(x, y - 105, MeasurerSourcer::Type::MeasU, Dir::Down,
            &bcCollectorMeasureRangeU, L("Uc диап"),
            &bcCollectorMeasureLimitU, L("Uc огр"));
    }
}


void PanelViewTest::DrawGround(int x, int y)
{
    dc->DrawLine(x - 10, y, x + 10, y);
}


void PanelViewTest::DrawBJTS(const wxString &type, const wxPoint &c)
{
    DrawBJT(type, c);
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
    return { 430, 200 };
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
    int y_base = c.y - 40;
    int dy_base = 50;

    int dx_substrate = 250;

    int width = WIDTH_CONTROL;

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

    {
        if(!bcBaseModeControl)
        {
            wxArrayString titles;
            titles.push_back(L("I"));
            titles.push_back(L("U"));
            titles.push_back(L("GND"));

            wxArrayString tooltips;
            tooltips.push_back(L("База управляется током"));
            tooltips.push_back(L("База управляется напряжением"));
            tooltips.push_back(L("База соединена с землёй"));

            CREATE_BUTTONS_COMBO(bcBaseModeControl, L("Управление"), 1, OnChangedBaseModeControl, x_base, 120);
        }

        if (!bcSubstrateModeControl)
        {
            wxArrayString titles;
            titles.push_back(L("I"));
            titles.push_back(L("U"));
            titles.push_back(L("GND"));

            wxArrayString tooltips;
            tooltips.push_back(L("Подложка управляется током"));
            tooltips.push_back(L("Подложка управляется напряжением"));
            tooltips.push_back(L("Подложка соединена с землёй"));

            CREATE_BUTTONS_COMBO(bcSubstrateModeControl, L("Управление"), 1, OnChangedBaseModeControl, x_base + dx_substrate, 120);
        }
    }

    y_base += dy_base;

    dy_base = 35;
    y_base += dy_base;

    y_base += dy_base;

    x_base += 70;
    y_base += 30;
    y_base += dy_base;

    {
        dx_substrate -= 150;
    }

    y_base += dy_base;

    int x = 200;
    int y = 50;

    width = 200;

    {
        if (!bcCollectorModeSource)
        {
            wxArrayString titles;
            titles.push_back(L("Высокое напряжение"));
            titles.push_back(L("Большой ток"));

            wxArrayString tooltips;
            tooltips.push_back(L("Uc <= 3000 В, Ic <= 1 А"));
            tooltips.push_back(L("Uc <= 50 В, Ic <= 50 А"));

            CREATE_BUTTONS_COMBO(bcCollectorModeSource, L("Режим"), 1, OnChangedCollectorModeSource, x, y);
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


void PanelViewTest::DrawBorder(int &x, int &y, int r, Dir::E dir, int num_controls)
{
    const int d = 5;

    PaintDC paint(*dc);

    paint.StorePenBrush();

    dc->SetPen({ *wxBLACK, 1, wxPENSTYLE_SHORT_DASH });
    dc->SetBrush(*wxTRANSPARENT_BRUSH);

    int width = WIDTH_CONTROL + d * 2;
    int height = (num_controls * (ButtonsCombo::HEIGHT + d)) + d;

    if (dir == Dir::Left)
    {
        x -= WIDTH_CONTROL + 2 * d + r;
        width += 2 * r + d;
        y -= height / 2;
        dc->DrawRectangle(x, y, width, height);
        x += d;
        y += d;
    }
    else if (dir == Dir::Up)
    {
        x = x - WIDTH_CONTROL / 2 - d;
        y -= d + r + (ButtonsCombo::HEIGHT + d) * num_controls;
        height += d + r * 2;

        dc->DrawRectangle(x, y, width, height);

        x += d;
        y += d;
    }
    else if (dir == Dir::Right)
    {
        x -= r + d;
        width += 2 * r + d;
        y -= height / 2;

        dc->DrawRectangle(x, y, width, height);

        y += d;
        x += d * 2 + r * 2;
    }
    if (dir == Dir::Down)
    {
        x = x - WIDTH_CONTROL / 2 - d;
        y = y - d - r;
        height += d + r * 2;

        dc->DrawRectangle(x, y, width, height);

        x += d;
        y += d * 2 + r * 2;
    }

    paint.RestorePenBrush();

}

#define CREATE_BUTTONS_COMBO_RANGE(name, title, _x, _y)             \
    name = new ComboInput(this, title, WIDTH_CONTROL, titles, tooltips, #name); \
    name->SetPosition({ _x, _y });


void PanelViewTest::CreateSourceBaseSubstrate(
    int x, int y, MeasurerSourcer::Type::E type, Dir::E dir,
    ComboInput **start, const wxString &label_start,
    ComboInput **step, const wxString &label_step,
    ComboInput **num_curves, const wxString &label_num_curves)
{
    MeasurerSourcer *meas_sourc = new MeasurerSourcer(type, *dc, true);

    meas_sourc->Draw({ x, y });

    DrawBorder(x, y, meas_sourc->GetRadius(), dir, CalculateCombos(start, step, num_curves));

    delete meas_sourc;

    if (!(*start))
    {
        wxArrayString titles;
        titles.push_back("2 мкА");
        titles.push_back("5 мкA");
        titles.push_back("10 мкА");
        titles.push_back("20 мкА");
        titles.push_back("50 мкА");
        titles.push_back("100 мкА");

        wxArrayString tooltips;
        tooltips.push_back(L("Начальное значение тока подложки"));

        CREATE_BUTTONS_COMBO_RANGE((*start), label_start, x, y);
    }

    y += d_combos;

    if (!(*step))
    {
        wxArrayString titles;
        titles.push_back("2 мкА");
        titles.push_back("5 мкA");
        titles.push_back("10 мкА");
        titles.push_back("20 мкА");
        titles.push_back("50 мкА");
        titles.push_back("100 мкА");
        titles.push_back("200 мкА");

        wxArrayString tooltips;
        tooltips.push_back(L("Шаг изменения тока подложки"));

        CREATE_BUTTONS_COMBO_RANGE((*step), label_step, x, y);
    }

    y += d_combos;

    if (num_curves && !(*num_curves))
    {
        wxArrayString titles;
        titles.push_back("1");
        titles.push_back("2");
        titles.push_back("3");
        titles.push_back("4");
        titles.push_back("5");
        titles.push_back("6");
        titles.push_back("7");
        titles.push_back("8");
        titles.push_back("9");
        titles.push_back("10");

        wxArrayString tooltips;
        tooltips.push_back(L("Количество измерений"));

        CREATE_BUTTONS_COMBO_RANGE((*num_curves), label_num_curves, x, y);
    }
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
