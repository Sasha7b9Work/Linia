// 2025/8/9 09:54:44 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel03_Config/PanelScheme/PanelScheme.h"
#include "MainWindow.h"
#include "Panels/Panel03_Config/Panel03_Config.h"
#include "Utils/SystemDepend.h"
#include "Panels/Panel03_Config/PanelScheme/WindowLibraryTests.h"
#include "Controls/BmpButtonsCombo.h"
#include "Controls/Bitmap.h"


PanelScheme::PanelScheme(wxPanel *parent, const int x) :
    wxPanel(parent)
{
    SetSize({ MainWindow::WIDTH3, PanelConfig::HEIGHT - 40 });
    SetPosition({ 0, 40 });

    const int width_category = 77;

    wxStaticBox *boxCommutation = new wxStaticBox(this, wxID_ANY, _L("Коммутация"), { x, 100 }, { MainWindow::WIDTH3 - 10, 350 });

    {
        wxArrayString choices;
        choices.Add(_L("внутренняя"));
        choices.Add(_L("внешняя"));

        int y = 20;

        // IDC_COMBO_KOMMUTATOR         m_iKommutator           OnSelchangeComboKommutator
        new ButtonsCombo(boxCommutation, "Тип", { 10, SD::Y_SB(20)}, PanelConfig::WIDTH_COMBO, choices, choices, 1);

        choices.clear();
        choices.Add(_L("канал") + " C");
        choices.Add(_L("канал") + " B");

        int delta = 60;

        y += 40;

        // IDC_COMBO_GNEZDO_C           m_iGnezdoC              OnSelchangeComboGnezdoC
        new ButtonsCombo(boxCommutation, "", { 10 + delta, SD::Y_SB(y) }, PanelConfig::WIDTH_COMBO - delta, choices, choices, 1);

        painterJackC = new PainterBMP(boxCommutation, { 20, SD::Y_SB(y) }, wxDefaultSize, "sch/jacks/jack_C.bmp", new wxColour(241, 241, 241));

        y += 40;

        // IDC_COMBO_GNEZDO_B
        new ButtonsCombo(boxCommutation, "", { 10 + delta, SD::Y_SB(y) }, PanelConfig::WIDTH_COMBO - delta, choices, choices, 1);

        painterJackB = new PainterBMP(boxCommutation, { 20, SD::Y_SB(y) }, wxDefaultSize, "sch/jacks/jack_B.bmp", new wxColour(241, 241, 241));

        y += 40;

        painterJackS = new PainterBMP(boxCommutation, { 20, SD::Y_SB(y) }, wxDefaultSize, "sch/jacks/jack_S.bmp", new wxColour(241, 241, 241));

        y += 40;

        painterJackE = new PainterBMP(boxCommutation, { 20, SD::Y_SB(y) }, wxDefaultSize, "sch/jacks/jack_E.bmp", new wxColour(241, 241, 241));

        choices.clear();
        choices.Add("C");
        choices.Add("B");
        choices.Add("E");

        int w = 30;

        y =  260;
        int dy = 50;
        int x0 = 60;
        int dx = 70;

        // IDC_COMBOCHECKKOL
        comboC = new ButtonsCombo(boxCommutation, "", { x0 + dx, y - dy }, w, choices, choices, 1);

        // IDC_COMBOCHECKBAZA
        comboB = new ButtonsCombo(boxCommutation, "", { x0, y }, w, choices, choices, 1);

        // IDC_COMBOCHECKDOP
        comboE = new ButtonsCombo(boxCommutation, "", { x0 + dx, y + dy }, w, choices, choices, 1);
    }

    wxStaticBox *boxCategory = new wxStaticBox(this, wxID_ANY, _L("Категория"), { x, 0 }, { width_category, 100 });

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
            "Полевой или МОП транзистор NMOS(трёхполюсный)",
            "Полевой или МОП транзистор PMOS(трёхполюсный)",
            "Биполярный NPN-транзистор (четырёхполюсный)",
            "Биполярный PNP-транзистор (четырёхполюсный)",
            "Полевой или МОП транзистор NMOS(четырёхполюсный)",
            "Полевой или МОП транзистор PMOS(четырёхполюсный)"
        };

        new BmpButtonsCombo(boxCategory, "Категория", { 18, SD::Y_SB(25) }, {32, 42}, files, tooltips, 0, 4);
    }

    (void)boxCategory;

    wxStaticBox *boxTest = new wxStaticBox(this, wxID_ANY, _L("Тест"), { x + width_category + 5, 0 }, { MainWindow::WIDTH3 - width_category - 15, 100 });

    Bind(wxEVT_BUTTON, &PanelScheme::OnEventButton, this);
    Bind(wxEVT_COMBOBOX, &PanelScheme::OnEventComboBox, this);

    {
        wxArrayString choices;
        choices.Add("IdVd");
        choices.Add("_USER");

        // IDC_COMBOMOD
        // IDC_BUTTON_LOADTST
        comboTest = new ButtonsCombo(boxTest, "", { 5, SD::Y_SB(20) }, 100, choices, choices, 1);

        btnLoad = new wxButton(boxTest, wxID_ANY, _L("Загрузить"), { 5, SD::Y_SB(50) }, { 100, 30 });

        btnLoad->Hide();
    }
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
}
