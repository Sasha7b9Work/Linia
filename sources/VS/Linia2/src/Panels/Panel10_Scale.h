// 2025/6/1 18:17:45 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Panels/Panel.h"


class PanelScale : public Panel
{
public:

    PanelScale(wxWindow *parent);

    static PanelScale *self;

private:

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
};
