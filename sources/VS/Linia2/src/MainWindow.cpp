// 2023/07/04 17:46:31 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Application.h"
#include "Utils/Configurator.h"
#include "Settings/Settings.h"
#include "Utils/Configurator.h"
#include "Windows/ConsoleRS232.h"
#include "MainWindow.h"
#include "Panels/Panel01_Name.h"
#include "Panels/Panel02_ModeUpper.h"
#include "Panels/Panel03_Config.h"
#include "Panels/Panel04_Model.h"
#include "Panels/Panel05_Graph/Panel05_Graph.h"
#include "Panels/Panel06_Percents.h"
#include "Panels/Panel07_Indicator.h"
#include "Panels/Panel08_ModeLower.h"
#include "Panels/Panel09_Table.h"
#include "Panels/Panel10_Scale.h"
#include "Panels/Panel11_Menu/Panel11_Menu.h"
#include "Panels/Panel12_Debug/Panel12_Debug.h"


/*
    ----------------------------------------------------------------
    |    1    |            2                    |         3        |
    |         |                                 |                  |
    +---------+---------------------------------+                  |
    |         |                                 |                  |
    |         |                                 |                  |
    |         |                                 |                  |
    |    4    |            5                    |                  |
    |         |                                 |                  |
    |         |                                 |                  |
    +---------+                                 +------------------|
    |    6    |                                 |        7         |
    +---------+                                 +------------------+
    |         +---------------------------------+                  |
    |    8    |             9                   |        10        |
    +---------+---------------------------------+                  |
    |               11                          |                  |
    +-------------------------------------------+------------------+

    1. Название ПО и дата сборки
    2. Текущий режим работы
    3. Текущая аппаратная конфигурация - каналы, схема включения, расчёт
    4. Файл модели
    5. Графики
    6. Дискета с процентами
    7. Индикация выходного напряжения
    8. Режим
    9. Таблица точек
    10. Шкала, ошибки
    11. Кнопки
*/


MainWindow *MainWindow::self = nullptr;


MainWindow::MainWindow(const wxString &title)
    : wxFrame((wxFrame *)NULL, wxID_ANY, title, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE & ~wxMAXIMIZE_BOX)
{
    self = this;

#ifdef WIN32
    SetIcon(wxICON(MAIN_ICON));
#endif

    Bind(wxEVT_MENU, &MainWindow::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &MainWindow::OnQuit, this, wxID_EXIT);

    Bind(wxEVT_CLOSE_WINDOW, &MainWindow::OnEventCloseWindow, this);
    TuneFont();

    SET::Init();

    wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);

    SetSizer(sizer);

    wxWindow::SetPosition(SET::GUI::position.Get());

    Bind(wxEVT_CHAR_HOOK, &MainWindow::OnEventKeyHook, this);

    {
        const wxSize size(WIDTH, HEIGHT);

        if (GF::IsBoardPCM())
        {
            SetSize(size);

            ShowFullScreen(true);
        }
        else
        {
            SetClientSize(size);

            SetWindowStyle(GetWindowStyle() & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX));
        }
    }

    new PanelName(this);

    new PanelModeUpper(this);

    new PanelConfig(this);

    new PanelModel(this);

    new PanelGraph(this);

    new PanelPercents(this);

    new PanelIndicator(this);

    new PanelModeLower(this);

    new PanelTable(this);

    new PanelScale(this);

    new PanelButtons(this);

    new PanelDebug(this);

    SetMode(SET::GUI::debug_mode.Get() ? ModeMainWindow::Debug : ModeMainWindow::Standard);
}


void MainWindow::OnEventKeyHook(wxKeyEvent &event)
{
    if (event.GetKeyCode() == 'K')
    {
        if (event.ControlDown())
        {
            ConsoleRS232::self->Show(!ConsoleRS232::self->IsShown());
        }
    }

    event.Skip();
}


void MainWindow::TuneFont()
{
    wxFont font = GetFont();

    //------------------------------------------------------------

    wxFontFamily family = wxFONTFAMILY_DEFAULT;

    ReadFontParameter("font_family", (int &)family);

    font.SetFamily(family);

    //------------------------------------------------------------

    wxString face_name = "Segoe UI";

    ReadFontParameter("font_face_name", face_name);

    font.SetFaceName(face_name);

    //------------------------------------------------------------

    wxFontStyle style = wxFONTSTYLE_NORMAL;

    ReadFontParameter("font_style", (int &)style);

    font.SetStyle(style);

    //------------------------------------------------------------

    int point_size = 9;

    ReadFontParameter("font_point_size", point_size);

    font.SetPointSize(point_size);

    //------------------------------------------------------------

    wxSize pixel_size = font.GetPixelSize();

    ReadFontParameter("font_pixel_size_x", pixel_size.x);
    ReadFontParameter("font_pixel_size_y", pixel_size.y);

    font.SetPixelSize(pixel_size);

    //------------------------------------------------------------

    font.SetUnderlined(false);

    font.SetStrikethrough(false);

    //------------------------------------------------------------

    wxFontWeight weigth = wxFONTWEIGHT_NORMAL;

    ReadFontParameter("font_weigth", (int &)weigth);

    font.SetWeight(weigth);

    //------------------------------------------------------------

    wxWindow::SetFont(font);
}


template <class T>
void MainWindow::ReadFontParameter(const wxString &key, T &parameter)
{
    if (g_config->Exists(key))
    {
        g_config->Read(key, &parameter);
    }
    else
    {
        g_config->Write(key, parameter);
    }
}


void MainWindow::OnQuit(wxCommandEvent &WXUNUSED(event))
{
    LOG_WRITE("Frame::OnQuit()");

    self = nullptr;

    Close(true);
}


void MainWindow::OnEventCloseWindow(wxCloseEvent &event)
{
    SET::Save("");

    if (ConsoleRS232::self)
    {
        SET::GUI::pos_console.Set(ConsoleRS232::self->GetPosition());

        wxSize size = ConsoleRS232::self->GetSize();
        SET::GUI::size_console.Set({ size.x, size.y });

        SET::GUI::maximized_console.Set(ConsoleRS232::self->IsMaximized());

        ConsoleRS232::self->Destroy();
    }

    SET::GUI::position.Set(wxWindow::GetPosition());

    self = nullptr;

    Application::self->Disable();

    event.Skip();
}


void MainWindow::OnAbout(wxCommandEvent &WXUNUSED(event))
{
    wxBoxSizer *topsizer;
    wxDialog dlg(this, wxID_ANY, wxString(_("About")));

    topsizer = new wxBoxSizer(wxVERTICAL);

#if wxUSE_STATLINE
    topsizer->Add(new wxStaticLine(&dlg, wxID_ANY), 0, wxEXPAND | wxLEFT | wxRIGHT, 10);
#endif // wxUSE_STATLINE

    wxButton *bu1 = new wxButton(&dlg, wxID_OK, _("OK"));
    bu1->SetDefault();

    topsizer->Add(bu1, 0, wxALL | wxALIGN_RIGHT, 15);

    dlg.SetSizer(topsizer);
    topsizer->Fit(&dlg);

    dlg.ShowModal();
}


void MainWindow::SetMode(ModeMainWindow::E mode)
{
    PanelName::self->Show(mode != ModeMainWindow::Debug);
    PanelModeUpper::self->Show(mode != ModeMainWindow::Debug);
    PanelConfig::self->Show(mode != ModeMainWindow::Debug);
    PanelModel::self->Show(mode != ModeMainWindow::Debug);
    PanelGraph::self->Show(mode != ModeMainWindow::Debug);
    PanelPercents::self->Show(mode != ModeMainWindow::Debug);
    PanelIndicator::self->Show(mode != ModeMainWindow::Debug);
    PanelModeLower::self->Show(mode != ModeMainWindow::Debug);
    PanelTable::self->Show(mode != ModeMainWindow::Debug);
    PanelScale::self->Show(mode != ModeMainWindow::Debug);
    PanelButtons::self->Show(mode != ModeMainWindow::Debug);

    PanelDebug::self->Show(mode == ModeMainWindow::Debug);

    wxFrame::Layout();
}
