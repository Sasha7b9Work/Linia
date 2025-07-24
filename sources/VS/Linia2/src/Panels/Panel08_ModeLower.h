// 2025/6/1 18:01:50 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Panels/Panel.h"
#include "MainWindow.h"
#include "Panels/Panel11_Menu/Panel11_Menu.h"


class PanelModeLower : public Panel
{
public:

    static const int WIDTH = MainWindow::WIDTH1;
    static const int HEIGHT = MainWindow::HEIGHT - MainWindow::HEIGTH1 - PanelModel::HEIGHT - PanelPercents::HEIGHT - PanelButtons::HEIGHT;

    PanelModeLower(wxWindow *parent);

    static PanelModeLower *self;
};
