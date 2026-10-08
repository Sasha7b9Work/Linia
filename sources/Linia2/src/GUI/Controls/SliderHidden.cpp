// 2026/10/06 16:24:13 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "GUI/Controls/SliderHidden.h"
#include "GUI/Controls/Slider.h"
#include "GUI/Controls/Sizers.h"
#include "GUI/Controls/Windows.h"


class WindowSlider : public PopupTransientWindow
{
public:
    WindowSlider(SliderHidden *parent) : PopupTransientWindow(parent)
    {
        PopupTransientWindow::Hide();

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

        PopupTransientWindow::Layout();

        PopupTransientWindow::Fit();

        GetParent()->Bind(wxEVT_KEY_DOWN, [this](wxKeyEvent &event)
            {
                if (event.GetKeyCode() != WXK_SPACE)
                {
                    Dismiss();
                }

                event.Skip();
            });

        PopupTransientWindow::Refresh();
        PopupTransientWindow::Update();

        PopupTransientWindow::SetBackgroundColour(GetBackgroundColour().ChangeLightness(50));

        // Отключаем изменение фона для всех детей
        for (auto child : GetChildren())
        {
            child->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE));
            child->SetBackgroundStyle(wxBG_STYLE_ERASE);
            child->Refresh(); // Обновляем внешний вид
        }

        PopupTransientWindow::SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY | wxWS_EX_PROCESS_UI_UPDATES);

        // Чтобы окно закрывалось деактивации окна
        Bind(wxEVT_ACTIVATE, [this](wxActivateEvent &event)
            {
                if (!event.GetActive())
                {
                    // Проверяем, не внутри ли попапа новый фокус
                    wxWindow *focused = wxWindow::FindFocus();
                    if (focused && IsDescendant(focused))
                    {
                        event.Skip();
                        return;
                    }

                    Dismiss();
                }
                event.Skip();
            });

        PopupTransientWindow::Show();
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
