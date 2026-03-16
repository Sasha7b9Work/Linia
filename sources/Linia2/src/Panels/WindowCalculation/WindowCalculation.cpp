// 2026/03/16 09:12:56 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/WindowCalculation/WindowCalculation.h"
#include "Settings/Settings.h"


WindowCalculation::WindowCalculation(wxFrame *parent) :
    wxFrame(parent, wxID_ANY, "Расчёт", wxDefaultPosition, wxSize(300, 200), wxDEFAULT_FRAME_STYLE | wxFRAME_FLOAT_ON_PARENT)
{
    wxPanel *panel = new wxPanel(this);
    wxButton *btn = new wxButton(panel, wxID_ANY,
        "Вызвать функцию главного окна",
        wxPoint(20, 20), wxSize(250, 30));

    btn->Bind(wxEVT_BUTTON, [this, parent](wxCommandEvent &)
        {
            if (GetParent())
            {
                ((wxFrame *)GetParent())->SetTitle("Главное окно - функция вызвана из дочернего");

                SET::GUI::color_background.Set((*wxWHITE).GetRGB());
            }
        });

    // Кнопка закрытия
    wxButton *closeBtn = new wxButton(panel, wxID_ANY, "Закрыть", wxPoint(20, 70), wxSize(250, 30));

    closeBtn->Bind(wxEVT_BUTTON, [this](wxCommandEvent &)
        {
            Close();
        });

    // Разрешаем окну получать фокус и взаимодействовать
    SetFocus();
}

