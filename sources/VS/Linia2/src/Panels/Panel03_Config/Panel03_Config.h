// 2025/6/1 17:20:51 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Panels/Panel.h"
#include "Controls/PainterBMP.h"
#include "Controls/RadioButtonICO.h"
#include "Controls/ButtonsCombo.h"


class PanelConfig : public Panel
{
public:

    static const int HEIGHT = 500;

    PanelConfig(wxWindow *parent);

    static PanelConfig *self;

private:

    wxToggleButton *btnScheme = nullptr,
        *btnChannelC = nullptr,
        *btnChannelB = nullptr,
        *btnChannelS = nullptr,
        *btnCalculate = nullptr;

    // Структура хранит кнопку и панель, которую та включает
    struct StructPanel
    {
        wxToggleButton *button;
        wxPanel        *panel;
    };

    // Здесь хранятся элементы панели "Канал C"
    struct ChannelC
    {
        std::vector<wxRadioButton *> rbs;
    };

    std::vector<StructPanel> str_panels;

    wxPanel *CreatePanel(wxToggleButton *);

    void OnEventToggleButton(wxCommandEvent &);

    // Отщёлкнуть все, кроме id
    void UnсheckAllAcross(int id);

    void EnablePanel(int button_id);
};
