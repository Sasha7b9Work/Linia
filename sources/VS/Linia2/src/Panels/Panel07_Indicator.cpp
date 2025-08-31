// 2025/6/1 18:25:37 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel03_Config/Panel03_Config.h"
#include "Panels/Panel07_Indicator.h"
#include "MainWindow.h"
#include "Utils/SystemDepend.h"
#include "Controls/Bitmap.h"
#include "DeviceTest/DeviceTest.h"


PanelIndicator *PanelIndicator::self = nullptr;


PanelIndicator::PanelIndicator(wxWindow* parent) :
    Panel(parent, MainWindow::WIDTH1 + MainWindow::WIDTH2, PanelConfig::HEIGHT,
        MainWindow::WIDTH3, HEIGHT)
{
    SetName("PanelIndicator");

    self = this;

    wxArrayString images =
    {
        "buttons/start_off.bmp",
        "buttons/start_on.bmp"
    };

    button_start = new ButtonBitmapChoice(this, {0, 0}, {64 - 10, 60 - 10}, images, "button_start");

    wxStaticText *text = new wxStaticText(this, wxID_ANY, "Uc 100 %", { 105, 5 }, { 25, 25});

    text->SetForegroundColour(wxColour(255, 0, 0));

    wxFont font = text->GetFont();
    font.SetPointSize(font.GetPointSize() + 7);
    font.SetWeight(wxFONTWEIGHT_BOLD);
    text->SetFont(font);

    button_start->Bind(EVT_BUTTON_BITMAP_CHOICE, &PanelIndicator::OnEventButton, this);
}


void PanelIndicator::OnEventButton(ButtonBitmapChoiceEvent &event)
{
    int id = event.GetId();

    if (id == button_start->GetId())
    {
        if (event.GetInt() == 0)
        {
            devicet->Stop();
        }
        else if (event.GetInt() == 1)
        {
            devicet->Start();
        }
    }

    event.Skip();
}
