// 2025/6/1 17:20:51 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Panels/Panel.h"


class PanelConfig : public Panel
{
public:

    static const int HEIGHT = 500;

    PanelConfig(wxWindow *parent);

    static PanelConfig *self;

private:

    // Структура хранит кнопку и панель, которую та включает
    struct StructPanel
    {
        wxToggleButton *button;
        wxPanel *panel;
    };

    // Здесь хранятся элементы панели "Канал C"
    struct ChannelC
    {
        std::vector<wxRadioButton *> rbs;
    };

    std::vector<StructPanel> str_panels;

    wxPanel *CreatePanel(wxToggleButton *);

    void OnEventButton(wxCommandEvent &);
    void OnEventRadioButton(wxCommandEvent &);

    // Отщёлкнуть все, кроме id
    void UnсheckAllAcross(int id);

    void EnablePanel(int button_id);

    void CreatePanelChannelC(wxPanel *, int x, int w);

    void CreatePanelChannelB(wxPanel *, int x, int w);

    void CreatePanelChannelS(wxPanel *, int x, int w);

    void CreatePanelCalculate(wxPanel *, int x, int w);
};
