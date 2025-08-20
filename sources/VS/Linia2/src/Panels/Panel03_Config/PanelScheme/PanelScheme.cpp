// 2025/8/9 09:54:44 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel03_Config/PanelScheme/PanelScheme.h"
#include "MainWindow.h"
#include "Panels/Panel03_Config/Panel03_Config.h"
#include "Utils/SystemDepend.h"
#include "Panels/Panel03_Config/PanelScheme/WindowLibraryTests.h"
#include "Panels/Panel03_Config/PanelChannelBS/PanelChannelBS.h"
#include "Controls/Bitmap.h"
#include "Device/Tests/Tests.h"


PanelScheme *PanelScheme::self = nullptr;


ComboJack::ComboJack(Channel::E ch, wxWindow *parent, const wxString &title, const wxPoint &pos, int width, const wxArrayString &labels) :
    ButtonsCombo(parent, title, pos, width, labels, labels, 1),
    channel(ch)
{

}


FullJack::FullJack(Channel::E ch, wxWindow *parent, const wxPoint &position, pchar file_jack_bmp, const wxArrayString *choices) :
    wxPanel(parent, wxID_ANY, position, { 180, 30 }),
    channel(ch)
{
    painterBMP = new PainterBMP(this, { 10, 0 }, wxDefaultSize, file_jack_bmp, { 241, 241, 241 });

    if (choices)
    {
        combo = new ButtonsCombo(this, "", { 60, 0 }, PanelConfig::WIDTH_COMBO - 60, *choices, *choices, 1);
    }
}


PanelScheme::PanelScheme(wxPanel *parent, const int x, int w, int h) :
    wxPanel(parent)
{
    self = this;

    SetSize({ MainWindow::WIDTH3, PanelConfig::HEIGHT - 40 });
    SetPosition({ 0, 40 });

    const int width_category = 77;

    StaticBox *boxCategory = new StaticBox(this, _L("Категория"), { x, 0 }, { width_category, 90 });

    {
        wxArrayString files;

        for (int i = 0; i < 10; i++)
        {
            files.push_back(wxString::Format("sch/cat%d.bmp", i + 1));
        }

        wxArrayString tooltips =
        {
            "Диод",
            "Тиристор",
            "Биполярный NPN-транзистор (трёхполюсный)",
            "Биполярный PNP-транзистор (трёхполюсный)",
            "Полевой или МОП транзистор NMOS (трёхполюсный)",
            "Полевой или МОП транзистор PMOS (трёхполюсный)",
            "Биполярный NPN-транзистор (четырёхполюсный)",
            "Биполярный PNP-транзистор (четырёхполюсный)",
            "Полевой или МОП транзистор NMOS (четырёхполюсный)",
            "Полевой или МОП транзистор PMOS (четырёхполюсный)"
        };

        comboCategory = new BmpButtonsCombo(boxCategory, "Категория", { 18, SD::XY0().y }, { 32, 42 }, files, tooltips, 0, 4);
    }

    (void)boxCategory;

    StaticBox *boxTest = new StaticBox(this, _L("Тест"), { x + width_category + 5, 0 },
        { w - width_category - 5, boxCategory->GetSize().y });

    {
        wxArrayString choices;
        choices.Add("IdVd");
        choices.Add("_USER");

        // IDC_COMBOMOD
        // IDC_BUTTON_LOADTST
        comboTest = new ButtonsCombo(boxTest, "", SD::XY0(), 100, choices, choices, 1);

        btnLoad = new wxButton(boxTest, wxID_ANY, _L("Загрузить"), { PanelConfig::X, SD::Y_SB(50) }, { 100, 30 });

        btnLoad->Hide();
    }

    StaticBox *boxCommutation = new StaticBox(this, _L("Коммутация"),
        { x, boxCategory->GetPosition().y + boxCategory->GetSize().y },
        { w, h - boxCategory->GetPosition().y - boxCategory->GetSize().y });

    {
        wxArrayString choices;
        choices.Add(_L("внутренняя"));
        choices.Add(_L("внешняя"));

        int y = 20;

        comboCommutation = new ButtonsCombo(boxCommutation, "Тип", { PanelConfig::X, SD::Y_SB(20)}, PanelConfig::WIDTH_COMBO, choices, choices, 1);

        choices.clear();
        choices.Add(_L("канал") + " C");
        choices.Add(_L("канал") + " B");

        y += 40;

        jack[ChC] = new FullJack(Channel::_C, boxCommutation, {10, SD::Y_SB(y)}, "sch/jacks/jack_C.bmp", &choices);

        int dy = 35;

        y += dy;

        jack[ChB] = new FullJack(Channel::_B, boxCommutation, {10, SD::Y_SB(y)}, "sch/jacks/jack_B.bmp", &choices);

        y += dy;

        jack[ChS] = new FullJack(Channel::_S, boxCommutation, {10, SD::Y_SB(y)}, "sch/jacks/jack_S.bmp", &choices);

        y += dy;

        jack[ChE] = new FullJack(Channel::_E, boxCommutation, {10, SD::Y_SB(y)}, "sch/jacks/jack_E.bmp");

        choices.clear();
        choices.Add("C");
        choices.Add("B");
        choices.Add("E");

        y = 53;
        dy = 52;
        int x0 = 3;
        int dx = 57;

        painter = new PainterScheme(boxCommutation, { 15, SD::Y_SB(220) }, { 170, 130 }, boxCommutation->GetBackgroundColour());

        int width = 45;

        // IDC_COMBOCHECKKOL
        combo[ChC] = new ComboJack(Channel::_C, painter, "", {x0 + dx, y - dy}, width, choices);

        // IDC_COMBOCHECKBAZA
        combo[ChB] = new ComboJack(Channel::_B, painter, "", {x0, y}, width, choices);

        // IDC_COMBOCHECKDOP
        combo[ChE] = new ComboJack(Channel::_E, painter, "", {x0 + dx, y + dy}, width, choices);

        combo[ChS] = new ComboJack(Channel::_S, painter, "", {x0 + 2 * dx, y}, width, choices);
    }

    BuildPanel();

    Bind(wxEVT_BUTTON, &PanelScheme::OnEventButton, this);
    Bind(wxEVT_COMBOBOX, &PanelScheme::OnEventComboBox, this);
}


void PanelScheme::OnEventButton(wxCommandEvent &event)
{
    int id = event.GetId();

    if (id == btnLoad->GetId())
    {
        WindowLibraryTests().ShowModal();
    }
}


void PanelScheme::OnEventComboBox(wxCommandEvent &event)
{
    int id = event.GetId();

    if (id == comboTest->GetId())
    {
        btnLoad->Show(comboTest->GetCurrentString() == "_USER");
    }
    else if (id == comboCategory->GetId() ||
        id == comboCommutation->GetId())
    {
        BuildPanel();
    }
}


void PanelScheme::BuildPanel()
{
    PanelConfig::self->btnChannelB->Enable(PanelChannelB::self->IsEnabled());
    PanelConfig::self->btnChannelS->Enable(PanelChannelS::self->IsEnabled());

    for (auto _jack : jack)
    {
        _jack->TuneState();
    }

    for (auto _combo : combo)
    {
        _combo->TuneState();
    }

    painter->Build();
}


void ComboJack::TuneState()
{
    SetChoices();

    SetVisibility();

    Enable(!TypeCommutation::IsInternal());
}


void FullJack::TuneState()
{
    SetChoices();

    SetVisibility();

    if (combo)
    {
        combo->Enable(TypeCommutation::IsInternal());
    }
}


void ComboJack::SetChoices()
{
    Category::E cat = Category::Current();

    wxArrayString choices;

    if (cat == Category::Diod)
    {
        StateJack::PrepareArray(choices, StateJack::_C, StateJack::_E);
        ButtonsCombo::SetChoices(choices, choices);
        SetChoice(StateJack::_C);
    }
    else
    {
        StateJack::PrepareArray(choices, StateJack::_C, StateJack::_B, StateJack::_E, StateJack::Break);
        ButtonsCombo::SetChoices(choices, choices);
        SetChoice(StateJack::_C);
    }
}


void FullJack::SetChoices()
{
    Category::E cat = Category::Current();

    if (channel == Channel::_C)
    {
        if (cat == Category::Diod)
        {
            wxArrayString choices;
            StateJack::PrepareArray(choices, StateJack::Break, StateJack::_C);
            combo->SetChoices(choices, choices);
            SetChoice(StateJack::_C);
        }
    }
}


void FullJack::SetChoice(StateJack::E state)
{
    combo->SetChoice(StateJack::Name(state));
}


void ComboJack::SetChoice(StateJack::E state)
{
    ButtonsCombo::SetChoice(StateJack::Name(state));
}


void FullJack::SetVisibility()
{
    Show(Channel(channel).IsVisible());
}


void ComboJack::SetVisibility()
{
    Show(Channel(channel).IsVisible());
}
