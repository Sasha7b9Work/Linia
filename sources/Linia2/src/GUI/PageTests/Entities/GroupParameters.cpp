// 2026/09/04 12:20:07 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "GUI/PageTests/Entities/GroupParameters.h"
#include "Utils/GlobalFunctions.h"
#include "GUI/Controls/ButtonCombo.h"
#include "GUI/PageTests/PanelViewTest.h"
#include "GUI/Controls/SliderHidden.h"


#define CREATE_BUTTONS_COMBO_RANGE(name, title, _x, _y)             \
    name = new ComboInput(this, title, WIDTH_CONTROL, titles, tooltips, #name); \
    name->SetPosition({ _x, _y });


GroupParameters::GroupParameters(Type::E _type, Chan::E _chan, Dir::E _dir) :
    type(_type), chan(_chan), dir(_dir)
{
}


void GroupParameters::Draw(AutoBufferedPaintDC &dc)
{
    if (!is_showing)
    {
        return;
    }

    coord_controls.x = center.x;
    coord_controls.y = center.y;

    wxRect rect = DrawBorder(dc, radius);

    CreateControls(rect);

    if (type.IsScanner())
    {
        dc.StorePenAndSetNew(dc.GetBackground().GetColour());
        dc.DrawRectangle({ rect.x + 1, rect.y + 1, rect.width - 2, rect.height - 2 });
        dc.RestorePen();
        dc.DrawTextInCenter(SymbolUGO(), {rect.x, rect.y, rect.width, 30});
    }
    else
    {
        DrawUGO(dc);
    }
}


int GroupParameters::CalculateNumControls() const
{
    if (chan.IsBS())
    {
        if (type.IsMeasUandI() || type.IsSourceUandI())
        {
            return 3;
        }
    }
    else if (chan.IsCollector())
    {
        if (type.IsSourceU() || type.IsScanner() || type.IsMeasI() || type.IsMeasU())
        {
            return 2;
        }
    }

    return 1;
}


void GroupParameters::DrawUGO(AutoBufferedPaintDC &dc)
{
    if (is_enabled)
    {
        if (!type.IsScanner())
        {
            dc.DrawCircle(center, radius);
        }

        const int dY = 3;

        if (type.IsSourceI() ||
            (type.IsSourceUandI() && !IsSetModeU()))
        {
            const int ddY = 3;

            dc.DrawLine(center.x, center.y - dY - ddY, center.x, center.y - radius);
            dc.DrawLine(center.x, center.y + dY - ddY, center.x, center.y + radius);

            const int l = 5;

            dc.DrawLine(center.x, center.y - dY - ddY, center.x - l, center.y - dY + l - ddY);
            dc.DrawLine(center.x, center.y - ddY, center.x - l, center.y + l - ddY);

            dc.DrawLine(center.x, center.y - dY - ddY, center.x + l, center.y - dY + l - ddY);
            dc.DrawLine(center.x, center.y - ddY, center.x + l, center.y + l - ddY);
        }
        else if (type.IsSourceU() ||
            (type.IsSourceUandI() && IsSetModeU()))
        {
            dc.DrawLine(center.x, center.y - radius + dY, center.x + dY, center.y + dY);
            dc.DrawLine(center.x, center.y - radius + dY, center.x - dY, center.y + dY);

            dc.DrawLine(center.x, center.y + radius - 5, center.x, center.y - radius + 6);
        }

        dc.SetFont(wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_EXTRALIGHT));

        if(!type.IsScanner())
        {
            dc.DrawTextInCenter(SymbolUGO(), wxRect(wxPoint{ center.x - radius, center.y - radius }, wxPoint{ center.x + radius, center.y + radius }));
        }
    }
}


wxString GroupParameters::SymbolUGO()
{
    if (type.IsMeasU())
    {
        return "V";
    }
    else if (type.IsMeasI())
    {
        return "I";
    }
    else if (type.IsMeasUandI())
    {
        return IsSetModeU() ? "V" : "I";
    }
    else if (type.IsScanner())
    {
        return L("Развёртка");
    }

    return "";
}


void GroupParameters::CreateControls(const wxRect &rect)
{
    if (parametersI.size() == 0 &&
        parametersU.size() == 0 &&
        parametersScanner.size() == 0)
    {
        CreateParametersI();

        CreateParametersU();

        CreateParametersScanner();

        wxPoint pos{ coord_controls };

        wxSize size{ 20, 20 };

        CreateButtonDisable(rect, size, pos);

        CreateButtonModeUI(rect, size, pos);

        ShowNeedParameters();
    }
}


void GroupParameters::ShowNeedParameters()
{
    if (chan.IsCollector())
    {
        for (auto wnd : parametersI)
        {
            wnd->Show();
        }

        for (auto wnd : parametersU)
        {
            wnd->Show();
        }
    }
    else if (btnModeUI)
    {
        if (IsSetModeU())
        {
            for (auto wnd : parametersI)
            {
                wnd->Hide();
            }

            for (auto wnd : parametersU)
            {
                wnd->Show();
            }
        }
        else
        {
            for (auto wnd : parametersU)
            {
                wnd->Hide();
            }

            for (auto wnd : parametersI)
            {
                wnd->Show();
            }
        }
    }
}


bool GroupParameters::IsSetModeU() const
{
    if (!btnModeUI)
    {
        LOG_ERROR("Not created button UI");
    }

    return (btnModeUI->GetLabel() == wxString("V") ||
        btnModeUI->GetLabel() == wxString("E"));
}


void GroupParameters::CreateButtonDisable(const wxRect &rect, const wxSize &size, wxPoint &pos)
{
    if (type.IsScanner())
    {
        return;
    }

    btnDisable = new Button(PanelViewTest::self, "x", size);

    if (type.IsMeasI() || type.IsMeasU())
    {
        btnDisable->SetToolTip(L("Включить/отключить блок измерителя"));
    }
    else
    {
        btnDisable->SetToolTip(L("Включить/отключить блок источника"));
    }

    btnDisable->Bind(wxEVT_BUTTON, [this](wxCommandEvent &event)
        {
            is_enabled = !is_enabled;
            for (auto wnd : parametersI)
            {
                wnd->Show(is_enabled);
            }
            for (auto wnd : parametersU)
            {
                wnd->Show(is_enabled);
            }
            if (is_enabled)
            {
                ShowNeedParameters();
            }
            if (btnModeUI)
            {
                btnModeUI->Enable(is_enabled);
            }
            PanelViewTest::self->Refresh();
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
    else if (dir == Dir::Down)
    {
        pos = { rect.x + 1 + rect.width - 22, rect.y + 1 };
    }

    btnDisable->SetPosition(pos);
}


#define CREATE_COMBO(title, name, vec)                                                                          \
    ComboInput *combo = new ComboInput(PanelViewTest::self, title, WIDTH_CONTROL, titles, tooltips, name);      \
    combo->SetPosition({ coord_controls.x, y });                                                                \
    vec.push_back(combo);                                                                                       \
    y += ButtonsCombo::HEIGHT + 5;                                                                              \
    titles.clear();                                                                                             \
    tooltips.clear();

void GroupParameters::CreateParametersI()
{
    for (wxWindow *object : parametersI)
    {
        object->Destroy();
    }

    parametersI.clear();

    wxArrayString titles;
    wxArrayString tooltips;

    int y = coord_controls.y;

    if (chan.IsBase())
    {
        if (type.IsMeasUandI())
        {
            {
                for (RangeI range{ RangeI::_4_5nA }; range.value < RangeI::Count; ++range)
                {
                    titles.push_back(range.Name(RowRange::_125));
                }

                tooltips.push_back(L("Начальное измерения тока базы"));

                CREATE_COMBO(L("Ib старт"), "comboBaseStartI", parametersI);
            }

            {
                titles.Clear();
                tooltips.Clear();

                for (RangeI range{ RangeI::_4_5nA }; range.value < RangeI::Count; ++range)
                {
                    titles.push_back(range.Name(RowRange::_125));
                }

                tooltips.push_back(L("Диапазон измерения тока базы"));

                CREATE_COMBO(L("Ib размах"), "comboBaseRangeI", parametersI);
            }

            {
                titles.Clear();
                tooltips.Clear();

                for (RangeI range{ RangeI::_4_5nA }; range.value < RangeI::Count; ++range)
                {
                    titles.push_back(range.Name(RowRange::_125));
                }

                tooltips.push_back(L("Ограничение по току базы"));

                CREATE_COMBO(L("Ib огр"), "comboBaseLimitI", parametersI);
            }
        }
        else if (type.IsSourceUandI())
        {
            {
                for (RangeI range{ RangeI::_4_5nA }; range.value < RangeI::Count; ++range)
                {
                    titles.push_back(range.Name(RowRange::_125));
                }

                tooltips.push_back(L("Начальное значение тока базы"));

                CREATE_COMBO(L("Jb старт"), "comboBaseStartI", parametersI);
            }

            {
                titles.Clear();
                tooltips.Clear();

                for (RangeI range{ RangeI::_4_5nA }; range.value < RangeI::Count; ++range)
                {
                    titles.push_back(range.Name(RowRange::_125));
                }

                tooltips.push_back(L("Изменение между ступеньками"));

                CREATE_COMBO(L("Jb шаг"), "comboBaseStepI", parametersI);
            }

            {
                titles.Clear();
                tooltips.Clear();

                for (int i = 1; i < 11; i++)
                {
                    titles.push_back(wxString::Format("%d", i));
                }

                tooltips.push_back(L("Количество ступенек"));

                CREATE_COMBO(L("Кол-во ступенек"), "comboBaseNumSteps", parametersI);
            }
        }
    }
    else if (chan.IsSubstrate())
    {
        if (type.IsMeasI() || type.IsMeasUandI())
        {
            for (RangeI range{ RangeI::_4_5nA }; range.value < RangeI::Count; ++range)
            {
                titles.push_back(range.Name(RowRange::_125));
            }

            tooltips.push_back(L("Диапазон измерения тока базы"));

            CREATE_COMBO(L("Предел"), "comboBaseMeasI", parametersI);
        }
        if (type.IsSourceI() || type.IsSourceUandI())
        {
            {
                titles.push_back("1 нА");
                titles.push_back("2 нА");

                tooltips.push_back(L("Шаг"));

                CREATE_COMBO(L("Шаг"), "comboBaseSourceI", parametersI);
            }

            {
                SliderHidden *slider = new SliderHidden(PanelViewTest::self, WIDTH_CONTROL, L("Смещение"), "sliderOffsetSourceChannelS");
                slider->SetToolTip(L("Смещение"));
                slider->SetPosition({ coord_controls.x, y });
                slider->SetRange(-10e-3, 10e-3);
                parametersI.push_back(slider);
                y += ButtonsCombo::HEIGHT + 5;
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
    else if (chan.IsCollector())
    {
        if (type.IsMeasI())
        {
            {
                for (RangeI range{ RangeI::_4_5nA }; range.value < RangeI::Count; ++range)
                {
                    titles.push_back(range.Name(RowRange::_125));
                }

                tooltips.push_back(L("Минимальное измеряемое значение тока коллектора"));

                CREATE_COMBO(L("Ic старт"), "comboBaseStartI", parametersI);
            }

            {
                titles.Clear();
                tooltips.Clear();

                for (RangeI range{ RangeI::_4_5nA }; range.value < RangeI::Count; ++range)
                {
                    titles.push_back(range.Name(RowRange::_125));
                }

                tooltips.push_back(L("Диапазон измерения тока коллектора"));

                CREATE_COMBO(L("Ic размах"), "comboBaseDeltaI", parametersI);
            }
        }
    }
}


void GroupParameters::CreateParametersU()
{
    for (wxWindow *object : parametersU)
    {
        object->Destroy();
    }

    parametersU.clear();

    wxArrayString titles;
    wxArrayString tooltips;

    int y = coord_controls.y;

    if (chan.IsBase())
    {
        if (type.IsMeasUandI())
        {
            {
                for (RangeU range{ RangeU::_1nV }; range.value < RangeU::Count; ++range)
                {
                    titles.push_back(range.Name(RowRange::_124));
                }

                tooltips.push_back(L("Начальное значенпие измеряемоого напряжения"));

                CREATE_COMBO(L("Ub старт"), "comboBaseStartU", parametersU);
            }

            {
                titles.Clear();
                tooltips.Clear();

                for (RangeU range{ RangeU::_1nV }; range.value < RangeU::Count; ++range)
                {
                    titles.push_back(range.Name(RowRange::_124));
                }

                tooltips.push_back(L("Диапазон измерения напряжения базы"));

                CREATE_COMBO(L("Ub размах"), "comboBaseRangeU", parametersU);
            }

            {
                titles.Clear();
                tooltips.Clear();

                for (RangeU range{ RangeU::_1nV }; range.value < RangeU::Count; ++range)
                {
                    titles.push_back(range.Name(RowRange::_124));
                }

                tooltips.push_back(L("Ограничеие на превышение напряжения"));

                CREATE_COMBO(L("Ub огр"), "comboBaseLimitU", parametersU);
            }
        }
        else if (type.IsSourceU() || type.IsSourceUandI())
        {
            {
                for (RangeU range{ RangeU::_1nV }; range.value < RangeU::Count; ++range)
                {
                    titles.push_back(range.Name(RowRange::_124));
                }

                tooltips.push_back(L("Значение первой ступеньги испытательного напряжения"));

                CREATE_COMBO(L("Eb старт"), "comboBaseStartU", parametersU);
            }

            {
                titles.Clear();
                tooltips.Clear();

                for (RangeU range{ RangeU::_1nV }; range.value < RangeU::Count; ++range)
                {
                    titles.push_back(range.Name(RowRange::_124));
                }

                tooltips.push_back(L("Напряжение между ступеньками испытательного напряжения"));

                CREATE_COMBO(L("Eb шаг"), "comboBaseStepU", parametersU);
            }

            {
                titles.Clear();
                tooltips.Clear();

                for (int i = 1; i < 11; i++)
                {
                    titles.push_back(wxString::Format("%d", i));
                }

                tooltips.push_back(L("Количество ступенек"));

                CREATE_COMBO(L("Кол-во ступенек"), "comboBaseNumSteps", parametersU);
            }
        }
    }
    else if (chan.IsSubstrate())
    {
        if (type.IsMeasUandI())
        {
            {
                for (RangeU range{ RangeU::_1nV }; range.value < RangeU::Count; ++range)
                {
                    titles.push_back(range.Name(RowRange::_124));
                }

                tooltips.push_back(L("Начальное значение измерения напряжения подложки"));

                CREATE_COMBO(L("Us старт"), "comboSubsrtateStartU", parametersU);
            }

            {
                titles.Clear();
                tooltips.Clear();

                for (RangeU range{ RangeU::_1nV }; range.value < RangeU::Count; ++range)
                {
                    titles.push_back(range.Name(RowRange::_124));
                }

                tooltips.push_back(L("Диапазон измерения напряжения подложки"));

                CREATE_COMBO(L("Us диап"), "comboSubsrtateRangeU", parametersU);
            }

            {
                titles.Clear();
                tooltips.Clear();

                for (RangeU range{ RangeU::_1nV }; range.value < RangeU::Count; ++range)
                {
                    titles.push_back(range.Name(RowRange::_124));
                }

                tooltips.push_back(L("Величина ограничения напряжения подложки"));

                CREATE_COMBO(L("Us ограничение"), "comboSubsrtateLimitU", parametersU);
            }
        }
        else if (type.IsSourceU() || type.IsSourceUandI())
        {
            {
                titles.push_back("1 мВ");
                titles.push_back("2 мВ");

                tooltips.push_back(L("Шаг изменения испытательного напряжения"));

                CREATE_COMBO(L("Us шаг"), "comboBaseSourceU", parametersU);
            }

            {
                SliderHidden *slider = new SliderHidden(PanelViewTest::self, WIDTH_CONTROL, L("Смещение"), "sliderOffsetSourceChannelS");
                slider->SetToolTip(L("Смещение"));
                slider->SetPosition({ coord_controls.x, y });
                slider->SetRange(-10e-3, 10e-3);
                parametersU.push_back(slider);
                y += ButtonsCombo::HEIGHT + 5;
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
    else if (chan.IsCollector())
    {
        if (type.IsMeasU())
        {
            {
                for (RangeU range{ RangeU::_1nV }; range.value < RangeU::Count; ++range)
                {
                    titles.push_back(range.NameStep(RowRange::_124));
                }

                tooltips.push_back(L("Начальное значение напряжения коллектора"));

                CREATE_COMBO(L("Uc старт"), "comboBaseStartU", parametersU);
            }

            {
                titles.Clear();
                tooltips.Clear();

                for (RangeU range{ RangeU::_1nV }; range.value < RangeU::Count; ++range)
                {
                    titles.push_back(range.Name(RowRange::_124));
                }

                tooltips.push_back(L("Диапазон измерения напряжения коллектора"));

                CREATE_COMBO(L("Uc размах"), "comboBaseDeltaU", parametersU);
            }
        }
        else if (type.IsSourceU())
        {
            {
                for (RangeU range{ RangeU::_1nV }; range.value < RangeU::Count; ++range)
                {
                    titles.push_back(range.NameStep(RowRange::_124));
                }

                tooltips.push_back(L("Начальное значние испытательного напряжения"));

                CREATE_COMBO(L("Ec старт"), "comboBaseStartU", parametersU);
            }

            {
                titles.Clear();
                tooltips.Clear();

                for (RangeU range{ RangeU::_1nV }; range.value < RangeU::Count; ++range)
                {
                    titles.push_back(range.Name(RowRange::_124));
                }

                tooltips.push_back(L("Диапазон изменения испытательного напряжения"));

                CREATE_COMBO(L("Ec размах"), "comboBaseRangeU", parametersU);
            }
        }
    }
}


void GroupParameters::CreateParametersScanner()
{
    if (!type.IsScanner())
    {
        return;
    }

#define CREATE_BUTTONS_COMBO(name, title, num, func, _x, _y)                                        \
    name = new ButtonsCombo(PanelViewTest::self, title, width, titles, tooltips, num, #name, ButtonsCombo::Type::Text);  \
    name->Bind(wxEVT_COMBOBOX, &GroupParameters::func, this);                                                 \
    name->SetPosition( {_x, _y} );

    int width = GroupParameters::WIDTH_CONTROL;

    int y = coord_controls.y;

    {
        ButtonsCombo *bcScanMode;

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

        CREATE_BUTTONS_COMBO(bcScanMode, L("Тип"), 1, OnChangedScanMode, 10, SD::Y_SB(20));

        y += ButtonsCombo::HEIGHT + 5;

        bcScanMode->SetPosition({ coord_controls.x, y });

        parametersScanner.push_back(bcScanMode);
    }

    {
        ButtonsCombo *bcScanNumberPoints;

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

        CREATE_BUTTONS_COMBO(bcScanNumberPoints, L("Кол-во точек"), 1, OnChangedScanNumberPoints, 10, SD::Y_SB(20 + 30));

        y += ButtonsCombo::HEIGHT + 5;

        bcScanNumberPoints->SetPosition({ coord_controls.x, y });

        parametersScanner.push_back(bcScanNumberPoints);
    }
}


void GroupParameters::CreateButtonModeUI(const wxRect &rect, const wxSize &size, const wxPoint &pos)
{
    if (type.IsSourceUandI())
    {
        btnModeUI = new Button(PanelViewTest::self, "E", size);
        if (dir == Dir::Left || dir == Dir::Right)
        {
            btnModeUI->SetPosition({ pos.x, pos.y + rect.height - 22 });
        }
        else if (dir == Dir::Down)
        {
            btnModeUI->SetPosition({ rect.x + 1, rect.y + 1 });
        }
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

                PanelViewTest::self->Refresh();

                event.Skip();
            });
    }
    else if (type.IsMeasUandI())
    {
        btnModeUI = new Button(PanelViewTest::self, "V", size);
        if (dir == Dir::Left || dir == Dir::Right)
        {
            btnModeUI->SetPosition({ pos.x, pos.y + rect.height - 22 });
        }
        else if (dir == Dir::Down)
        {
            btnModeUI->SetPosition({ rect.x + 1, rect.y + 1});
        }
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

                PanelViewTest::self->Refresh();

                event.Skip();
            });
    }
}


wxRect GroupParameters::DrawBorder(AutoBufferedPaintDC &dc, int r)
{
    const int d = 5;

    dc.StorePenBrush();

    dc.SetPen({ is_enabled ? (*wxBLACK) : wxColour(150, 150, 150), 1, wxPENSTYLE_SHORT_DASH });
    dc.SetBrush(*wxTRANSPARENT_BRUSH);

    wxRect rect
    {
        coord_controls.x,
        coord_controls.y,
        WIDTH_CONTROL + d * 2,
        (CalculateNumControls() * (ButtonsCombo::HEIGHT + d)) + d
    };

    if (dir == Dir::Left)
    {
        rect.x -= WIDTH_CONTROL + 2 * d + r;
        rect.width += 2 * r + d;
        rect.y -= rect.height / 2;
        dc.DrawRectangle(rect.x, rect.y, rect.width, rect.height);
        coord_controls.x = rect.x + d;
        coord_controls.y = rect.y + d;
    }
    else if (dir == Dir::Up)
    {
        rect.x = coord_controls.x - WIDTH_CONTROL / 2 - d;
        rect.y -= d + r + (ButtonsCombo::HEIGHT + d) * CalculateNumControls();
        rect.height += d + r * 2;

        dc.DrawRectangle(rect.x, rect.y, rect.width, rect.height);

        coord_controls.x += d;
        coord_controls.y += d;
    }
    else if (dir == Dir::Right)
    {
        if (type.IsScanner())
        {
            rect.x -= r + d;
            rect.width += 2 * r + d;
            rect.height += d + ButtonsCombo::HEIGHT;
        }
        else
        {
            rect.x -= r + d;
            rect.width += 2 * r + d;
            rect.y -= rect.height / 2;
        }

        dc.DrawRectangle(rect.x, rect.y, rect.width, rect.height);

        coord_controls.y = rect.y + d;
        coord_controls.x = rect.x + d * 2 + r * 2;
    }
    if (dir == Dir::Down)
    {
        rect.x = coord_controls.x - WIDTH_CONTROL / 2 - d;
        rect.y = coord_controls.y - d - r;
        rect.height += d + r * 2;

        dc.DrawRectangle(rect.x, rect.y, rect.width, rect.height);

        coord_controls.x = rect.x + d;
        coord_controls.y = rect.y + d * 2 + r * 2;
    }

    dc.RestorePenBrush();

    return rect;
}


void GroupParameters::OnChangedScanMode(wxCommandEvent &)
{

}


void GroupParameters::OnChangedScanNumberPoints(wxCommandEvent &)
{

}
