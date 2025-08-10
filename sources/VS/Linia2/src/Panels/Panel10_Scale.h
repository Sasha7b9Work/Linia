// 2025/6/1 18:17:45 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Panels/Panel.h"
#include "MainWindow.h"


class PanelScale : public Panel
{
public:

    static const int WIDTH = MainWindow::WIDTH3;
    static const int HEIGTH = MainWindow::HEIGHT - PanelConfig::HEIGHT - PanelIndicator::HEIGHT;

    PanelScale(wxWindow *parent);

    static PanelScale *self;

private:

    wxToggleButton *btnScale = nullptr,
        *btnErrors = nullptr;

    wxCheckBox *chb1 = nullptr,
        *chb2 = nullptr,
        *chb3 = nullptr,
        *chb4 = nullptr,
        *chb5 = nullptr;

    void OnEventButton(wxCommandEvent &);

    // Отщёлкнуть все, кроме id
    void UncheckAllAcross(int id);

    void EnablePanel(int button_id);

    // Структура хранит кнопку и панель, которую та включает
    struct StructPanel
    {
        wxToggleButton *button;
        wxPanel *panel;
    };

    std::vector<StructPanel> str_panels;

    wxPanel *CreatePanel(wxToggleButton *);

    void CreatePanelScale(wxPanel *, int x, int w);

    void CreatePanelErrors(wxPanel *, int x, int w);
};
