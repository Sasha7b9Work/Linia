// 2025/6/1 17:20:45 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel03_Config/Panel03_Config.h"
#include "MainWindow.h"
#include "Utils/GlobalFunctions.h"
#include "Utils/SystemDepend.h"
#include "Controls/SpinBox.h"
#include "Panels/Panel03_Config/WindowLibraryTests.h"


PanelConfig *PanelConfig::self = nullptr;


PanelConfig::PanelConfig(wxWindow* parent) :
    Panel(parent, MainWindow::WIDTH1 + MainWindow::WIDTH2, 0, MainWindow::WIDTH3, HEIGHT)
{
    self = this;

    Bind(wxEVT_BUTTON, &PanelConfig::OnEventButton, this);
    Bind(wxEVT_TOGGLEBUTTON, &PanelConfig::OnEventToggleButton, this);
    Bind(wxEVT_RADIOBUTTON, &PanelConfig::OnEventRadioButton, this);
    Bind(wxEVT_COMBOBOX, &PanelConfig::OnEventComboBox, this);

    const int h = 20;

    wxSize sizeChan = { 67, h };

    btnChannelC = new wxToggleButton(this, wxID_ANY, _L("Канал C"), { 0, 0 }, sizeChan);
    str_panels.push_back({ btnChannelC, CreatePanel(btnChannelC) });

    btnChannelB = new wxToggleButton(this, wxID_ANY, _L("Канал B"), { sizeChan.x, 0 }, sizeChan);
    str_panels.push_back({ btnChannelB, CreatePanel(btnChannelB) });

    btnChannelS = new wxToggleButton(this, wxID_ANY, _L("Канал S"), { sizeChan.x * 2, 0 }, sizeChan);
    str_panels.push_back({ btnChannelS, CreatePanel(btnChannelS) });

    wxSize sizeScheme = { 120, h };
    btnScheme = new wxToggleButton(this, wxID_ANY, _L("Схема включения"), { 0, h }, sizeScheme);
    str_panels.push_back({ btnScheme, CreatePanel(btnScheme) });

    btnCalculate = new wxToggleButton(this, wxID_ANY, _L("Расчёт"), { sizeScheme.x, h }, { MainWindow::WIDTH3 - sizeScheme.x - 3, h });
    str_panels.push_back({ btnCalculate, CreatePanel(btnCalculate) });

    {
        // Включаем панель

        int id = btnChannelC->GetId();

        wxCommandEvent evt(wxEVT_TOGGLEBUTTON, id);
        evt.SetInt(1);
        this->ProcessWindowEvent(evt);
    }
}

wxPanel *PanelConfig::CreatePanel(wxToggleButton *button)
{
    wxPanel *panel = new wxPanel(self);

    panel->SetSize({ MainWindow::WIDTH3, HEIGHT - 40 });
    panel->SetPosition({ 0, 40 });

    int x = 4;
    int w = MainWindow::WIDTH3 - 2 * x - 1;

    int id = button->GetId();

    if (id == btnChannelC->GetId())
    {
        CreatePanelChannelC(panel, x, w);
    }
    else if (id == btnChannelB->GetId())
    {
        CreatePanelChannelB(panel, x, w);
    }
    else if (id == btnChannelS->GetId())
    {
        CreatePanelChannelS(panel, x, w);
    }
    else if (id == btnScheme->GetId())
    {
        CreatePanelScheme(panel, x, w);
    }
    else if (id == btnCalculate->GetId())
    {
        CreatePanelCalculate(panel, x, w);
    }

    return panel;
}


void PanelConfig::CreatePanelChannelC(wxPanel *panel, int x, int w)
{
    wxStaticBox *boxScan = new wxStaticBox(panel, wxID_ANY, _L("Развёртка"), { x, 0 }, { w, 160 });

    {
        const wxSize size_rb{ 50, 30 };

        rbScan[0] = new RadioButtonICO(boxScan, { 10, SD::Y_SB(20) }, size_rb, "icons/graphs/imp_up.ico");      // IMP_POS  IDC_RADIO_RAZVIMPPOSITIVE   OnRadioRazvimppositive
        rbScan[1] = new RadioButtonICO(boxScan, { 70, SD::Y_SB(20) }, size_rb, "icons/graphs/imp_down.ico");    // IMP_NEG  IDC_RADIO_RAZVIMPNEGATIVE

        rbScan[2] = new RadioButtonICO(boxScan, { 10, SD::Y_SB(50) }, size_rb, "icons/graphs/triang_up.ico");   // DC_POS   IDC_RADIO_RAZVDCPOSITIVE
        rbScan[3] = new RadioButtonICO(boxScan, { 70, SD::Y_SB(50) }, size_rb, "icons/graphs/triang_down.ico"); // DC_NEG   IDC_RADIO_RAZVDCNEGATIVE

        rbScan[4] = new RadioButtonICO(boxScan, { 10, SD::Y_SB(80) }, size_rb, "icons/graphs/sin_hi.ico");      // SYN_POS  IDC_RADIO_SYNPOSITIVE
        rbScan[5] = new RadioButtonICO(boxScan, { 70, SD::Y_SB(80) }, size_rb, "icons/graphs/sin_lo.ico");      // SYN_NEG  IDC_RADIO_SYNNEGATIVE

        rbScan[6] = new RadioButtonICO(boxScan, { 130, SD::Y_SB(20) }, size_rb, "icons/graphs/sin.ico");        // AC       IDC_RADIO_RAZVAC

        {
            RadioButtonICO *object = rbScan[0];
            wxCommandEvent evt(wxEVT_RADIOBUTTON, object->GetId());
            evt.SetInt(1);
            evt.SetEventObject(object);
            boxScan->ProcessWindowEvent(evt);
        }

        {
            wxStaticBox *boxImpulse = new wxStaticBox(boxScan, wxID_ANY, _L("Импульс"), { 10, SD::Y_SB(110) }, { 75, 40 });

            new wxStaticText(boxImpulse, wxID_ANY, "0.2 ms", { 10, SD::Y_SB(20) });
        }

        new wxCheckBox(boxScan, wxID_ANY, _L("Скважн. x 2"), { 100, SD::Y_SB(120) }, { 100, 20 });
    }

    wxStaticBox *boxMeter = new wxStaticBox(panel, wxID_ANY, _L("Измеритель"), { x, boxScan->GetSize().y + x }, { w, 50 });

    {
        int y = 20;
        int dY = 3;

        new wxStaticText(boxMeter, wxID_ANY, "Uc", { 10, SD::Y_SB(y + dY) });
        new wxStaticText(boxMeter, wxID_ANY, "Ic", { 100, SD::Y_SB(y + dY) });

        wxArrayString choices;
        choices.Add("100V");

        new wxComboBox(boxMeter, wxID_ANY, choices[0], { 30, SD::Y_SB(y) }, { 60, TEXTCNTRL_HEIGHT }, choices, wxCB_READONLY);

        choices.Clear();
        choices.Add("10A");

        new wxComboBox(boxMeter, wxID_ANY, choices[0], { 120, SD::Y_SB(y) }, { 60, TEXTCNTRL_HEIGHT }, choices, wxCB_READONLY);
    }

    wxStaticBox *boxSource = new wxStaticBox(panel, wxID_ANY, _L("Источник") + " U", {x, boxMeter->GetPosition().y + boxMeter->GetSize().y + x}, {w, 200});

    {
        new wxStaticText(boxSource, wxID_ANY, _L("Диапазон Uc"), { 10, SD::Y_SB(30) });

        wxArrayString choices;
        choices.Add("20V");

        new wxComboBox(boxSource, wxID_ANY, choices[0], { 100, SD::Y_SB(27) }, { 60, TEXTCNTRL_HEIGHT }, choices, wxCB_READONLY);

        new wxStaticText(boxSource, wxID_ANY, _L("Ограничение Uc, %%"), { 40, SD::Y_SB(55) });

        int y = 80;
        int dy = 3;

        new wxStaticText(boxSource, wxID_ANY, _L("Старт"), { 10, SD::Y_SB(y + dy) });
        new wxStaticText(boxSource, wxID_ANY, _L("Стоп"), { 10, SD::Y_SB(y + 30 + dy) });

        new wxSpinCtrl(boxSource, wxID_ANY, "0", { 80, SD::Y_SB(y) }, { 100, TEXTCNTRL_HEIGHT });
        new SpinBox(boxSource, wxID_ANY, "100", { 80, SD::Y_SB(y + 30) }, { 100, TEXTCNTRL_HEIGHT });
    }
}


void PanelConfig::CreatePanelChannelB(wxPanel *panel, int x, int w)
{
    wxStaticBox *boxGenerator = new wxStaticBox(panel, wxID_ANY, _L("Генератор ступенек"), { x, 0 }, { w, 300 });

    {
        int y = 25;
        new wxStaticText(boxGenerator, wxID_ANY, _L("Тип"), { 10, SD::Y_SB(y) });

        wxArrayString choices;
        choices.Add("U");
        choices.Add("I");

        new wxComboBox(boxGenerator, wxID_ANY, choices[0], { 70, SD::Y_SB(y - 3) }, { 60, TEXTCNTRL_HEIGHT }, choices, wxCB_READONLY);

        y += 25;

        new wxStaticText(boxGenerator, wxID_ANY, _L("Импульс"), { 10, SD::Y_SB(y) });

        choices.Clear();
        choices.Add(_L("Вкл"));
        choices.Add(_L("Выкл"));

        new wxComboBox(boxGenerator, wxID_ANY, choices[0], { 70, SD::Y_SB(y - 3) }, { 60, TEXTCNTRL_HEIGHT }, choices, wxCB_READONLY);

        y += 25;

        choices.Clear();
        choices.Add("2V");

        new wxStaticText(boxGenerator, wxID_ANY, _L("Амплитуда ступени"), { 10, SD::Y_SB(y) });

        new wxComboBox(boxGenerator, wxID_ANY, choices[0], { 130, SD::Y_SB(y - 3) }, { 60, TEXTCNTRL_HEIGHT }, choices, wxCB_READONLY);

        y += 25;

        new wxCheckBox(boxGenerator, wxID_ANY, "x 0.1", { 10, SD::Y_SB(y) }, { 60, TEXTCNTRL_HEIGHT });

        y += 25;

        new wxStaticText(boxGenerator, wxID_ANY, _L("Число ступенек"), { 10, SD::Y_SB(y + 3) });

        new SpinBox(boxGenerator, wxID_ANY, "5", { 120, SD::Y_SB(y) }, { 50, TEXTCNTRL_HEIGHT });

        y += 25;

        choices.Clear();
        choices.Add(_L("прямая"));
        choices.Add(_L("обратная"));

        new wxStaticText(boxGenerator, wxID_ANY, _L("Полярность"), { 10, SD::Y_SB(y + 3) });

        new wxComboBox(boxGenerator, wxID_ANY, choices[0], { 100, SD::Y_SB(y) }, { 80, TEXTCNTRL_HEIGHT }, choices, wxCB_READONLY);

        y += 25;

        wxStaticBox *boxOffset = new wxStaticBox(boxGenerator, wxID_ANY, _L("Смещение"), { x, y }, { w - 10, 100 });

        {
            y = 20;

            new wxSlider(boxOffset, wxID_ANY, 5, 0, 20, { 10, SD::Y_SB(y) }, { 120, TEXTCNTRL_HEIGHT });

            y += 25;

            choices.Clear();
            choices.Add(_L("прямая"));
            choices.Add(_L("обратная"));

            new wxStaticText(boxOffset, wxID_ANY, _L("Полярность"), { 10, SD::Y_SB(y + 3) });

            new wxComboBox(boxOffset, wxID_ANY, choices[0], { 100, SD::Y_SB(y) }, { 80, TEXTCNTRL_HEIGHT }, choices, wxCB_READONLY);
        }
    }

    wxStaticBox *boxLimitation = new wxStaticBox(panel, wxID_ANY, _L("Ограничение"), { x, boxGenerator->GetSize().y + x }, { w, 100 });

    {
        int y = 20;

        new wxStaticText(boxLimitation, wxID_ANY, _L("Диапазон"), { 10, SD::Y_SB(y) });

        wxArrayString choices;
        choices.Add("10 mA");

        new wxComboBox(boxLimitation, wxID_ANY, choices[0], { 100, SD::Y_SB(y - 3) }, { 80, TEXTCNTRL_HEIGHT }, choices, wxCB_READONLY);

        y += 25;

        new wxSlider(boxLimitation, wxID_ANY, 5, 0, 20, { 10, SD::Y_SB(y) }, { 120, TEXTCNTRL_HEIGHT });
    }
}


void PanelConfig::CreatePanelChannelS(wxPanel *panel, int x, int w)
{
    wxStaticBox *boxGenerator = new wxStaticBox(panel, wxID_ANY, _L("Генератор ступенек"), { x, 0 }, { w, 300 });

    {
        int y = 25;
        new wxStaticText(boxGenerator, wxID_ANY, _L("Тип"), { 10, SD::Y_SB(y) });

        wxArrayString choices;
        choices.Add("U");
        choices.Add("I");

        new wxComboBox(boxGenerator, wxID_ANY, choices[0], { 70, SD::Y_SB(y - 3) }, { 60, TEXTCNTRL_HEIGHT }, choices, wxCB_READONLY);

        y += 25;

        new wxStaticText(boxGenerator, wxID_ANY, _L("Импульс"), { 10, SD::Y_SB(y) });

        choices.Clear();
        choices.Add(_L("Вкл"));
        choices.Add(_L("Выкл"));

        new wxComboBox(boxGenerator, wxID_ANY, choices[0], { 70, SD::Y_SB(y - 3) }, { 60, TEXTCNTRL_HEIGHT }, choices, wxCB_READONLY);

        y += 25;

        choices.Clear();
        choices.Add("2V");

        new wxStaticText(boxGenerator, wxID_ANY, _L("Амплитуда ступени"), { 10, SD::Y_SB(y) });

        new wxComboBox(boxGenerator, wxID_ANY, choices[0], { 130, SD::Y_SB(y - 3) }, { 60, TEXTCNTRL_HEIGHT }, choices, wxCB_READONLY);

        y += 25;

        y += 25;

        new wxStaticText(boxGenerator, wxID_ANY, _L("Число ступенек"), { 10, SD::Y_SB(y + 3) });

        new SpinBox(boxGenerator, wxID_ANY, "5", { 120, SD::Y_SB(y) }, { 50, TEXTCNTRL_HEIGHT });

        y += 25;

        choices.Clear();
        choices.Add(_L("прямая"));
        choices.Add(_L("обратная"));

        new wxStaticText(boxGenerator, wxID_ANY, _L("Полярность"), { 10, SD::Y_SB(y + 3) });

        new wxComboBox(boxGenerator, wxID_ANY, choices[0], { 100, SD::Y_SB(y) }, { 80, TEXTCNTRL_HEIGHT }, choices, wxCB_READONLY);

        y += 25;

        wxStaticBox *boxOffset = new wxStaticBox(boxGenerator, wxID_ANY, _L("Смещение"), { x, y }, { w - 10, 100 });

        {
            y = 20;

            new wxSlider(boxOffset, wxID_ANY, 5, 0, 20, { 10, SD::Y_SB(y) }, { 120, TEXTCNTRL_HEIGHT });

            y += 25;

            choices.Clear();
            choices.Add(_L("прямая"));
            choices.Add(_L("обратная"));

            new wxStaticText(boxOffset, wxID_ANY, _L("Полярность"), { 10, SD::Y_SB(y + 3) });

            new wxComboBox(boxOffset, wxID_ANY, choices[0], { 100, SD::Y_SB(y) }, { 80, TEXTCNTRL_HEIGHT }, choices, wxCB_READONLY);
        }
    }

    wxStaticBox *boxLimitation = new wxStaticBox(panel, wxID_ANY, _L("Ограничение"), { x, boxGenerator->GetSize().y + x }, { w, 100 });

    {
        int y = 20;

        new wxStaticText(boxLimitation, wxID_ANY, _L("Диапазон"), { 10, SD::Y_SB(y) });

        wxArrayString choices;
        choices.Add("10 mA");

        new wxComboBox(boxLimitation, wxID_ANY, choices[0], { 100, SD::Y_SB(y - 3) }, { 80, TEXTCNTRL_HEIGHT }, choices, wxCB_READONLY);

        y += 25;

        new wxSlider(boxLimitation, wxID_ANY, 5, 0, 20, { 10, SD::Y_SB(y) }, { 120, TEXTCNTRL_HEIGHT });
    }
}


void PanelConfig::CreatePanelScheme(wxPanel *panel, int x, int /*w*/)
{
    const int width_category = 77;

    wxStaticBox *boxCommutation = new wxStaticBox(panel, wxID_ANY, wxString("                           ") + _L("Коммутация"), { x, 100 }, { MainWindow::WIDTH3 - 10, 300 });

    {
        wxArrayString choices;
        choices.Add(_L("внутренняя"));
        choices.Add(_L("внешняя"));

        // IDC_COMBO_KOMMUTATOR
        new wxComboBox(boxCommutation, wxID_ANY, choices[0], { x + width_category, SD::Y_SB(20) }, { 110, TEXTCNTRL_HEIGHT }, choices, wxCB_READONLY);

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

    wxStaticBox *boxCategory = new wxStaticBox(panel, wxID_ANY, _L("Категория"), { x, 0 }, { width_category, 250 });

    {
        int w = 32;
        int h = 41;
        int x0 = 5;
        int y0 = 20;
        int d = 3;

        wxString tooltips[10] =
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

        for (int row = 0; row < 5; row++)
        {
            for (int col = 0; col < 2; col++)
            {
                int num_category = row * 2 + col;

                bmpCategory[num_category] = new PainterBMP(boxCategory, { x0 + col * (w + d), SD::Y_SB(y0 + row * (h + d)) }, { w, h }, wxString::Format("sch/cat%d.bmp", num_category + 1));

                bmpCategory[num_category]->SetEnabled(false);

                bmpCategory[num_category]->Bind(wxEVT_LEFT_DOWN, &PanelConfig::OnEventCategoryBmpClick, this);

                bmpCategory[num_category]->SetToolTip(tooltips[num_category]);
            }
        }

        bmpCategory[0]->SetEnabled(true);
    }

    (void)boxCategory;

    wxStaticBox *boxTest = new wxStaticBox(panel, wxID_ANY, _L("Тест"), { x + width_category + 5, 0 }, { MainWindow::WIDTH3 - width_category - 15, 100 });

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


void PanelConfig::CreatePanelCalculate(wxPanel *panel, int x, int w)
{
    wxStaticBox *boxGraph = new wxStaticBox(panel, wxID_ANY, _L("График"), { x, 0 }, { w, 75 });

    int y = 20;
    const int dy = 3;

    {
        new wxStaticText(boxGraph, wxID_ANY, _L("Ось X"), { 10, SD::Y_SB(y + dy) });

        new wxTextCtrl(boxGraph, wxID_ANY, "Ud", { 100, SD::Y_SB(y) }, { 50, TEXTCNTRL_HEIGHT });

        y += 25;

        new wxStaticText(boxGraph, wxID_ANY, _L("Ось Y"), { 10, SD::Y_SB(y + dy) });

        new wxTextCtrl(boxGraph, wxID_ANY, "Id", { 100, SD::Y_SB(y) }, { 50, TEXTCNTRL_HEIGHT });
    }

    wxStaticBox *boxParameter = new wxStaticBox(panel, wxID_ANY, _L("Параметр"), { x, boxGraph->GetSize().y + x }, { w, 200 });

    {
        y = 20;

        new wxStaticText(boxParameter, wxID_ANY, _L("Найти"), { 10, SD::Y_SB(y + dy) });
        new wxStaticText(boxParameter, wxID_ANY, _L("при"), { 100, SD::Y_SB(y + dy) });

        new wxTextCtrl(boxParameter, wxID_ANY, "Id", { 50, SD::Y_SB(y) }, { 40, TEXTCNTRL_HEIGHT });
        new wxTextCtrl(boxParameter, wxID_ANY, "Ud", { 140, SD::Y_SB(y) }, { 40, TEXTCNTRL_HEIGHT });

        y += 30;

        new wxStaticText(boxParameter, wxID_ANY, _L("равном"), { 10, SD::Y_SB(y) });

        y += 20;

        new wxStaticText(boxParameter, wxID_ANY, _L("значению") + " 1", { 10, SD::Y_SB(y + dy) });
        new wxTextCtrl(boxParameter, wxID_ANY, "6", { 100, SD::Y_SB(y) }, { 80, TEXTCNTRL_HEIGHT });

        y += 25;

        new wxStaticText(boxParameter, wxID_ANY, _L("значению") + " 2", { 10, SD::Y_SB(y + dy) });
        new wxTextCtrl(boxParameter, wxID_ANY, "6", { 100, SD::Y_SB(y) }, { 80, TEXTCNTRL_HEIGHT });

        y += 25;

        new wxStaticText(boxParameter, wxID_ANY, _L("на ступеньке") + " № 5", { 10, SD::Y_SB(y + dy) });

        wxStaticBox *boxCalculate = new wxStaticBox(boxParameter, wxID_ANY, _L("Рассчитать"), { x, y }, { w - 10, 60 });

        {
            y = 25;

            new wxRadioButton(boxCalculate, wxID_ANY, "dY/dX", { 10, SD::Y_SB(y) });
            new wxRadioButton(boxCalculate, wxID_ANY, "dX/dY", { 100, SD::Y_SB(y) });
        }
    }

    wxStaticBox *boxBorder = new wxStaticBox(panel, wxID_ANY, _L("Пороговые значения"), { x, boxParameter->GetPosition().y + boxParameter->GetSize().y + x }, { w, 80 });

    {
        new wxStaticText(boxBorder, wxID_ANY, "MIN", { 10, SD::Y_SB(y + dy) });
        new wxTextCtrl(boxBorder, wxID_ANY, "3", { 100, SD::Y_SB(y) }, { 80, TEXTCNTRL_HEIGHT });

        y += 25;

        new wxStaticText(boxBorder, wxID_ANY, "MAX", { 10, SD::Y_SB(y + dy) });
        new wxTextCtrl(boxBorder, wxID_ANY, "4", { 100, SD::Y_SB(y) }, { 80, TEXTCNTRL_HEIGHT });
    }
}


void PanelConfig::OnEventButton(wxCommandEvent &event)
{
    int id = event.GetId();

    if (id == btnLoad->GetId())
    {
        WindowLibraryTests().ShowModal();
    }
}


void PanelConfig::OnEventToggleButton(wxCommandEvent &event)
{
    if (!event.IsChecked())
    {
        ((wxToggleButton *)event.GetEventObject())->SetValue(true);
    }
    else
    {
        UnсheckAllAcross(event.GetId());

        EnablePanel(event.GetId());
    }
}


void PanelConfig::OnEventRadioButton(wxCommandEvent &event)
{
    RadioButtonICO *object = ((RadioButtonICO *)event.GetEventObject());

    for (int i = 0; i < 7; i++)
    {
        bool value = (object == rbScan[i]);
        rbScan[i]->SetValue(value);
    }

    if (object == rbScan[0])        // IDC_RADIO_RAZVIMPPOSITIVE OnRadioRazvimppositive()
    {
        /*
        int irazv_old = m_irazv;
        if (n_IPPP == 1 && n_Switch == 2)
        {
            m_irazv = IMP_NEG;
        }
        InitIconImp();
        ChangeDiapazonKol();
        ChangeDiapazonKolMeasU();
        ShowRejimChanel();
        ShowButtonLoopingCompensation();
        if (iX == COLLECTOR && iY == COLLECTOR) ResetDisplay(LEFT);
        int er = ControlParam();
        */
    }
}


void PanelConfig::OnEventComboBox(wxCommandEvent &event)
{
    int id = event.GetId();

    if (id == comboTest->GetId())
    {
        int selection = comboTest->GetSelection();

        btnLoad->Show(comboTest->GetString((uint)selection) == "_USER");
    }
}


void PanelConfig::UnсheckAllAcross(int id)
{
    for (auto &str : str_panels)
    {
        str.button->SetValue(str.button->GetId() == id);
    }
}


void PanelConfig::EnablePanel(int button_id)
{
    for (auto &str : str_panels)
    {
        str.panel->Show(str.button->GetId() == button_id);
    }
}


void PanelConfig::OnEventCategoryBmpClick(wxMouseEvent &event)
{
    wxPanel *clickedPanel = dynamic_cast<wxPanel *>(event.GetEventObject());

    for (int i = 0; i < 10; i++)
    {
        bmpCategory[i]->SetEnabled(clickedPanel == bmpCategory[i]);
    }
}
