// 2026/04/29 16:03:58 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "GUI/PageTests/PanelViewTest.h"
#include "Utils/GlobalFunctions.h"
#include "Utils/LineDrawer.h"
#include "GUI/PageTests/Entities/Measurers.h"
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

    int x_vert = c.x - radius * 10 / 18;                    // Здесь заканчивается линия базы внутри окружности

    wxPoint coord_base{ 50, c.y };

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

                wxPoint coord{ coord_base.x, coord_base.y + 150 };

                dc->DrawLine(coord_base, coord);

                int r = 20;

                coord.y += r;

                Voltmeter(*dc, true).Draw(coord);

                coord.y += r;

                wxPoint coord_end{ coord.x, coord.y + 50 };

                dc->DrawLine(coord, coord_end);

                dc->DrawLine({ coord_end.x - 10, coord_end.y }, { coord_end.x + 10, coord_end.y });
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

                Voltmeter(*dc, true).Draw({ x + dx, y - 80 });
            }
        }
    }

    {
        // Рисуем цепь коллектора

        LineDriwer driwer{ *dc, x_col, y_col };

        int x = driwer.LineOnDX(150);
        int y = driwer.GetY() + 50;
        driwer.LineOnDY(500);

        DrawMeasurerCollectorI(x, y, Dir::Down, &bcCollectorMeasureRangeI, &bcCollectorMeasureLimitI);

        DrawSourceCollectorU(x, y + 300, Dir::Down, &bcCollectorValueStart, &bcCollectorValueFinish);

        DrawGround(x, driwer.GetY());

        driwer.MoveOnDY(-340);
        driwer.LineOnDX(100);
        y = driwer.LineOnDY(150);
        x = driwer.GetX();

        DrawGround(x, y);

        DrawMeasurerCollectorU(x, y - 105, Dir::Down, &bcCollectorMeasureRangeU, &bcCollectorMeasureLimitU);
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
    return { 400, 200 };
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
    int dy_substrate = 250;

    int width = WIDTH_CONTROL;

#define CREATE_BUTTONS_COMBO(name, title, num, func, _x, _y)  \
    name = new ButtonsCombo(this, title, width, titles, tooltips, num, #name, ButtonsCombo::Type::Text);    \
    name->Bind(wxEVT_COMBOBOX, &PanelViewTest::func, this); \
    name->SetPosition( {_x, _y} );

#define CREATE_BUTTONS_COMBO_RANGE(name, title, func, _x, _y)  \
    name = new ButtonsComboRange(this, title, width, titles, tooltips, #name);    \
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

            CREATE_BUTTONS_COMBO(bcModeScan, L("Развёртка"), 1, OnEventComboBoxModeScan, x_base, 50);
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
            CREATE_BUTTONS_COMBO(bcTypeSemiconductor, L(""), 1, OnEventComboBoxTypeSemiconductor, c.x - 20, c.y - 10);
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

            CREATE_BUTTONS_COMBO(bcBaseModeControl, L("Управление"), 1, OnEventComboBoxBaseModeControl, x_base, y_base);
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

            CREATE_BUTTONS_COMBO(bcSubstrateModeControl, L("Управление"), 1, OnEventComboBoxBaseModeControl, x_base + dx_substrate, y_base + dy_substrate);
        }
    }

    y_base += dy_base;

    {
        if (!bcBaseStartValue)
        {
            wxArrayString titles;
            titles.push_back("2 мкА");
            titles.push_back("5 мкA");
            titles.push_back("10 мкА");
            titles.push_back("20 мкА");
            titles.push_back("50 мкА");
            titles.push_back("100 мкА");

            wxArrayString tooltips;
            tooltips.push_back(L("Начальное значение тока базы"));

            CREATE_BUTTONS_COMBO_RANGE(bcBaseStartValue, L("Ib старт"), OnEventComboBoxBaseStartValue, x_base, y_base);
        }

        if (!bcSubstrateStartValue)
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

            CREATE_BUTTONS_COMBO_RANGE(bcSubstrateStartValue, L("Isub старт"), OnEventComboBoxBaseStartValue, x_base + dx_substrate, y_base + dy_substrate);
        }
    }

    dy_base = 35;
    y_base += dy_base;

    {
        if (!bcBaseDeltaValue)
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
            tooltips.push_back(L("Шаг изменения тока базы"));

            CREATE_BUTTONS_COMBO_RANGE(bcBaseDeltaValue, L("Ib шаг"), OnEventComboBoxBaseDeltaValue, x_base, y_base);
        }

        if (!bcSubstrateDeltaValue)
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

            CREATE_BUTTONS_COMBO_RANGE(bcSubstrateDeltaValue, L("Isub шаг"), OnEventComboBoxBaseDeltaValue, x_base + dx_substrate, y_base + dy_substrate);
        }
    }

    y_base += dy_base;

    {
        if (!bcBaseNumMeasures)
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

            CREATE_BUTTONS_COMBO_RANGE(bcBaseNumMeasures, L("N кривых"), OnEventComboBoxBaseNumMeasures, x_base, y_base);
        }

        if (!bcSubstrateNumMeasures)
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

            CREATE_BUTTONS_COMBO_RANGE(bcSubstrateNumMeasures, L("N кривых"), OnEventComboBoxBaseNumMeasures, x_base + dx_substrate, y_base + dy_substrate);
        }
    }

    x_base += 70;
    y_base += 30;
    y_base += dy_base;

    {
        if (!bcBaseMeasureRange)
        {
            wxArrayString titles;
            titles.push_back("50 мВ");
            titles.push_back("100 мВ");
            titles.push_back("200 мВ");
            titles.push_back("500 мВ");
            titles.push_back("1 В");
            titles.push_back("2 В");
            titles.push_back("5 В");
            titles.push_back("10 В");
            titles.push_back("20 В");
            titles.push_back("50 В");

            wxArrayString tooltips;
            tooltips.push_back(L("Диапазон измерения"));

            CREATE_BUTTONS_COMBO_RANGE(bcBaseMeasureRange, L("Ub диап"), OnEventComboBoxBaseMeasureRange, x_base, y_base);
        }

        dx_substrate -= 150;

        if (!bcSubstrateRangeMeasure)
        {
            wxArrayString titles;
            titles.push_back("50 мВ");
            titles.push_back("100 мВ");
            titles.push_back("200 мВ");
            titles.push_back("500 мВ");
            titles.push_back("1 В");
            titles.push_back("2 В");
            titles.push_back("5 В");
            titles.push_back("10 В");
            titles.push_back("20 В");
            titles.push_back("50 В");

            wxArrayString tooltips;
            tooltips.push_back(L("Диапазон измерения"));

            CREATE_BUTTONS_COMBO_RANGE(bcSubstrateRangeMeasure, L("Usub диап"), OnEventComboBoxBaseMeasureRange, x_base + dx_substrate, y_base + dy_substrate);
        }
    }

    y_base += dy_base;

    {
        if (!bcBaseMeasureLimit)
        {
            wxArrayString titles;
            titles.push_back("50 мВ");
            titles.push_back("100 мВ");
            titles.push_back("200 мВ");
            titles.push_back("500 мВ");
            titles.push_back("1 В");
            titles.push_back("2 В");
            titles.push_back("5 В");
            titles.push_back("10 В");
            titles.push_back("20 В");
            titles.push_back("50 В");

            wxArrayString tooltips;
            tooltips.push_back(L(""));

            CREATE_BUTTONS_COMBO_RANGE(bcBaseMeasureLimit, L("Ub огр"), OnEventComboBoxBaseMeasureLimit, x_base, y_base);
        }

        if (!bcSubstrateRangeLimit)
        {
            wxArrayString titles;
            titles.push_back("50 мВ");
            titles.push_back("100 мВ");
            titles.push_back("200 мВ");
            titles.push_back("500 мВ");
            titles.push_back("1 В");
            titles.push_back("2 В");
            titles.push_back("5 В");
            titles.push_back("10 В");
            titles.push_back("20 В");
            titles.push_back("50 В");

            wxArrayString tooltips;
            tooltips.push_back(L(""));

            CREATE_BUTTONS_COMBO_RANGE(bcSubstrateRangeLimit, L("Usub огр"), OnEventComboBoxBaseMeasureLimit, x_base + dx_substrate, y_base + dy_substrate);
        }
    }

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

            CREATE_BUTTONS_COMBO(bcCollectorModeSource, L("Режим"), 1, OnEventComboBoxCollectorModeSource, x, y);
        }
    }

#undef CREATE_BUTTONS_COMBO
#undef CREATE_BUTTONS_COMBO_RANGE
}


void PanelViewTest::OnEventComboBoxModeScan(wxCommandEvent &)
{

}


void PanelViewTest::OnEventComboBoxTypeSemiconductor(wxCommandEvent &)
{

}


void PanelViewTest::OnEventComboBoxBaseModeControl(wxCommandEvent &)
{

}


void PanelViewTest::OnEventComboBoxBaseStartValue(wxCommandEvent &)
{

}


void PanelViewTest::OnEventComboBoxBaseDeltaValue(wxCommandEvent &)
{

}


void PanelViewTest::OnEventComboBoxBaseNumMeasures(wxCommandEvent &)
{

}


void PanelViewTest::OnEventComboBoxBaseMeasureRange(wxCommandEvent &)
{

}


void PanelViewTest::OnEventComboBoxBaseMeasureLimit(wxCommandEvent &)
{

}


void PanelViewTest::OnEventComboBoxCollectorModeSource(wxCommandEvent &)
{

}


void PanelViewTest::OnEventComboBoxCollectorValueStart(wxCommandEvent &)
{

}


void PanelViewTest::OnEventComboBoxCollectorValueFinish(wxCommandEvent &)
{

}


void PanelViewTest::OnEventComboBoxCollectorMeasureRange(wxCommandEvent &)
{

}


void PanelViewTest::OnEventComboBoxCollectorMeasureLimit(wxCommandEvent &)
{

}


void PanelViewTest::DrawBorder(int &x, int &y, int r, Dir::E dir, int /*num_controls*/)
{
    const int d = 5;

    PaintDC paint(*dc);

    paint.StorePenBrush();

    dc->SetPen({ *wxBLACK, 1, wxPENSTYLE_SHORT_DASH });
    dc->SetBrush(*wxTRANSPARENT_BRUSH);

    if (dir == Dir::Right)
    {
        dc->DrawRectangle(x - r - d, y - r - d - 15, WIDTH_CONTROL + 60, 80);

        x += 30;
        y -= 30;
    }
    else if (dir == Dir::Down)
    {
        dc->DrawRectangle(x - WIDTH_CONTROL / 2 - d, y - r - d, WIDTH_CONTROL + 2 * d, 120);

        x -= WIDTH_CONTROL / 2;
        y += 30;
    }

    paint.RestorePenBrush();

}

#define CREATE_BUTTONS_COMBO_RANGE(name, title, func, _x, _y)                               \
    name = new ButtonsComboRange(this, title, WIDTH_CONTROL, titles, tooltips, #name);      \
    name->Bind(wxEVT_COMBOBOX, &PanelViewTest::func, this);


void PanelViewTest::DrawMeasurerCollectorI(int x, int y, Dir::E dir, ButtonsComboRange **cbRange, ButtonsComboRange **cbLimit)
{
    Ampermeter ampermeter(*dc, true);
    ampermeter.Draw({ x, y });

    DrawBorder(x, y, ampermeter.GetRadius(), dir, 2);

    if (!(*cbRange))
    {
        wxArrayString titles;
        titles.push_back(L("10 А"));
        titles.push_back(L("20 А"));

        wxArrayString tooltips;
        tooltips.push_back(L("Диапазон измеряемого тока"));

        CREATE_BUTTONS_COMBO_RANGE((*cbRange), L("Ic диап"), OnEventComboBoxCollectorMeasureRange, x, y);
    }

    (*cbRange)->SetPosition({ x, y });

    if (!(*cbLimit))
    {
        wxArrayString titles;
        titles.push_back(L("10 А"));
        titles.push_back(L("20 А"));

        wxArrayString tooltips;
        tooltips.push_back(L("Предельное знанчение измеряемого тока"));

        CREATE_BUTTONS_COMBO_RANGE((*cbLimit), L("Ic огр"), OnEventComboBoxCollectorMeasureLimit, x, y);
    }

    (*cbLimit)->SetPosition({ x, y + 35 });
}


void PanelViewTest::DrawMeasurerCollectorU(int x, int y, Dir::E dir, ButtonsComboRange **cbRange, ButtonsComboRange **cbLimit)
{
    Voltmeter voltmeter(*dc, true);
    voltmeter.Draw({ x, y });

    DrawBorder(x, y, voltmeter.GetRadius(), dir, 2);

    if (!(*cbRange))
    {
        wxArrayString titles;
        titles.push_back(L("10 А"));
        titles.push_back(L("20 А"));

        wxArrayString tooltips;
        tooltips.push_back(L("Диапазон измеряемого тока"));

        CREATE_BUTTONS_COMBO_RANGE((*cbRange), L("Ic диап"), OnEventComboBoxCollectorMeasureRange, x, y);
    }

    (*cbRange)->SetPosition({ x, y });

    if (!(*cbLimit))
    {
        wxArrayString titles;
        titles.push_back(L("10 А"));
        titles.push_back(L("20 А"));

        wxArrayString tooltips;
        tooltips.push_back(L("Предельное знанчение измеряемого тока"));

        CREATE_BUTTONS_COMBO_RANGE((*cbLimit), L("Ic огр"), OnEventComboBoxCollectorMeasureLimit, x, y);
    }

    (*cbLimit)->SetPosition({ x, y + 35 });
}


void PanelViewTest::DrawSourceCollectorU(int x, int y, Dir::E dir, ButtonsComboRange **cbValueStart, ButtonsComboRange **cbValueFinish)
{
    SourceVoltage sourceVoltage(*dc, true);
    sourceVoltage.Draw({ x, y });

    DrawBorder(x, y, sourceVoltage.GetRadius(), dir, 2);

    if (!(*cbValueStart))
    {
        wxArrayString titles;
        titles.push_back(L("10 В"));
        titles.push_back(L("20 В"));

        wxArrayString tooltips;
        tooltips.push_back(L("Начальное значение испытательного напряжения"));

        CREATE_BUTTONS_COMBO_RANGE((*cbValueStart), L("Uc старт"), OnEventComboBoxCollectorValueStart, x, y);
    }

    (*cbValueStart)->SetPosition({ x, y });

    if (!(*cbValueFinish))
    {
        wxArrayString titles;
        titles.push_back(L("10 В"));
        titles.push_back(L("20 В"));

        wxArrayString tooltips;
        tooltips.push_back(L("Конечное значение испытательного напряжения"));

        CREATE_BUTTONS_COMBO_RANGE((*cbValueFinish), L("Uc стоп"), OnEventComboBoxCollectorValueFinish, x, y);
    }

    (*cbValueFinish)->SetPosition({ x, y + 35 });
}
