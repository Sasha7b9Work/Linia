// 2023/07/04 17:46:31 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once

#pragma warning(push, 0)
#define WIN32_LEAN_AND_MEAN
#include <wx/wx.h>
#pragma warning(pop)


class MainWindow : public wxFrame
{
public:

    static const int WIDTH = 1024;     // Полная ширина - соотвествует разрешению экрана
    static const int HEIGHT = 768;     // Полная высота - соотвествует разрешению экрана

	static const int WIDTH1 = 100;                      // Ширина первого столбика с панелями
    static const int WIDTH2 = 400;                      // Ширина второго столбика с панелями
    static const int WIDTH3 = WIDTH - WIDTH1 - WIDTH2;  // Ширина последнего, третьего, столбика с панелями


    MainWindow(const wxString &title);

    // Вызвать после загрузки настроек
    void Preprocess();

    void OnQuit(wxCommandEvent &event);
    void OnAbout(wxCommandEvent &event);

    static MainWindow *self;

    void OnEventCloseWindow(wxCloseEvent &);

private:

    wxToolBar *toolBar = nullptr;

    void OnEventKeyHook(wxKeyEvent &);

    // Настроить шрифт
    void TuneFont();

    template <class T> void ReadFontParameter(const wxString &, T &);
};
