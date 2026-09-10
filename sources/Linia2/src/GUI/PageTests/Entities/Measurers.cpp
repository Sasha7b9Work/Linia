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
    coord_controls.x = center.x;
    coord_controls.y = center.y;

    wxRect rect = DrawBorder(dc, coord_controls.x, coord_controls.y, radius, 4);

    CreateControls(rect);

    for (wxWindow *object : parametersI)
    {
        object->Enable(!disabled);
    }

    for (wxWindow *object : parametersU)
    {
        object->Enable(!disabled);
    }

    if (btnModeUI)
    {
        btnModeUI->Enable(!disabled);
    }

    DrawUGO(dc);
}


void MeasurerSourcer::DrawUGO(wxPaintDC &dc)
{
    if (!disabled)
    {
        dc.DrawCircle(center, radius);

        const int dY = 3;

        if (type == Type::SourceI ||
            (type == Type::SourceUI && !IsSetModeU()))
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
        else if (type == Type::SourceU ||
            (type == Type::SourceUI && IsSetModeU()))
        {
            dc.DrawLine(center.x, center.y - radius + dY, center.x + dY, center.y + dY);
            dc.DrawLine(center.x, center.y - radius + dY, center.x - dY, center.y + dY);

            dc.DrawLine(center.x, center.y + radius - 5, center.x, center.y - radius + 6);
        }

        dc.SetFont(wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_EXTRALIGHT));

        GF::DrawTextInCenter(dc, SymbolUGO(), wxRect(wxPoint{ center.x - radius, center.y - radius }, wxPoint{ center.x + radius, center.y + radius }));
    }
}


pchar MeasurerSourcer::SymbolUGO()
{
    if (type == Type::MeasU)
    {
        return "V";
    }
    else if (type == Type::MeasI)
    {
        return "I";
    }
    else if (type == Type::MeasUI)
    {
        return IsSetModeU() ? "V" : "I";
    }

    return "";
}


void MeasurerSourcer::CreateControls(const wxRect &rect)
{
    if (parametersI.size() == 0 && parametersU.size() == 0)
    {
        CreateParametersI();

        CreateParametersU();

        wxPoint pos{ coord_controls };

        wxSize size{ 20, 20 };

        CreateButtonDisable(rect, size, pos);

        CreateButtonModeUI(rect, size, pos);

        ShowNeedParameters();
    }
}


void MeasurerSourcer::ShowNeedParameters()
{
    if (btnModeUI)
    {
        std::vector<wxWindow *> *hidden = &parametersU;
        std::vector<wxWindow *> *shownen = &parametersI;

        if (IsSetModeU())
        {
            hidden = &parametersI;
            shownen = &parametersU;
        }

        for (wxWindow *combo : *hidden)
        {
            combo->Hide();
        }

        for (wxWindow *combo : *shownen)
        {
            combo->Show();
        }
    }
}


bool MeasurerSourcer::IsSetModeU() const
{
    if (!btnModeUI)
    {
        LOG_ERROR("Not created button UI");
    }

    return (btnModeUI->GetLabel() == wxString("V") ||
        btnModeUI->GetLabel() == wxString("E"));
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


#define CREATE_COMBO(title, name, vec)                                                                  \
    ComboInput *combo = new ComboInput(ThePanelViewTest, title, WIDTH_CONTROL, titles, tooltips, name); \
    combo->SetPosition({ coord_controls.x, y });                                                        \
    vec.push_back(combo);                                                                               \
    y += ButtonsCombo::HEIGHT + 5;                                                                      \
    titles.clear();                                                                                     \
    tooltips.clear();

void MeasurerSourcer::CreateParametersI()
{
    for (wxWindow *object : parametersI)
    {
        object->Destroy();
    }

    parametersI.clear();

    wxArrayString titles;
    wxArrayString tooltips;

    int y = coord_controls.y;

    if (chan == Chan::_B)
    {
        if (type == Type::MeasI || type == Type::MeasUI)
        {
            for (RangeI range{ RangeI::_4_5nA }; range.value < RangeI::Count; ++range)
            {
                titles.push_back(range.Name(RowRange::_125));
            }

            tooltips.push_back(L("Диапазон измерения тока базы"));

            CREATE_COMBO(L("Ib макс"), "comboBaseMeasI", parametersI);
        }
        if (type == Type::SourceI || type == Type::SourceUI)
        {
            {
                titles.push_back("1 нА");
                titles.push_back("2 нА");

                tooltips.push_back(L("Шаг"));

                CREATE_COMBO(L("Ib шаг"), "comboBaseSourceI", parametersI);
            }

            {
                SliderFloat *slider = new SliderFloat(ThePanelViewTest, WIDTH_CONTROL, L("Смещение"));
                slider->SetToolTip(L("Смещение"));
                slider->SetPosition({ coord_controls.x, y });
                slider->SetRange(0.0, 10.0, "mA", 2);
                parametersI.push_back(slider);
                y += ButtonsCombo::HEIGHT + 5;
            }

            {
                titles.push_back(L("+"));
                titles.push_back(L("-"));

                tooltips.push_back(L("Полярность смещения"));

                CREATE_COMBO(L("Полярность"), "comboBasePolarityI", parametersI);
            }

            {
                for (int i = 1; i < 11; i++)
                {
                    titles.push_back(wxString::Format("%d", i));
                }

                tooltips.push_back(L("Количество ступенек"));

                CREATE_COMBO(L("Кол-во ступенек"), "comboBaseNumSteps", parametersI);
            }
        }
    }
    else if (chan == Chan::_S)
    {
        if (type == Type::MeasI || type == Type::MeasUI)
        {
            for (RangeI range{ RangeI::_4_5nA }; range.value < RangeI::Count; ++range)
            {
                titles.push_back(range.Name(RowRange::_125));
            }

            tooltips.push_back(L("Диапазон измерения тока базы"));

            CREATE_COMBO(L("Предел"), "comboBaseMeasI", parametersI);
        }
        if (type == Type::SourceI || type == Type::SourceUI)
        {
            {
                titles.push_back("1 нА");
                titles.push_back("2 нА");

                tooltips.push_back(L("Шаг"));

                CREATE_COMBO(L("Шаг"), "comboBaseSourceI", parametersI);
            }

            {
                SliderFloat *slider = new SliderFloat(ThePanelViewTest, WIDTH_CONTROL, L("Смещение"));
                slider->SetToolTip(L("Смещение"));
                slider->SetPosition({ coord_controls.x, y });
                slider->SetRange(0.0, 10.0, "mA", 2);
                parametersI.push_back(slider);
                y += ButtonsCombo::HEIGHT + 5;
            }

            {
                titles.push_back(L("+"));
                titles.push_back(L("-"));

                tooltips.push_back(L("Полярность смещения"));

                CREATE_COMBO(L("Полярность"), "comboSubstratePolarityI", parametersI);
            }

            {
                for (int i = 1; i < 11; i++)
                {
                    titles.push_back(wxString::Format("%d", i));
                }

                tooltips.push_back(L("Количество ступенек"));

                CREATE_COMBO(L("Кол-во ступенек"), "comboSubstarteNumSteps", parametersI);
            }
        }
    }
    else if (chan == Chan::_C)
    {
        if (type == Type::MeasI)
        {
            for (RangeI range{ RangeI::_4_5nA }; range.value < RangeI::Count; ++range)
            {
                titles.push_back(range.Name(RowRange::_125));
            }

            tooltips.push_back(L("Диапазон измерения тока коллектора"));

            CREATE_COMBO(L("Ic макс"), "comboBaseMeasI", parametersI);
        }
    }
}


void MeasurerSourcer::CreateParametersU()
{
    for (wxWindow *object : parametersU)
    {
        object->Destroy();
    }

    parametersU.clear();

    wxArrayString titles;
    wxArrayString tooltips;

    int y = coord_controls.y;

    if (chan == Chan::_B)
    {
        if (type == Type::MeasU || type == Type::MeasUI)
        {
            for (RangeU range{ RangeU::_1nV }; range.value < RangeU::Count; ++range)
            {
                titles.push_back(range.Name(RowRange::_124));
            }

            tooltips.push_back(L("Диапазон измерения напряжения базы"));

            CREATE_COMBO(L("Ub макс"), "comboBaseMeasU", parametersU);
        }
        else if (type == Type::SourceU || type == Type::SourceUI)
        {
            {
                titles.push_back("1 мВ");
                titles.push_back("2 мВ");

                tooltips.push_back(L("Шаг изменения испытательного напряжения"));

                CREATE_COMBO(L("Ub шаг"), "comboBaseSourceU", parametersU);
            }

            {
                SliderFloat *slider = new SliderFloat(ThePanelViewTest, WIDTH_CONTROL, L("Смещение"));
                slider->SetToolTip(L("Смещение"));
                slider->SetPosition({ coord_controls.x, y });
                slider->SetRange(0.0, 10.0, "mA", 2);
                parametersU.push_back(slider);
                y += ButtonsCombo::HEIGHT + 5;
            }

            {
                titles.push_back(L("+"));
                titles.push_back(L("-"));

                tooltips.push_back(L("Полярность смещения"));

                CREATE_COMBO(L("Полярность"), "comboBasePolarityU", parametersU);
            }

            {
                for (int i = 1; i < 11; i++)
                {
                    titles.push_back(wxString::Format("%d", i));
                }

                tooltips.push_back(L("Количество ступенек"));

                CREATE_COMBO(L("Кол-во ступенек"), "comboBaseNumSteps", parametersU);
            }
        }
    }
    else if (chan == Chan::_S)
    {
        if (type == Type::MeasU || type == Type::MeasUI)
        {
            for (RangeU range{ RangeU::_1nV }; range.value < RangeU::Count; ++range)
            {
                titles.push_back(range.Name(RowRange::_124));
            }

            tooltips.push_back(L("Диапазон измерения напряжения подложки"));

            CREATE_COMBO(L("Us макс"), "comboBaseMeasU", parametersU);
        }
        else if (type == Type::SourceU || type == Type::SourceUI)
        {
            {
                titles.push_back("1 мВ");
                titles.push_back("2 мВ");

                tooltips.push_back(L("Шаг изменения испытательного напряжения"));

                CREATE_COMBO(L("Us шаг"), "comboBaseSourceU", parametersU);
            }

            {
                SliderFloat *slider = new SliderFloat(ThePanelViewTest, WIDTH_CONTROL, L("Смещение"));
                slider->SetToolTip(L("Смещение"));
                slider->SetPosition({ coord_controls.x, y });
                slider->SetRange(0.0, 10.0, "mA", 2);
                parametersU.push_back(slider);
                y += ButtonsCombo::HEIGHT + 5;
            }

            {
                titles.push_back(L("+"));
                titles.push_back(L("-"));

                tooltips.push_back(L("Полярность смещения"));

                CREATE_COMBO(L("Полярность"), "comboSubstratePolarityU", parametersU);
            }

            {
                for (int i = 1; i < 11; i++)
                {
                    titles.push_back(wxString::Format("%d", i));
                }

                tooltips.push_back(L("Количество ступенек"));

                CREATE_COMBO(L("Кол-во ступенек"), "comboBaseNumSteps", parametersU);
            }
        }
    }
    else if (chan == Chan::_C)
    {
        if (type == Type::MeasU)
        {
            for (RangeU range{ RangeU::_1nV }; range.value < RangeU::Count; ++range)
            {
                titles.push_back(range.Name(RowRange::_124));
            }

            tooltips.push_back(L("Диапазон измерения напряжения коллектора"));

            CREATE_COMBO(L("Uc макс"), "comboBaseMeasU", parametersU);
        }
        else if (type == Type::SourceU)
        {
            {
                titles.push_back("1 мВ");
                titles.push_back("2 мВ");

                tooltips.push_back(L("Шаг изменения испытательного напряжения"));

                CREATE_COMBO(L("Uc диапазон"), "comboBaseSourceU", parametersU);
            }

            {
                SliderFloat *slider = new SliderFloat(ThePanelViewTest, WIDTH_CONTROL, L("Смещение"));
                slider->SetToolTip(L("Смещение"));
                slider->SetPosition({ coord_controls.x, y });
                slider->SetRange(0.0, 10.0, "mA", 2);
                parametersU.push_back(slider);
                y += ButtonsCombo::HEIGHT + 5;
            }
        }
    }
}


void MeasurerSourcer::CreateButtonModeUI(const wxRect &rect, const wxSize &size, const wxPoint &pos)
{
    if (type == Type::SourceUI)
    {
        btnModeUI = new Button(ThePanelViewTest, "E", size);
        btnModeUI->SetPosition({ pos.x, pos.y + rect.height - 22 });
        btnModeUI->SetToolTip(L("Включён блок источника напряжения"));
        btnModeUI->Bind(wxEVT_BUTTON, [this](wxCommandEvent &event)
            {
                if (btnModeUI->GetLabel() == wxString("E"))
                {
                    btnModeUI->SetLabel("J");
                    btnModeUI->SetToolTip(L("Включён блок источника тока"));
                }
                else
                {
                    btnModeUI->SetLabel(wxString("E"));
                    btnModeUI->SetToolTip(L("Включён блок источника напряжения"));
                }

                ShowNeedParameters();

                ThePanelViewTest->Refresh();

                event.Skip();
            });
    }
    else if (type == Type::MeasUI)
    {
        btnModeUI = new Button(ThePanelViewTest, "V", size);
        btnModeUI->SetPosition({ pos.x, pos.y + rect.height - 22 });
        btnModeUI->SetToolTip(L("Включён блок измерителя напряжения"));
        btnModeUI->Bind(wxEVT_BUTTON, [this](wxCommandEvent &event)
            {
                if (btnModeUI->GetLabel() == wxString("V"))
                {
                    btnModeUI->SetLabel("I");
                    btnModeUI->SetToolTip(L("Включён блок измерителя тока"));
                }
                else
                {
                    btnModeUI->SetLabel("V");
                    btnModeUI->SetToolTip(L("Включён блок измерителя напряжения"));
                }

                ShowNeedParameters();

                ThePanelViewTest->Refresh();

                event.Skip();
            });
    }
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
