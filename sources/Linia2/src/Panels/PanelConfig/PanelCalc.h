// 2025/6/1 17:41:14 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Panels/Panel.h"
#include "MainWindow.h"
#include "Panels/PanelMenu/PanelMenu.h"


class PanelCalc : public wxPanel
{
public:

    static const int HEIGHT = MainWindow::HEIGHT - MainWindow::HEIGHT_HI - PanelMenu::HEIGHT;

    PanelCalc(wxWindow *parent, int x, int w, int h);

    static PanelCalc *self;

    virtual void Update() override;

private:

    wxPanel *panel_graph = nullptr;
    wxStaticText *txtName = nullptr;

    void SetName(const wxString &);
};
