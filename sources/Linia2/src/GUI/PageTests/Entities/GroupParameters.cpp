 // 2026/09/04 12:20:07 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "GUI/PageTests/Entities/GroupParameters.h"
#include "Utils/GlobalFunctions.h"
#include "GUI/Controls/ButtonCombo.h"
#include "GUI/PageTests/PanelViewTest.h"
#include "GUI/Controls/SliderHidden.h"
#include "GUI/Controls/Slider.h"
#include "Settings/Tests/SettingsTests.h"


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
        if (dir != Dir::Right)
        {
            dc.StorePenAndSetNew(dc.GetBackground().GetColour());
            dc.DrawRectangle({ rect.x + 1, rect.y + 1, rect.width - 2, rect.height - 2 });
            dc.RestorePen();
        }
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
        if (type.IsSourceU() || type.IsScanner())
        {
            return 3;
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


#define CREATE_COMBO(name, title, vec)                                                                         \
    ComboInput *name = new ComboInput(PanelViewTest::self, title, WIDTH_CONTROL, choices, tooltips, #name);    \
    name->SetPosition({ coord_controls.x, y });                                                                \
    vec.push_back(name);                                                                                       \
    y += ButtonsCombo::HEIGHT + 5;

void GroupParameters::CreateParametersI()
{
    for (wxWindow *object : parametersI)
    {
        object->Destroy();
    }

    parametersI.clear();

    if (chan.IsBase())
    {
        CreateParametersBaseI();
    }
    else if (chan.IsSubstrate())
    {
        CreateParametersSubstrateI();
    }
    else if (chan.IsCollector())
    {
        CreateParametersCollectorI();
    }
}


void GroupParameters::CreateParametersBaseI()
{
    int y = coord_controls.y;

    if (type.IsMeasUandI())
    {
        {
            wxArrayString choices;

            for (RangeI range{ RangeI::_4_5nA }; range.value < RangeI::Count; ++range)
            {
                choices.push_back(range.Name(RowRange::_125));
            }

            wxArrayString tooltips{ L("Начальное измерения тока базы") };

            CREATE_COMBO(comboBaseStartI, L("Ib старт"), parametersI);
        }

        {
            wxArrayString choices;

            for (RangeI range{ RangeI::_4_5nA }; range.value < RangeI::Count; ++range)
            {
                choices.push_back(range.Name(RowRange::_125));
            }

            wxArrayString tooltips{ L("Диапазон измерения тока базы") };

            CREATE_COMBO(comboBaseRangeI, L("Ib размах"), parametersI);
        }

        {
            wxArrayString choices;

            for (RangeI range{ RangeI::_4_5nA }; range.value < RangeI::Count; ++range)
            {
                choices.push_back(range.Name(RowRange::_125));
            }

            wxArrayString tooltips{ L("Ограничение по току базы") };

            CREATE_COMBO(comboBaseLimitI, L("Ib огр"), parametersI);
        }
    }
    else if (type.IsSourceUandI())
    {
        {
            wxArrayString choices;

            for (RangeI range{ RangeI::_4_5nA }; range.value < RangeI::Count; ++range)
            {
                choices.push_back(range.Name(RowRange::_125));
            }

            wxArrayString tooltips{ L("Начальное значение тока базы") };

            CREATE_COMBO(comboBaseStartI, L("Jb старт"), parametersI);
        }

        {
            wxArrayString choices;

            for (RangeI range{ RangeI::_4_5nA }; range.value < RangeI::Count; ++range)
            {
                choices.push_back(range.Name(RowRange::_125));
            }

            wxArrayString tooltips{ L("Изменение между ступеньками") };

            CREATE_COMBO(comboBaseStepI, L("Jb шаг"), parametersI);
        }

        {
            wxArrayString choices;

            for (int i = 1; i < 11; i++)
            {
                choices.push_back(wxString::Format("%d", i));
            }

            wxArrayString tooltips{ L("Количество ступенек") };

            CREATE_COMBO(comboBaseNumSteps, L("Кол-во ступенек"), parametersI);
        }
    }
}


void GroupParameters::CreateParametersSubstrateI()
{
    int y = coord_controls.y;

    if (type.IsMeasUandI())
    {
        {
            wxArrayString choices;

            for (RangeI range{ RangeI::_4_5nA }; range.value < RangeI::Count; ++range)
            {
                choices.push_back(range.Name(RowRange::_125));
            }

            wxArrayString tooltips{ L("Начальное значение измеряемого тока подложки") };

            CREATE_COMBO(comboSubstrateStartI, L("Is старт"), parametersI);
        }

        {
            wxArrayString choices;

            for (RangeI range{ RangeI::_4_5nA }; range.value < RangeI::Count; ++range)
            {
                choices.push_back(range.Name(RowRange::_125));
            }

            wxArrayString tooltips{ L("Диапазон измерения тока подложки") };

            CREATE_COMBO(comboSubstrateRangeI, L("Is размах"), parametersI);
        }

        {
            wxArrayString choices;

            for (RangeI range{ RangeI::_4_5nA }; range.value < RangeI::Count; ++range)
            {
                choices.push_back(range.Name(RowRange::_125));
            }

            wxArrayString tooltips{ L("Напряжение ограничения тока подложки") };

            CREATE_COMBO(comboSubstrateLimitI, L("Is огр"), parametersI);
        }
    }
    if (type.IsSourceUandI())
    {
        {
            wxArrayString choices;

            for (RangeI range{ RangeI::_1nA }; range.value < RangeI::Count; ++range)
            {
                choices.push_back(range.Name(RowRange::_124));
            }

            wxArrayString tooltips{ L("Начальное значение источника тока подложки") };

            CREATE_COMBO(comboSubsrtateStartI, L("Js старт"), parametersI);
        }

        {
            wxArrayString choices;

            for (RangeI range{ RangeI::_1nA }; range.value < RangeI::Count; ++range)
            {
                choices.push_back(range.Name(RowRange::_124));
            }

            wxArrayString tooltips{ L("Разница тока между сосденими ступеньками источника тока подложки") };

            CREATE_COMBO(comboSubsrtateStepI, L("Js шаг"), parametersI);
        }

        {
            wxArrayString choices;

            for (int i = 1; i < 11; i++)
            {
                choices.push_back(wxString::Format("%d", i));
            }

            wxArrayString tooltips{ L("") };

            CREATE_COMBO(comboSubsrtateStepI, L("Кол-во ступенек"), parametersI);
        }
    }
}


void GroupParameters::CreateParametersCollectorI()
{
    int y = coord_controls.y;

    if (type.IsMeasI())
    {
        {
            wxArrayString choices;

            for (RangeI range{ RangeI::_4_5nA }; range.value < RangeI::Count; ++range)
            {
                choices.push_back(range.Name(RowRange::_125));
            }

            wxArrayString tooltips{ L("Диапазон измерения тока в блоке коллектора") };

            CREATE_COMBO(comboCollectroRangeMeasI, L("Ic диап"), parametersI);

            OnEventChangeCommutator();
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

    if (chan.IsBase())
    {
        CreateParametersBaseU();
    }
    else if (chan.IsSubstrate())
    {
        CreateParametersSubstrateU();
    }
    else if (chan.IsCollector())
    {
        CreateParametersCollectorU();
    }
}


void GroupParameters::CreateParametersBaseU()
{
    int y = coord_controls.y;

    if (type.IsMeasUandI())
    {
        {
            wxArrayString choices;

            for (RangeU range{ RangeU::_1nV }; range.value < RangeU::Count; ++range)
            {
                choices.push_back(range.Name(RowRange::_124));
            }

            wxArrayString tooltips{ L("Начальное значение измеряемоого напряжения") };

            CREATE_COMBO(comboBaseStartU, L("Ub старт"), parametersU);
        }

        {
            wxArrayString choices;

            for (RangeU range{ RangeU::_1nV }; range.value < RangeU::Count; ++range)
            {
                choices.push_back(range.Name(RowRange::_124));
            }

            wxArrayString tooltips{ L("Диапазон измерения напряжения базы") };

            CREATE_COMBO(comboBaseRangeU, L("Ub размах"), parametersU);
        }

        {
            wxArrayString choices;

            for (RangeU range{ RangeU::_1nV }; range.value < RangeU::Count; ++range)
            {
                choices.push_back(range.Name(RowRange::_124));
            }

            wxArrayString tooltips{ L("Ограничеие на превышение напряжения") };

            CREATE_COMBO(comboBaseLimitU, L("Ub огр"), parametersU);
        }
    }
    else if (type.IsSourceU() || type.IsSourceUandI())
    {
        {
            wxArrayString choices;

            for (RangeU range{ RangeU::_1nV }; range.value < RangeU::Count; ++range)
            {
                choices.push_back(range.Name(RowRange::_124));
            }

            wxArrayString tooltips{ L("Значение первой ступеньги испытательного напряжения") };

            CREATE_COMBO(comboBaseStartU, L("Eb старт"), parametersU);
        }

        {
            wxArrayString choices;

            for (RangeU range{ RangeU::_1nV }; range.value < RangeU::Count; ++range)
            {
                choices.push_back(range.Name(RowRange::_124));
            }

            wxArrayString tooltips{ L("Напряжение между ступеньками испытательного напряжения") };

            CREATE_COMBO(comboBaseStepU, L("Eb шаг"), parametersU);
        }

        {
            wxArrayString choices;

            for (int i = 1; i < 11; i++)
            {
                choices.push_back(wxString::Format("%d", i));
            }

            wxArrayString tooltips{ L("Количество ступенек") };

            CREATE_COMBO(comboBaseNumSteps, L("Кол-во ступенек"), parametersU);
        }
    }
}


void GroupParameters::CreateParametersSubstrateU()
{
    int y = coord_controls.y;

    if (type.IsMeasUandI())
    {
        {
            wxArrayString choices;

            for (RangeU range{ RangeU::_1nV }; range.value < RangeU::Count; ++range)
            {
                choices.push_back(range.Name(RowRange::_124));
            }

            wxArrayString tooltips{ L("Начальное значение измерения напряжения подложки") };

            CREATE_COMBO(comboSubsrtateStartU, L("Us старт"), parametersU);
        }

        {
            wxArrayString choices;

            for (RangeU range{ RangeU::_1nV }; range.value < RangeU::Count; ++range)
            {
                choices.push_back(range.Name(RowRange::_124));
            }

            wxArrayString tooltips{ L("Диапазон измерения напряжения подложки") };

            CREATE_COMBO(comboSubsrtateRangeU, L("Us размах"), parametersU);
        }

        {
            wxArrayString choices;

            for (RangeU range{ RangeU::_1nV }; range.value < RangeU::Count; ++range)
            {
                choices.push_back(range.Name(RowRange::_124));
            }

            wxArrayString tooltips{ L("Величина ограничения напряжения подложки") };

            CREATE_COMBO(comboSubsrtateLimitU, L("Us огр"), parametersU);
        }
    }
    else if (type.IsSourceUandI())
    {
        {
            wxArrayString choices;

            for (RangeU range{ RangeU::_1nV }; range.value < RangeU::Count; ++range)
            {
                choices.push_back(range.Name(RowRange::_124));
            }

            wxArrayString tooltips{ L("Начальное значение источника напряжения подложки") };

            CREATE_COMBO(comboSubsrtateStartU, L("Es старт"), parametersU);
        }

        {
            wxArrayString choices;

            for (RangeU range{ RangeU::_1nV }; range.value < RangeU::Count; ++range)
            {
                choices.push_back(range.Name(RowRange::_124));
            }

            wxArrayString tooltips{ L("Разница напряжения между сосденими ступеньками источника напряжения подложки") };

            CREATE_COMBO(comboSubsrtateStepU, L("Es шаг"), parametersU);
        }

        {
            wxArrayString choices;

            for (int i = 1; i < 11; i++)
            {
                choices.push_back(wxString::Format("%d", i));
            }

            wxArrayString tooltips{ L("") };

            CREATE_COMBO(comboSubsrtateStepU, L("Кол-во ступенек"), parametersU);
        }
    }
}


void GroupParameters::CreateParametersCollectorU()
{
    int y = coord_controls.y;

    if (type.IsMeasU())
    {
        {
            wxArrayString choices;

            for (RangeU range{ RangeU::_400_500mV }; range.value <= RangeU::_4_5kV; ++range)
            {
                choices.push_back(range.Name(RowRange::_125));
            }

            wxArrayString tooltips{ L("Диапазон измерения напряжения в блоке коллектора") };

            CREATE_COMBO(comboBaseStartU, L("Uc диап"), parametersU);
        }
    }
    else if (type.IsSourceU())
    {
        {
            wxArrayString choices
            {
                L("30 В"),
                L("300 В"),
                L("3000 В")
            };

            wxArrayString tooltips{ L("Диапазон изменения испытательного напряжения") };

            ComboInput *comboCollectorSourceU = new ComboInput(PanelViewTest::self, L("Ec диап"), WIDTH_CONTROL, choices, tooltips, "comboCollectorSourceU", 1);
            comboCollectorSourceU->SetPosition({ coord_controls.x, y });
            parametersU.push_back(comboCollectorSourceU);
            y += ButtonsCombo::HEIGHT + 5;
        }

        {
            SliderInt *sliderStartSourceU = new SliderInt(PanelViewTest::self, WIDTH_CONTROL, 0, 100, "sliderStartSourceU");
            sliderStartSourceU->SetPosition({ coord_controls.x, y });
            sliderStartSourceU->SetToolTip(L("Старт ограничения в процентах"));
            parametersU.push_back(sliderStartSourceU);
            y += ButtonsCombo::HEIGHT + 2;

            sliderStartSourceU->Bind(wxEVT_SLIDER, [this](wxCommandEvent &event)
                {
                    SliderInt *sliderStop = FindByName<SliderInt>("sliderStopSourceU");
                    if (event.GetInt() > sliderStop->GetValue())
                    {
                        FindByName<SliderInt>("sliderStartSourceU")->SetValue(sliderStop->GetValue());
                    }
                    event.Skip();
                });
        }

        {
            SliderInt *sliderStopSourceU = new SliderInt(PanelViewTest::self, WIDTH_CONTROL, 0, 100, "sliderStopSourceU");
            sliderStopSourceU->SetPosition({ coord_controls.x, y });
            sliderStopSourceU->SetToolTip(L("Стоп ограничения в процентах"));
            sliderStopSourceU->SetValue(100, false);
            parametersU.push_back(sliderStopSourceU);
            y += ButtonsCombo::HEIGHT + 5;

            sliderStopSourceU->Bind(wxEVT_SLIDER, [this](wxCommandEvent &event)
                {
                    SliderInt *sliderStart = FindByName<SliderInt>("sliderStartSourceU");
                    if (event.GetInt() < sliderStart->GetValue())
                    {
                        FindByName<SliderInt>("sliderStopSourceU")->SetValue(sliderStart->GetValue());
                    }
                    event.Skip();
                });
        }
    }
}


void GroupParameters::CreateParametersScanner()
{
    if (!type.IsScanner())
    {
        return;
    }

#define CREATE_BUTTONS_COMBO(name, title, num, func, _x, _y)                                                                \
    name = new ButtonsCombo(PanelViewTest::self, title, width, choices, tooltips, num, #name, ButtonsCombo::Type::Text);    \
    name->Bind(wxEVT_COMBOBOX, &GroupParameters::func, this);                                                               \
    name->SetPosition( {_x, _y} );

    int width = GroupParameters::WIDTH_CONTROL;

    int y = coord_controls.y;

    {
        ButtonsCombo *bcScanMode;

        wxArrayString choices;
        choices.Add(TypeScan(TypeScan::SinPos).NameShort());
        choices.Add(TypeScan(TypeScan::SinNeg).NameShort());
        choices.Add(TypeScan(TypeScan::AC).NameShort());
        choices.Add(TypeScan(TypeScan::DCPos).NameShort());
        choices.Add(TypeScan(TypeScan::DCNeg).NameShort());
        choices.Add(TypeScan(TypeScan::ImpPos).NameShort());
        choices.Add(TypeScan(TypeScan::ImpNeg).NameShort());

        wxArrayString tooltips { "" };

        CREATE_BUTTONS_COMBO(bcScanMode, L("Тип"), 1, OnChangedScanMode, 10, SD::Y_SB(20));

        y += ButtonsCombo::HEIGHT + 5;

        bcScanMode->SetPosition({ coord_controls.x, y });

        parametersScanner.push_back(bcScanMode);
    }

    {
        ButtonsCombo *bcScanNumberPoints;

        wxArrayString choices
        {
            "200"
        };

        wxArrayString tooltips{ "Количество точек в ВАХ" };

        CREATE_BUTTONS_COMBO(bcScanNumberPoints, L("Кол-во точек"), 1, OnChangedScanNumberPoints, 10, SD::Y_SB(20 + 30));

        y += ButtonsCombo::HEIGHT + 5;

        bcScanNumberPoints->SetPosition({ coord_controls.x, y });

        parametersScanner.push_back(bcScanNumberPoints);
    }

    {
        ButtonsCombo *bcScanDurationImpulse;

        wxArrayString choices
        {
            "20",
            "50",
            "100"
        };

        wxArrayString tooltips { L("Длительность испытательных импульсов") };

        CREATE_BUTTONS_COMBO(bcScanDurationImpulse, L("Длит. имп."), 1, OnChangedScanDurationImpulse, 10, SD::Y_SB(20 + 30));

        y += ButtonsCombo::HEIGHT + 5;

        bcScanDurationImpulse->SetPosition({ coord_controls.x, y });

        parametersScanner.push_back(bcScanDurationImpulse);
    }

    OnEventChangeCommutator();
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


wxWindow *GroupParameters::FindWindowByName(const wxString &name)
{
    for (auto wnd : parametersI)
    {
        if (wnd->GetName().EndsWith(name))
        {
            return wnd;
        }
    }

    for (auto wnd : parametersU)
    {
        if (wnd->GetName().EndsWith(name))
        {
            return wnd;
        }
    }

    for (auto wnd : parametersScanner)
    {
        if (wnd->GetName().EndsWith(name))
        {
            return wnd;
        }
    }

    return nullptr;
}


template<class T>
T *GroupParameters::FindByName(const wxString &name)
{
    return (T *)FindWindowByName(name);
}


void GroupParameters::OnChangedScanMode(wxCommandEvent &event)
{
    TypeScan::E type_scan = PanelViewTest::self->meas_sourcers.scannerCollector->GetTypeScan();

    if (type_scan == TypeScan::DCPos ||
        type_scan == TypeScan::DCNeg ||
        type_scan == TypeScan::ImpPos ||
        type_scan == TypeScan::ImpNeg)
    {
        wxArrayString choices
        {
            "20",
            "50",
            "100"
        };

        FindByName<ButtonsCombo>("bcScanNumberPoints")->SetChoices(choices);
    }
    else
    {
        wxArrayString choices{ "200" };

        FindByName<ButtonsCombo>("bcScanNumberPoints")->SetChoices(choices);
    }

    PanelViewTest::self->OnEventChangeScanMode();

    event.Skip();
}


void GroupParameters::OnChangedScanNumberPoints(wxCommandEvent &)
{

}


void GroupParameters::OnChangedScanDurationImpulse(wxCommandEvent &)
{

}


void Ampermeter::OnEventChangeCommutator()
{
    if (chan.IsCollector())
    {
        // Заполняем диапазоны измерителя тока

        {
            wxArrayString choices;

            if (PanelViewTest::self->com_controls.commutator->Is3kV())
            {
                FillChoices(choices, RangeI::_4_5A, RangeI::_400_500A, RowRange::_125);
            }
            else
            {
                FillChoices(choices, RangeI::_4_5nA, RangeI::_10A, RowRange::_125);
            }

            FindByName<ComboInput>("comboCollectroRangeMeasI")->SetChoices(choices);
        }
    }
}


void SourceVoltage::OnEventChangeCommutator()
{
    if (chan.IsCollector())
    {
        // Заполняем диапазоны источника напряжения

        {
            wxArrayString choices;

            if (PanelViewTest::self->com_controls.commutator->Is3kV())
            {
                choices.push_back("30 В");
                choices.push_back("300 В");
                choices.push_back("3000 В");
            }
            else
            {
                FillChoices(choices, RangeU::_1V, RangeU::_40_50V, RowRange::_124);
            }

            FindByName<ComboInput>("comboCollectorSourceU")->SetChoices(choices);
        }

    }
}


void Ampermeter::FillChoices(wxArrayString &choices, RangeI::E begin, RangeI::E end, RowRange::E row_range)
{
    for (RangeI range{ begin }; range.value <= end; ++range)
    {
        choices.push_back(range.Name(row_range));
    }
}


void SourceVoltage::FillChoices(wxArrayString &choices, RangeU::E begin, RangeU::E end, RowRange::E row_range)
{
    for (RangeU range{ begin }; range.value <= end; ++range)
    {
        choices.push_back(range.Name(row_range));
    }
}


void Ampermeter::OnEventChangeScanMode()
{
    if (chan.IsCollector())
    {
        auto min_max = GetMinMaxCurrentMeasCollector();

        wxArrayString choices;

        FillChoices(choices, min_max.first, min_max.second, RowRange::_125);

        FindByName<ButtonsCombo>("comboCollectroRangeMeasI")->SetChoices(choices);
    }
}


std::pair<RangeI::E, RangeI::E> Ampermeter::GetMinMaxCurrentMeasCollector()
{
    TypeScan::E type_scan = PanelViewTest::self->meas_sourcers.scannerCollector->GetTypeScan();

    if (PanelViewTest::self->com_controls.commutator->Is3kV())
    {
        if (type_scan == TypeScan::DCPos ||
            type_scan == TypeScan::DCNeg)
        {
            return std::make_pair(RangeI::_4_5nA, RangeI::_2mA);
        }
        else if (type_scan == TypeScan::AC ||
            type_scan == TypeScan::SinPos ||
            type_scan == TypeScan::SinNeg)
        {
            return std::make_pair(RangeI::_400_500nA, RangeI::_10A);
        }
        else            // IMP+, IMP-
        {
            return std::make_pair(RangeI::_40_50uA, RangeI::_10A);
        }
    }
    else
    {
        return std::make_pair(RangeI::_4_5A, RangeI::_400_500A);
    }
}


void Scanner::OnEventChangeCommutator()
{
    if (chan.IsCollector())
    {
        ButtonsCombo *combo = FindByName<ButtonsCombo>("bcScanMode");

        wxArrayString choices;

        if (PanelViewTest::self->com_controls.commutator->Is3kV())
        {
            choices.push_back(TypeScan(TypeScan::SinPos).NameShort());
            choices.push_back(TypeScan(TypeScan::SinNeg).NameShort());
            choices.push_back(TypeScan(TypeScan::AC).NameShort());
            choices.push_back(TypeScan(TypeScan::DCPos).NameShort());
            choices.push_back(TypeScan(TypeScan::DCNeg).NameShort());
            choices.push_back(TypeScan(TypeScan::ImpPos).NameShort());
            choices.push_back(TypeScan(TypeScan::ImpNeg).NameShort());
        }
        else
        {
            choices.push_back(TypeScan(TypeScan::ImpPos).NameShort());
            choices.push_back(TypeScan(TypeScan::ImpNeg).NameShort());
        }

        combo->SetChoices(choices);
    }
}


TypeScan::E Scanner::GetTypeScan()
{
    wxString type_scan = FindByName<ButtonsCombo>("bcScanMode")->GetCurrentString();

    for (int i = 0; i < TypeScan::Count; i++)
    {
        TypeScan t{(TypeScan::E)i };

        if (type_scan == t.NameShort())
        {
            return t.value;
        }
    }

    LOG_ERROR("Нет названия для типа развёртки \"%s\"", type_scan.c_str().AsChar());

    return TypeScan::Count;
}
