// 2025/8/9 09:54:44 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel03_Config/PanelScheme/PanelScheme.h"
#include "MainWindow.h"
#include "Panels/Panel03_Config/Panel03_Config.h"
#include "Utils/SystemDepend.h"
#include "Panels/Panel03_Config/PanelScheme/WindowLibraryTests.h"
#include "Controls/BmpButtonsCombo.h"


PanelScheme::PanelScheme(wxPanel *parent, int x) :
    wxPanel(parent)
{
    SetSize({ MainWindow::WIDTH3, PanelConfig::HEIGHT - 40 });
    SetPosition({ 0, 40 });

    const int width_category = 77;

    wxStaticBox *boxCommutation = new wxStaticBox(this, wxID_ANY, _L("Коммутация"), { x, 100 }, { MainWindow::WIDTH3 - 10, 300 });

    int width = 150;

    {
        wxArrayString choices;
        choices.Add(_L("внутренняя"));
        choices.Add(_L("внешняя"));

        // IDC_COMBO_KOMMUTATOR
        new ButtonsCombo(boxCommutation, "Тип", { 10, SD::Y_SB(20)}, width, choices, 0, 1);

        choices.clear();
        choices.Add(_L("канал") + " C");
        choices.Add(_L("канал") + " B");

        new wxComboBox(boxCommutation, wxID_ANY, choices[0], { x + width_category + 30, SD::Y_SB(50) }, { 110 - 30, TEXTCNTRL_HEIGHT }, choices, wxCB_READONLY);

        new wxComboBox(boxCommutation, wxID_ANY, choices[1], { x + width_category + 30, SD::Y_SB(80) }, { 110 - 30, TEXTCNTRL_HEIGHT }, choices, wxCB_READONLY);

        choices.clear();
        choices.Add("C");
        choices.Add("B");
        choices.Add("E");

        int w = 30;

        comboC = new wxComboBox(boxCommutation, wxID_ANY, choices[0], { 100, 170 }, { w, TEXTCNTRL_HEIGHT }, choices, wxCB_READONLY);
        comboB = new wxComboBox(boxCommutation, wxID_ANY, choices[1], { 40, 210 }, { w, TEXTCNTRL_HEIGHT }, choices, wxCB_DROPDOWN);
        comboE = new wxComboBox(boxCommutation, wxID_ANY, choices[2], { 100, 250 }, { w, TEXTCNTRL_HEIGHT }, choices, wxCB_READONLY);

        comboC->Enable(false);
        comboB->Enable(false);
        comboE->Enable(false);
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
        comboTest = new wxComboBox(boxTest, wxID_ANY, choices[0], { 5, SD::Y_SB(20) }, { 110, TEXTCNTRL_HEIGHT }, choices, wxCB_READONLY);

        btnLoad = new wxButton(boxTest, wxID_ANY, _L("Загрузить"), { 5, SD::Y_SB(50) }, { 110, 30 });

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
        int selection = comboTest->GetSelection();

        btnLoad->Show(comboTest->GetString((uint)selection) == "_USER");
    }
}
