// 2023/07/04 17:46:31 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Application.h"
#include "Utils/Configurator.h"
#include "Settings/Settings.h"
#include "Utils/Configurator.h"
#include "Windows/ConsoleRS232.h"
#include "MainWindow.h"
#include "Panels/PanelName.h"
#include "Panels/PanelModeUpper.h"
#include "Panels/PanelConfig/PanelConfig.h"
#include "Panels/PanelModel.h"
#include "Display/Display.h"
#include "Panels/PanelIndicator.h"
#include "Panels/PanelModeLower.h"
#include "Panels/PanelTable.h"
#include "Panels/PanelMenu/PanelMenu.h"
#include "Panels/PanelDebug/PanelDebug.h"
#include "Panels/PanelReferenceGraphs.h"
#include "Controls/Dialog.h"
#include "IPPP/Tests/Tests.h"


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
    |         |                                 +------------------|
    |         |                                 |        7         |
    |         |                                 +------------------+
    |         +---------------------------------+                  |
    |         |             9                   |        8         |
    +---------+---------------------------------+                  |
    |               11                          |                  |
    +-------------------------------------------+------------------+

    1. Название ПО и дата сборки
    2. Текущий режим работы
    3. Текущая аппаратная конфигурация - каналы, схема включения, расчёт
    4. Файл модели
    5. Графики
    7. Индикация выходного напряжения
    8. Режим
    9. Таблица точек
    11. Кнопки
*/


MainWindow *MainWindow::self = nullptr;
ModeMainWindow::E ModeMainWindow::current = ModeMainWindow::Standard;


MainWindow::MainWindow(const wxString &title)
    : wxFrame((wxFrame *)NULL, wxID_ANY, title, wxDefaultPosition, wxDefaultSize, wxNO_BORDER | wxSTAY_ON_TOP)
{
    self = this;

#ifdef WIN32
    SetIcon(wxICON(MAIN_ICON));
#endif

    Bind(wxEVT_MENU, &MainWindow::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &MainWindow::OnQuit, this, wxID_EXIT);

    Bind(wxEVT_CLOSE_WINDOW, &MainWindow::OnEventCloseWindow, this);
    TuneFont();

    wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);

    SetSizer(sizer);

    wxWindow::SetPosition(SET::GUI::position.Get());



    Bind(wxEVT_CHAR_HOOK, &MainWindow::OnEventKeyHook, this);

    {
        const wxSize size(WIDTH, HEIGHT);

        if (GF::IsBoardPCM())
        {
            SetSize(size);

            SetSize(wxGetDisplaySize());

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

    new Display(this);

    new PanelIndicator(this);

    new PanelModeLower(this);

    new PanelTable(this);

    new PanelButtons(this);

    new PanelDebug(this);

    new PanelReferenceGraph(this);

    SetMode(SET::GUI::debug_mode.Get() ? ModeMainWindow::Debug : ModeMainWindow::Standard);
}


void MainWindow::Update()
{
    PanelDebug::self->Update();
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

    Config::ReadFontParameter("font_family", family);

    font.SetFamily(family);

    //------------------------------------------------------------

    wxString face_name = "Segoe UI";

    Config::ReadString("font_face_name", face_name);

    font.SetFaceName(face_name);

    //------------------------------------------------------------

    wxFontStyle style = wxFONTSTYLE_NORMAL;

    Config::ReadFontParameter("font_style", style);

    font.SetStyle(style);

    //------------------------------------------------------------

    int point_size = 9;

    Config::ReadInt("font_point_size", point_size);

    font.SetPointSize(point_size);

    //------------------------------------------------------------

    wxPoint pixel_size{ font.GetPixelSize().x, font.GetPixelSize().y };

    Config::Read("font_pixel_size_x", pixel_size.x);
    Config::Read("font_pixel_size_y", pixel_size.y);

    font.SetPixelSize({ pixel_size.x, pixel_size.y });

    //------------------------------------------------------------

    font.SetUnderlined(false);

    font.SetStrikethrough(false);

    //------------------------------------------------------------

    wxFontWeight weigth = wxFONTWEIGHT_NORMAL;

    Config::ReadFontParameter("font_weigth", weigth);

    font.SetWeight(weigth);

    //------------------------------------------------------------

    wxWindow::SetFont(font);
}


void MainWindow::OnQuit(wxCommandEvent &WXUNUSED(event))
{
    LOG_WRITE("Frame::OnQuit()");

    self = nullptr;

    Close(true);
}


void MainWindow::OnEventCloseWindow(wxCloseEvent &event)
{
    PanelDebug::self->Show(false);

    Test::Save("example.tst");

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
    Dialog dlg(this, wxID_ANY, wxString(_("About")));

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
    ModeMainWindow::current = mode;

    PanelName::self->Show(mode == ModeMainWindow::Standard);
    PanelModeUpper::self->Show(mode == ModeMainWindow::Standard);
    PanelConfig::self->Show(mode == ModeMainWindow::Standard);
    PanelModel::self->Show(mode == ModeMainWindow::Standard);
    Display::self->Show(mode == ModeMainWindow::Standard || mode == ModeMainWindow::FullGraph);
    PanelIndicator::self->Show(mode == ModeMainWindow::Standard);
    PanelModeLower::self->Show(mode == ModeMainWindow::Standard);
    PanelTable::self->Show(mode == ModeMainWindow::Standard);
    PanelButtons::self->Show(mode == ModeMainWindow::Standard);

    PanelDebug::self->Show(mode == ModeMainWindow::Debug);

    PanelReferenceGraph::self->Show(mode == ModeMainWindow::ReferenceGraphs);

    Display::self->FullScreen(mode == ModeMainWindow::FullGraph);

    wxFrame::Layout();

    PanelDebug::self->Pack();
}
