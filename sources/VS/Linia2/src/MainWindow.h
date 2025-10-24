// 2023/07/04 17:46:31 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once

#pragma warning(push, 0)
#define WIN32_LEAN_AND_MEAN
#include <wx/wx.h>
#pragma warning(pop)


struct ModeMainWindow
{
    friend class MainWindow;

    enum E
    {
        Standard,           // Стартовый вид окна
        ReferenceGraphs,    // Архив эталонных графиков на весь экран
        FullGraph,          // Графики на весь экран
        Debug,              // Отладочный режим - засылка в регисты
        Count
    };

    static E Current()
    {
        return current;
    }

private:

    static E current;
};


class MainWindow : public wxFrame
{
public:

    static const int WIDTH = 1024;     // Полная ширина - соотвествует разрешению экрана
    static const int HEIGHT = 768;     // Полная высота - соотвествует разрешению экрана

    static const int WIDTH1 = 160;                  // Ширина левой верхней зоны
    static const int WIDTH_DRAW = 820;              // Ширина зоны отрисовки
    static const int WIDTH3 = WIDTH - WIDTH_DRAW;   // Ширина последнего, третьего, столбика с панелями

    static const int HEIGHT1 = 75;
    static const int HEIGHT_DRAW = 660;             // Столько занимает окно отрисовки, когда не развёрнуто на весь экран


    MainWindow(const wxString &title);

    void OnQuit(wxCommandEvent &event);
    void OnAbout(wxCommandEvent &event);

    static MainWindow *self;

    void OnEventCloseWindow(wxCloseEvent &);

    void SetMode(ModeMainWindow::E);

    void Update();

private:

    wxToolBar *toolBar = nullptr;

    void OnEventKeyHook(wxKeyEvent &);

    // Настроить шрифт
    void TuneFont();
};
