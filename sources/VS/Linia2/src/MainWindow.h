// 2023/07/04 17:46:31 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once

#pragma warning(push, 0)
#define WIN32_LEAN_AND_MEAN
#include <wx/wx.h>
#pragma warning(pop)


class Notebook;
class CommonPanel;


class MainWindow : public wxFrame
{
public:

    static const int FULL_WIDTH = 1024;     // Полная ширина - соотвествует разрешению экрана
    static const int FULL_HEIGHT = 768;     // Полная высота - соотвествует разрешению экрана

    MainWindow(const wxString &title);

    // Вызвать после загрузки настроек
    void Preprocess();

    void OnQuit(wxCommandEvent &event);
    void OnAbout(wxCommandEvent &event);

    static MainWindow *self;

    void OnEventCloseWindow(wxCloseEvent &);

private:

    wxToolBar *toolBar = nullptr;
    Notebook *notebook = nullptr;                 // There will be pages for working with files

    CommonPanel *panel = nullptr;

    void OnEventKeyHook(wxKeyEvent &);

    // Настроить шрифт
    void TuneFont();

    template <class T> void ReadFontParameter(const wxString &, T &);
};
