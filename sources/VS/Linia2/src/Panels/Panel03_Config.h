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

    std::vector<wxToggleButton *> tab_buttons;

    wxPanel *CreatePanel(wxAuiNotebook *);

    void OnEventButton(wxCommandEvent &);

    // Отщёлкнуть все, кроме id
    void UnсheckAllAcross(int id);
};
