// 2026/04/29 16:03:58 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "GUI/PageTests/PanelViewTest.h"
#include "Utils/GlobalFunctions.h"


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
        wxPaintDC dc(this);

        dc.SetBrush(wxBrush(GetBackgroundColour()));

        dc.SetPen(wxPen(*wxBLACK, 1));

        // Устанавливаем цвет текста
        dc.SetTextForeground(*wxBLACK);

        DrawElement(dc);

        // Устанавливаем шрифт (опционально)
        dc.SetFont(wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));

        // Рисуем текст в левом верхнем углу
        dc.DrawText(test->lib->name + " : " + test->name, 5, 5);
    }

    event.Skip();
}


void PanelViewTest::DrawElement(wxPaintDC &dc)
{
    CreateControls();

    if (test->lib->UGO == "BJT")
    {
        DrawBJT(dc, "npn", GetCenter());
    }
    else if (test->lib->UGO == "BJTS")
    {
        DrawBJTS(dc, "npn", GetCenter());
    }
}


void PanelViewTest::DrawBJT(wxPaintDC &dc, const wxString &type, const wxPoint &c)
{
    dc.DrawLine(c.x + radius / 2, c.y - 2 * radius, c.x + radius / 2, c.y + 2 * radius);    // Эмиттер

    dc.DrawCircle(c, radius);

    int x_vert = c.x - radius * 10 / 18;                    // Здесь заканчивается линия базы внутри окружности

    wxPoint coord_base{ 100, c.y };

    dc.DrawLine(coord_base, { x_vert, c.y });               // База

    {
        // Рисуем транзистор

        {
            // Наклонные линии

            int dy = radius * 4 / 18;

            int y_top = c.y - radius * 100 / 115;
            int y_bottom = c.y + radius * 100 / 115;

            int xx = c.x + radius * 10 / 20;

            dc.DrawLine(x_vert, c.y - dy, xx, y_top);            // Верхняя наклонная линия (коллектор)
            dc.DrawLine(x_vert, c.y + dy, xx, y_bottom);         // Нижняя наклонная линия (эмиттер)

            {
                double length = radius * 10 / 40;

                if (type == "npn")
                {
                    DrawLineWithAngle(dc, { xx, y_bottom }, length, 125);
                    DrawLineWithAngle(dc, { xx, y_bottom }, length, 170);
                }
                else if (type == "pnp")
                {
                    DrawLineWithAngle(dc, { x_vert, c.y + dy }, length, -8);
                    DrawLineWithAngle(dc, { x_vert, c.y + dy }, length, -53);
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

            dc.DrawLine(x_vert, c.y - dy, x_vert, c.y + dy);

            {
                // Рисуем измеритель базы

                wxPoint coord{ coord_base.x, coord_base.y + 150 };

                dc.DrawLine(coord_base, coord);

                int r = 20;

                coord.y += r;

                dc.DrawCircle(coord, r);                                        // Измеритель

                dc.SetFont(wxFont(18, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_EXTRALIGHT));

                GF::DrawTextInCenter(dc, "V", wxRect(wxPoint{ coord.x - r, coord.y - r }, wxPoint{ coord.x + r, coord.y + r }));

                coord.y += r;

                wxPoint coord_end{ coord.x, coord.y + 50 };

                dc.DrawLine(coord, coord_end);

                dc.DrawLine({ coord_end.x - 10, coord_end.y }, { coord_end.x + 10, coord_end.y });
            }
        }

        {
            // Подложка

            int dy = radius * 4 / 16;

            int x = c.x + (c.x - x_vert) + radius / 10;

            dc.DrawLine(x, c.y - dy, x, c.y + dy);

            {
                // Измеритель подложки

                dc.DrawLine(x, c.y, x + 100, c.y);
            }
        }
    }
}


void PanelViewTest::DrawBJTS(wxPaintDC &dc, const wxString &type, const wxPoint &c)
{
    DrawBJT(dc, type, c);
}


void PanelViewTest::DrawLineWithAngle(wxPaintDC &dc, const wxPoint &start, double length, double angleDeg)
{
    double angleRad = angleDeg * M_PI / 180.0;

    int endX = start.x + (int)(length * cos(angleRad));
    int endY = start.y - (int)(length * sin(angleRad));  // минус, т.к. Y вниз

    dc.DrawLine(start.x, start.y, endX, endY);
}


wxPoint PanelViewTest::GetCenter() const
{
    return { 300, 200 };
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

    int dx_substrate = 320;
    int dy_substrate = 150;

    const int width = 200;

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

            bcModeScan = new ButtonsCombo(this, L("Развёртка"), width, titles, tooltips, 1, "bcModeScan", ButtonsCombo::Type::Text);

            bcModeScan->Bind(wxEVT_COMBOBOX, &PanelViewTest::OnEventComboBoxModeScan, this);

            bcModeScan->SetPosition({ x_base, 50 });
        }

        if (!bcTypeSemiconductor)
        {
            wxArrayString titles;
            titles.push_back("npn");
            titles.push_back("pnp");

            wxArrayString tooltips;
            tooltips.push_back("");

            bcTypeSemiconductor = new ButtonsCombo(this, L(""), 40, titles, tooltips, 1, "bcTypeSemiconductor", ButtonsCombo::Type::Text);

            bcTypeSemiconductor->Bind(wxEVT_COMBOBOX, &PanelViewTest::OnEventComboBoxTypeSemiconductor, this);

            bcTypeSemiconductor->SetPosition({ c.x - 20, c.y - 10 });
        }
    }

    {
        {
            wxArrayString titles;
            titles.push_back(L("Током"));
            titles.push_back(L("Напряжением"));
            titles.push_back(L("Земля"));

            wxArrayString tooltips;
            tooltips.push_back(L("База управляется током"));
            tooltips.push_back(L("База управляется напряжением"));
            tooltips.push_back(L("База соединена с землёй"));

            if (!bcBaseModeControl)
            {
                bcBaseModeControl = new ButtonsCombo(this, L("Управление"), width, titles, tooltips, 1, "bcBaseModeControl", ButtonsCombo::Type::Text);

                bcBaseModeControl->Bind(wxEVT_COMBOBOX, &PanelViewTest::OnEventComboBoxBaseModeControl, this);
            }
            else
            {
                bcBaseModeControl->SetChoices(titles, tooltips);
            }

            bcBaseModeControl->SetPosition({ x_base, y_base });
        }

        {
            wxArrayString titles;
            titles.push_back(L("Током"));
            titles.push_back(L("Напряжением"));
            titles.push_back(L("Земля"));

            wxArrayString tooltips;
            tooltips.push_back(L("Подложка управляется током"));
            tooltips.push_back(L("Подложка управляется напряжением"));
            tooltips.push_back(L("Подложка соединена с землёй"));

            if (!bcSubstrateModeControl)
            {
                bcSubstrateModeControl = new ButtonsCombo(this, L("Управление"), width, titles, tooltips, 1, "bcSubstrateModeControl", ButtonsCombo::Type::Text);

                bcSubstrateModeControl->Bind(wxEVT_COMBOBOX, &PanelViewTest::OnEventComboBoxBaseModeControl, this);
            }
            else
            {
                bcSubstrateModeControl->SetChoices(titles, tooltips);
            }

            bcSubstrateModeControl->SetPosition({ x_base + dx_substrate, y_base + dy_substrate });
        }
    }

    y_base += dy_base;

    {
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

            if (!bcBaseStartValue)
            {
                bcBaseStartValue = new ButtonsComboRange(this, L("Начальное значение"), width, titles, tooltips, "bcBaseStartValue");

                bcBaseStartValue->Bind(wxEVT_COMBOBOX, &PanelViewTest::OnEventComboBoxBaseStartValue, this);
            }
            else
            {
                bcBaseStartValue->SetChoices(titles, tooltips);
            }

            bcBaseStartValue->SetPosition({ x_base, y_base });
        }

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

            if (!bcSubstrateStartValue)
            {
                bcSubstrateStartValue = new ButtonsComboRange(this, L("Начальное значение"), width, titles, tooltips, "bcSubstrateStartValue");

                bcSubstrateStartValue->Bind(wxEVT_COMBOBOX, &PanelViewTest::OnEventComboBoxBaseStartValue, this);
            }
            else
            {
                bcSubstrateStartValue->SetChoices(titles, tooltips);
            }

            bcSubstrateStartValue->SetPosition({ x_base + dx_substrate, y_base + dy_substrate });
        }
    }

    dy_base = 35;
    y_base += dy_base;

    {
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

            if (!bcBaseDeltaValue)
            {
                bcBaseDeltaValue = new ButtonsComboRange(this, L("Шаг"), width, titles, tooltips, "bcBaseDeltaValue");

                bcBaseDeltaValue->Bind(wxEVT_COMBOBOX, &PanelViewTest::OnEventComboBoxBaseDeltaValue, this);
            }
            else
            {
                bcBaseDeltaValue->SetChoices(titles, tooltips);
            }

            bcBaseDeltaValue->SetPosition({ x_base, y_base });
        }

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

            if (!bcSubstrateDeltaValue)
            {
                bcSubstrateDeltaValue = new ButtonsComboRange(this, L("Шаг"), width, titles, tooltips, "bcSubstrateDeltaValue");

                bcSubstrateDeltaValue->Bind(wxEVT_COMBOBOX, &PanelViewTest::OnEventComboBoxBaseDeltaValue, this);
            }
            else
            {
                bcSubstrateDeltaValue->SetChoices(titles, tooltips);
            }

            bcSubstrateDeltaValue->SetPosition({ x_base + dx_substrate, y_base + dy_substrate });
        }
    }

    y_base += dy_base;

    {
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

            if (!bcBaseNumMeasures)
            {
                bcBaseNumMeasures = new ButtonsCombo(this, L("Кол-во измерений"), width, titles, tooltips, 3, "bcBaseNumMeasures", ButtonsCombo::Type::Text);

                bcBaseNumMeasures->Bind(wxEVT_COMBOBOX, &PanelViewTest::OnEventComboBoxBaseNumMeasures, this);
            }
            else
            {
                bcBaseNumMeasures->SetChoices(titles, tooltips);
            }

            bcBaseNumMeasures->SetPosition({ x_base, y_base });
        }

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

            if (!bcSubstrateNumMeasures)
            {
                bcSubstrateNumMeasures = new ButtonsCombo(this, L("Кол-во измерений"), width, titles, tooltips, 3, "bcSubstrateNumMeasures", ButtonsCombo::Type::Text);

                bcSubstrateNumMeasures->Bind(wxEVT_COMBOBOX, &PanelViewTest::OnEventComboBoxBaseNumMeasures, this);
            }
            else
            {
                bcSubstrateNumMeasures->SetChoices(titles, tooltips);
            }

            bcSubstrateNumMeasures->SetPosition({ x_base + dx_substrate, y_base + dy_substrate });
        }
    }

    x_base += 120;
    y_base += 30;
    y_base += dy_base;

    {
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

            if (!bcBaseRangeMeasure)
            {
                bcBaseRangeMeasure = new ButtonsComboRange(this, L("Диапазон"), 150, titles, tooltips, "bcBaseRangeMeasure");

                bcBaseRangeMeasure->Bind(wxEVT_COMBOBOX, &PanelViewTest::OnEventComboBoxBaseRangeMeasure, this);
            }
            else
            {
                bcBaseRangeMeasure->SetChoices(titles, tooltips);
            }

            bcBaseRangeMeasure->SetPosition({ x_base, y_base });
        }

        dx_substrate -= 170;

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

            if (!bcSubstrateRangeMeasure)
            {
                bcSubstrateRangeMeasure = new ButtonsComboRange(this, L("Диапазон"), 150, titles, tooltips, "bcSubstrateRangeMeasure");

                bcSubstrateRangeMeasure->Bind(wxEVT_COMBOBOX, &PanelViewTest::OnEventComboBoxBaseRangeMeasure, this);
            }
            else
            {
                bcSubstrateRangeMeasure->SetChoices(titles, tooltips);
            }

            bcSubstrateRangeMeasure->SetPosition({ x_base + dx_substrate, y_base + dy_substrate });
        }
    }

    y_base += dy_base;

    {
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

            if (!bcBaseRangeLimit)
            {
                bcBaseRangeLimit = new ButtonsComboRange(this, L("Ограничение"), 150, titles, tooltips, "bcBaseRangeLimit");

                bcBaseRangeLimit->Bind(wxEVT_COMBOBOX, &PanelViewTest::OnEVentComboBoxBaseRangeLimit, this);
            }
            else
            {
                bcBaseRangeLimit->SetChoices(titles, tooltips);
            }

            bcBaseRangeLimit->SetPosition({ x_base, y_base });
        }

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

            if (!bcSubstrateRangeLimit)
            {
                bcSubstrateRangeLimit = new ButtonsComboRange(this, L("Ограничение"), 150, titles, tooltips, "bcSubstrateRangeLimit");

                bcSubstrateRangeLimit->Bind(wxEVT_COMBOBOX, &PanelViewTest::OnEVentComboBoxBaseRangeLimit, this);
            }
            else
            {
                bcSubstrateRangeLimit->SetChoices(titles, tooltips);
            }

            bcSubstrateRangeLimit->SetPosition({ x_base + dx_substrate, y_base + dy_substrate });
        }
    }
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


void PanelViewTest::OnEventComboBoxBaseRangeMeasure(wxCommandEvent &)
{

}


void PanelViewTest::OnEVentComboBoxBaseRangeLimit(wxCommandEvent &)
{

}
