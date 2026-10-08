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

        {
            SliderFloat *slider = new SliderFloat(mainPanel, 200, L("Смещение"));
            slider->SetRange(parent->min, parent->max, "A", 1);
            slider->SetBackgroundColour(slider->GetBackgroundColour().ChangeLightness(LIGHTNESS));
            slider->SetBackgroundColour(slider->GetBackgroundColour().ChangeLightness(170));

            gridSizer->Add(slider, 0, wxEXPAND | wxALL, 2);
        }

        mainPanel->SetSizer(boxSizer);

        wxBoxSizer *outerSizer = new wxBoxSizer(wxVERTICAL);
        // Внешние отступы
        outerSizer->Add(mainPanel, 1, wxEXPAND | wxALL, 3);
        SetSizer(outerSizer);

        wxPopupTransientWindow::Layout();

        wxPopupTransientWindow::Fit();

        GetParent()->Bind(wxEVT_KEY_DOWN, [this](wxKeyEvent &event)
            {
                if (event.GetKeyCode() != WXK_SPACE)
                {
                    Dismiss();
                }

                event.Skip();
            });

        wxPopupTransientWindow::Refresh();
        wxPopupTransientWindow::Update();

        wxPopupTransientWindow::SetBackgroundColour(GetBackgroundColour().ChangeLightness(50));

        // Отключаем изменение фона для всех детей
        for (auto child : GetChildren())
        {
            child->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE));
            child->SetBackgroundStyle(wxBG_STYLE_ERASE);
            child->Refresh(); // Обновляем внешний вид
        }

        wxPopupTransientWindow::SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY | wxWS_EX_PROCESS_UI_UPDATES);

        // Чтобы окно закрывалось при потере фокуса
        Bind(wxEVT_KILL_FOCUS, [this](wxFocusEvent &event)
            {
                Dismiss();
                event.Skip();
            });

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
