// 2025/6/1 17:41:14 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Panels/Panel.h"
#include "MainWindow.h"
#include "Panels/Panel11_Menu/Panel11_Menu.h"


class PanelModel : public Panel
{
public:

    static const int WIDTH = MainWindow::WIDTH1;
    static const int HEIGHT = MainWindow::HEIGHT - MainWindow::HEIGHT1 - PanelButtons::HEIGHT;

    PanelModel(wxWindow *parent);

    static PanelModel *self;

    virtual void Update() override;

private:

    wxPanel *panel_graph = nullptr;
    wxStaticText *txtName = nullptr;

    void SetName(const wxString &);
};
