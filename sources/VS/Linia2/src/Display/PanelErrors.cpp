// 2025/09/01 22:59:47 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/PanelErrors.h"
#include "MainWindow.h"
#include "Display/Display.h"


PanelErrors::PanelErrors(wxWindow *parent) :
    wxPanel(parent, wxID_ANY, wxDefaultPosition, { MainWindow::WIDTH2, 100 })
{
    text_ctrl = new wxTextCtrl(this, wxID_ANY, wxEmptyString, { 0, 0 }, { MainWindow::WIDTH2, 90 }, wxTE_MULTILINE | wxTE_READONLY | wxNO_BORDER);

    text_ctrl->SetBackgroundColour(*wxRED);

    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(text_ctrl, 0, wxEXPAND | wxALL, 0);

    SetSizer(sizer);

    text_ctrl->SetForegroundColour(*wxWHITE);

    text_ctrl->AppendText("ERROR 123 - Мало памяти\n");
    text_ctrl->AppendText("ERROR 177 - Много памяти\n");

    btnCollapse = new wxButton(text_ctrl, wxID_ANY, "Свернуть", wxDefaultPosition, { 90, 22 });

    ReInit();

    Bind(wxEVT_BUTTON, &PanelErrors::OnEventButton, this);
    Bind(wxEVT_TIMER, &PanelErrors::OnEventTimer, this, timer.GetId());

    timer.SetOwner(this, timer.GetId());
}


void PanelErrors::ReInit()
{
    wxSize size = GetSize();

    size.x = Display::self->GetSize().x;

    SetSize(size);                                                  // Корректируем размер под размер дисплея

    wxPoint position{ size.x - btnCollapse->GetSize().x - 20, 0 };

    btnCollapse->SetPosition(position);                             // Корректируем положение кнопки
}


void PanelErrors::OnEventButton(wxCommandEvent &event)
{
    collapse = !collapse;

    SetColors(false);

    if (collapse)
    {
        SetSize({ GetSize().x, 23 });
        btnCollapse->SetLabel("Развернуть");
        timer.Start(500);
    }
    else
    {
        SetSize({ GetSize().x, 100 });
        btnCollapse->SetLabel("Свернуть");
        timer.Stop();
    }

    event.Skip();
}


void PanelErrors::OnEventTimer(wxTimerEvent &)
{
    SetColors(text_ctrl->GetBackgroundColour() == *wxRED);
}


void PanelErrors::SetColors(bool inverse)
{
    text_ctrl->SetBackgroundColour(inverse ? *wxWHITE : *wxRED);

    text_ctrl->SetForegroundColour(inverse ? *wxBLACK : *wxWHITE);

    text_ctrl->Refresh();
}
