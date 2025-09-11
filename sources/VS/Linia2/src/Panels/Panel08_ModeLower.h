// 2025/6/1 18:01:50 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Panels/Panel.h"
#include "MainWindow.h"
#include "Panels/Panel11_Menu/Panel11_Menu.h"
#include "Controls/ButtonsCombo.h"
#include "Panels/PanelConfig/PanelConfig.h"
#include "Panels/Panel07_Indicator.h"


class PanelModeLower : public Panel
{
public:

    static const int WIDTH = MainWindow::WIDTH3;
    static const int HEIGHT = MainWindow::HEIGHT - PanelConfig::HEIGHT - PanelIndicator::HEIGHT;

    PanelModeLower(wxWindow *parent);

    static PanelModeLower *self;
};
