// 2026/03/13 16:31:04 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class MenuDisplay : public wxMenu
{
public:

    MenuDisplay();

private:

    struct MenuItemColor
    {
        wxMenuItem *item;
    };

    // Следить
    void AppendMenuTrack();

    // Внешний вид
    void AppendMenuFacade();

    void OnReset(wxCommandEvent &);
    void OnFullScreen(wxCommandEvent &);
    void OnTrackX(wxCommandEvent &);
    void OnTrackY(wxCommandEvent &);
    void OnTrackNone(wxCommandEvent &);
    void OnColor(wxCommandEvent &);
    void OnSizePoint(wxCommandEvent &);
    void OnSaveGraphic(wxCommandEvent &);
    void OnResetGraphic(wxCommandEvent &);
};
