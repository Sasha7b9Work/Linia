// 2026/10/06 16:24:13 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "GUI/Controls/SliderHidden.h"
#include "GUI/Controls/Slider.h"
#include "GUI/Controls/Sizers.h"
#pragma warning(push, 0)
    #include <wx/popupwin.h>
#pragma warning(pop)


class WindowSlider : public wxPopupTransientWindow
{
public:
    WindowSlider(SliderHidden *parent) : wxPopupTransientWindow(parent)
    {
        wxPopupTransientWindow::Hide();

        wxPanel *mainPanel = new wxPanel(this);

        wxGridSizer *gridSizer = new wxGridSizer(1, 1, 2, 2);

        // Добавляем рамку вокруг сетки клеток
        StaticBoxSizer *boxSizer = new StaticBoxSizer(wxVERTICAL, mainPanel, L("Смещение"));
        boxSizer->Add(gridSizer, 1, wxEXPAND | wxALL, 0);

        SliderInt *slider = new SliderInt(mainPanel, 200, 0, 100, L("Смещение"));

        gridSizer->Add(slider, 0, wxEXPAND | wxALL, 2);

        mainPanel->SetSizer(boxSizer);

        wxBoxSizer *outerSizer = new wxBoxSizer(wxVERTICAL);
        // Внешние отступы 15px
        outerSizer->Add(mainPanel, 1, wxEXPAND | wxALL, 3);
        SetSizer(outerSizer);

        wxPopupTransientWindow::Layout();

        wxPopupTransientWindow::Fit();

        wxPopupTransientWindow::Refresh();
        wxPopupTransientWindow::Update();

        wxPopupTransientWindow::SetBackgroundColour(GetBackgroundColour().ChangeLightness(50));

        wxPopupTransientWindow::SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY | wxWS_EX_PROCESS_UI_UPDATES);

        wxPopupTransientWindow::Show();
    }
private:
};


SliderHidden::SliderHidden(wxWindow *parent, int width, const wxString &title, const wxString &name_file) :
    DrawingButton(parent, title, { width, ButtonsCombo::HEIGHT }, name_file)
{
    Bind(wxEVT_BUTTON, [this](wxCommandEvent &)
        {
            WindowSlider *wndSlider = new WindowSlider(this);

            wxPoint pos = ClientToScreen(wxPoint(GetSize().x / 2, GetSize().y / 2));

            wxSize size = wndSlider->GetSize();

            pos.x -= size.x / 2;
            pos.y -= size.y / 2;

            wndSlider->Position(pos, wxSize(0, 0));
            wndSlider->Popup();
            wndSlider->Refresh();
            wndSlider->Update();;
            wndSlider->SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
        });
}


void SliderHidden::SetRange(double _min, double _max)
{
    min = _min;
    max = _max;
}
