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

    struct StructPanel
    {
        wxToggleButton *button;
        wxPanel *panel;
    };

    std::vector<StructPanel> str_panels;

    wxPanel *CreatePanel(wxToggleButton *);

    void OnEventButton(wxCommandEvent &);

    // Отщёлкнуть все, кроме id
    void UnсheckAllAcross(int id);

    void EnablePanel(int button_id);
};
