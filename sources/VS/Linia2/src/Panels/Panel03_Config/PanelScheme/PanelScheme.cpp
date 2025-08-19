// 2025/8/9 09:54:44 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel03_Config/PanelScheme/PanelScheme.h"
#include "MainWindow.h"
#include "Panels/Panel03_Config/Panel03_Config.h"
#include "Utils/SystemDepend.h"
#include "Panels/Panel03_Config/PanelScheme/WindowLibraryTests.h"
#include "Controls/Bitmap.h"
#include "Device/Tests/Tests.h"


PanelScheme *PanelScheme::self = nullptr;

Jack::Jack(wxWindow *parent, const wxPoint &position, pchar file_jack_bmp, const wxArrayString &choices) :
    wxPanel(parent, wxID_ANY, position, { 180, 50 })
{
    painterBMP = new PainterBMP(this, { 10, 0 }, wxDefaultSize, file_jack_bmp, new wxColour(241, 241, 241));

    combo = new ButtonsCombo(this, "", { 60, 0 }, PanelConfig::WIDTH_COMBO - 60, choices, choices, 1);
}


PanelScheme::PanelScheme(wxPanel *parent, const int x, int w, int h) :
    wxPanel(parent)
{
    self = this;

    SetSize({ MainWindow::WIDTH3, PanelConfig::HEIGHT - 40 });
    SetPosition({ 0, 40 });

    const int width_category = 77;

    wxStaticBox *boxCategory = new wxStaticBox(this, wxID_ANY, _L("Категория"), { x, 0 }, { width_category, 90 });

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

        comboCategory = new BmpButtonsCombo(boxCategory, "Категория", { 18, SD::Y_SB(25) }, { 32, 42 }, files, tooltips, 0, 4);
    }

    (void)boxCategory;

    wxStaticBox *boxTest = new wxStaticBox(this, wxID_ANY, _L("Тест"), { x + width_category + 5, 0 },
        { w - width_category - 5, boxCategory->GetSize().y });

    {
        wxArrayString choices;
        choices.Add("IdVd");
        choices.Add("_USER");

        // IDC_COMBOMOD
        // IDC_BUTTON_LOADTST
        comboTest = new ButtonsCombo(boxTest, "", { PanelConfig::X, SD::Y_SB(20) }, 100, choices, choices, 1);

        btnLoad = new wxButton(boxTest, wxID_ANY, _L("Загрузить"), { PanelConfig::X, SD::Y_SB(50) }, { 100, 30 });

        btnLoad->Hide();
    }

    wxStaticBox *boxCommutation = new wxStaticBox(this, wxID_ANY, _L("Коммутация"),
        { x, boxCategory->GetPosition().y + boxCategory->GetSize().y },
        { w, h - boxCategory->GetPosition().y - boxCategory->GetSize().y });

    {
        wxArrayString choices;
        choices.Add(_L("внутренняя"));
        choices.Add(_L("внешняя"));

        int y = 20;

        // IDC_COMBO_KOMMUTATOR         m_iKommutator           OnSelchangeComboKommutator
        new ButtonsCombo(boxCommutation, "Тип", { PanelConfig::X, SD::Y_SB(20)}, PanelConfig::WIDTH_COMBO, choices, choices, 1);

        choices.clear();
        choices.Add(_L("канал") + " C");
        choices.Add(_L("канал") + " B");

        int delta = 60;

        y += 40;

        // IDC_COMBO_GNEZDO_C           m_iGnezdoC              OnSelchangeComboGnezdoC
        new ButtonsCombo(boxCommutation, "", { 10 + delta, SD::Y_SB(y) }, PanelConfig::WIDTH_COMBO - delta, choices, choices, 1);

        painterJackC = new PainterBMP(boxCommutation, { 20, SD::Y_SB(y) }, wxDefaultSize, "sch/jacks/jack_C.bmp", new wxColour(241, 241, 241));

        y += 40;

        jackB = new Jack(boxCommutation, { 10, SD::Y_SB(y) }, "sch/jacks/jack_B.bmp", choices);

        y += 40;

        painterJackS = new PainterBMP(boxCommutation, { 20, SD::Y_SB(y) }, wxDefaultSize, "sch/jacks/jack_S.bmp", new wxColour(241, 241, 241));

        y += 40;

        painterJackE = new PainterBMP(boxCommutation, { 20, SD::Y_SB(y) }, wxDefaultSize, "sch/jacks/jack_E.bmp", new wxColour(241, 241, 241));

        choices.clear();
        choices.Add("C");
        choices.Add("B");
        choices.Add("E");

        int width = 30;

        y = 53;
        int dy = 52;
        int x0 = 3;
        int dx = 57;

        painter = new PainterScheme(boxCommutation, { 25, SD::Y_SB(220) }, { 150, 130 });

        // IDC_COMBOCHECKKOL
        comboC = new ButtonsCombo(painter, "", { x0 + dx, y - dy }, width, choices, choices, 1);

        // IDC_COMBOCHECKBAZA
        comboB = new ButtonsCombo(painter, "", { x0, y }, width, choices, choices, 1);

        // IDC_COMBOCHECKDOP
        comboE = new ButtonsCombo(painter, "", { x0 + dx, y + dy }, width, choices, choices, 1);

        comboS = new ButtonsCombo(painter, "", { x0 + 2 * dx, y }, width, choices, choices, 1);
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
    else if (id == comboCategory->GetId())
    {
        BuildPanel();
    }
}


void PanelScheme::BuildPanel()
{
    PanelConfig::self->btnChannelB->Enable(PanelChannelB::self->IsEnabled());
    PanelConfig::self->btnChannelS->Enable(PanelChannelS::self->IsEnabled());

    painter->Build();
}
