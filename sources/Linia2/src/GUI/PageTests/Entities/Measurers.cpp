// 2026/09/04 12:20:07 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "GUI/PageTests/Entities/Measurers.h"
#include "Utils/GlobalFunctions.h"
#include "GUI/Controls/ButtonCombo.h"
#include "GUI/PageTests/PanelViewTest.h"


#define CREATE_BUTTONS_COMBO_RANGE(name, title, _x, _y)             \
    name = new ComboInput(this, title, WIDTH_CONTROL, titles, tooltips, #name); \
    name->SetPosition({ _x, _y });


MeasurerSourcer::MeasurerSourcer(Type::E _type, Chan::E _chan, const wxPoint _center, Dir::E _dir) :
    type(_type), chan(_chan), dir(_dir), center(_center)
{
}


void MeasurerSourcer::Draw(wxPaintDC &dc)
{
    if (!disabled)
    {
        dc.DrawCircle(center, radius);


        dc.SetFont(wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_EXTRALIGHT));

        static const pchar labels[Type::Count] =
        {
            "V",
            "I",
            "",
            ""
        };

        GF::DrawTextInCenter(dc, labels[type], wxRect(wxPoint{ center.x - radius, center.y - radius }, wxPoint{ center.x + radius, center.y + radius }));

        const int dY = 3;

        if (type == Type::SourceI)
        {
            const int ddY = 3;

            dc.DrawLine(center.x, center.y - dY - ddY, center.x, center.y - radius);
            dc.DrawLine(center.x, center.y + dY - ddY, center.x, center.y + radius);

            const int l = 5;

            dc.DrawLine(center.x, center.y - dY - ddY, center.x - l, center.y - dY + l - ddY);
            dc.DrawLine(center.x, center.y - dY + dY - ddY, center.x - l, center.y - dY + l + dY - ddY);

            dc.DrawLine(center.x, center.y - dY - ddY, center.x + l, center.y - dY + l - ddY);
            dc.DrawLine(center.x, center.y - dY + dY - ddY, center.x + l, center.y - dY + l + dY - ddY);
        }
        else if (type == Type::SourceU)
        {
            dc.DrawLine(center.x, center.y - radius + dY, center.x + dY, center.y + dY);
            dc.DrawLine(center.x, center.y - radius + dY, center.x - dY, center.y + dY);

            dc.DrawLine(center.x, center.y + radius - 5, center.x, center.y - radius + 6);
        }
    }

    int x = center.x;
    int y = center.y;

    wxRect rect = DrawBorder(dc, x, y, radius, 5);

    CreateControls(rect, x, y);

    for (ComboInput *combo : parametersI)
    {
        combo->Enable(!disabled);
    }

    for (ComboInput *combo : parametersU)
    {
        combo->Enable(!disabled);
    }

    if (btnModeSourceUI)
    {
        btnModeSourceUI->Enable(!disabled);
    }
}


void MeasurerSourcer::CreateControls(const wxRect &rect, int x, int y)
{
    if (parametersI.size() == 0 && parametersU.size() == 0)
    {
        wxPoint pos{ x, y };

        wxSize size{ 20, 20 };

        CreateButtonDisable(rect, size, pos);

        if (type == Type::SourceUI)
        {
            btnModeSourceUI = new Button(ThePanelViewTest, "E", size);
            btnModeSourceUI->SetPosition({ pos.x, pos.y + 25 });
            btnModeSourceUI->SetToolTip(L("Включён блок источника напряжения"));
            btnModeSourceUI->Bind(wxEVT_BUTTON, [this](wxCommandEvent &event)
                {
                    if (btnModeSourceUI->GetLabel() == wxString("E"))
                    {
                        btnModeSourceUI->SetLabel("J");
                        btnModeSourceUI->SetToolTip(L("Включён блок источника тока"));
                    }
                    else
                    {
                        btnModeSourceUI->SetLabel("E");
                        btnModeSourceUI->SetToolTip(L("Включён блок источника напряжения"));
                    }

                    event.Skip();
                });
        }

        for (int i = 0; i < 5; i++)
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

            ComboInput *combo = new ComboInput(ThePanelViewTest, L("Ток"), WIDTH_CONTROL, titles, tooltips, wxString::Format("combo%d", i));
            combo->SetPosition({ x, y + i * (ButtonsCombo::HEIGHT + 5) });

            parametersI.push_back(combo);
        }
    }
}


void MeasurerSourcer::CreateButtonDisable(const wxRect &rect, const wxSize &size, wxPoint &pos)
{
    btnDisable = new Button(ThePanelViewTest, "x", size);

    if (type == MeasurerSourcer::Type::MeasI || MeasurerSourcer::Type::MeasU)
    {
        btnDisable->SetToolTip(L("Включить/отключить блок измерителя"));
    }
    else
    {
        btnDisable->SetToolTip(L("Включить/отключить блок источника"));
    }

    btnDisable->Bind(wxEVT_BUTTON, [this](wxCommandEvent &event)
        {
            disabled = !disabled;
            ThePanelViewTest->Refresh();
            event.Skip();
        });

    if (dir == Dir::Left)
    {
        pos = { rect.x + rect.width - btnDisable->GetSize().x - 1, rect.y + 1 };
    }
    else if (dir == Dir::Right)
    {
        pos = { rect.x + 1, rect.y + 1 };
    }

    btnDisable->SetPosition(pos);
}


wxRect MeasurerSourcer::DrawBorder(wxPaintDC &dc, int &x, int &y, int r, int num_controls)
{
    const int d = 5;

    PaintDC paint(dc);

    paint.StorePenBrush();

    dc.SetPen({ disabled ? wxColour(100, 100, 100) : (*wxBLACK), 1, wxPENSTYLE_SHORT_DASH});
    dc.SetBrush(*wxTRANSPARENT_BRUSH);

    int width = WIDTH_CONTROL + d * 2;
    int height = (num_controls * (ButtonsCombo::HEIGHT + d)) + d;

    wxRect rect{ x, y, width, height };

    if (dir == Dir::Left)
    {
        rect.x -= WIDTH_CONTROL + 2 * d + r;
        rect.width += 2 * r + d;
        rect.y -= height / 2;
        dc.DrawRectangle(rect.x, rect.y, rect.width, rect.height);
        x = rect.x + d;
        y = rect.y + d;
    }
    else if (dir == Dir::Up)
    {
        rect.x = x - WIDTH_CONTROL / 2 - d;
        rect.y -= d + r + (ButtonsCombo::HEIGHT + d) * num_controls;
        rect.height += d + r * 2;

        dc.DrawRectangle(rect.x, rect.y, rect.width, rect.height);

        x += d;
        y += d;
    }
    else if (dir == Dir::Right)
    {
        rect.x -= r + d;
        rect.width += 2 * r + d;
        rect.y -= height / 2;

        dc.DrawRectangle(rect.x, rect.y, rect.width, rect.height);

        y = rect.y + d;
        x = rect.x + d * 2 + r * 2;
    }
    if (dir == Dir::Down)
    {
        rect.x = x - WIDTH_CONTROL / 2 - d;
        rect.y = y - d - r;
        rect.height += d + r * 2;

        dc.DrawRectangle(rect.x, rect.y, rect.width, rect.height);

        x += d;
        y += d * 2 + r * 2;
    }

    paint.RestorePenBrush();

    return rect;
}
