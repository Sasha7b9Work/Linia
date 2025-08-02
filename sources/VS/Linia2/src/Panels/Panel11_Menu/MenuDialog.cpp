// 2025/7/12 10:56:03 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel11_Menu/MenuDialog.h"


MenuDialog::MenuDialog(const wxString &title, int width_button,
    const wxString &btn1, void (*func1)(),
    const wxString &btn2, void (*func2)(),
    const wxString &btn3, void (*func3)(),
    const wxString &btn4, void (*func4)(),
    const wxString &btn5, void (*func5)(),
    const wxString &btn6, void (*func6)(),
    const wxString &btn7, void (*func7)(),
    const wxString &btn8, void (*func8)(),
    const wxString &btn9, void (*func9)()) :
    Dialog(nullptr, wxID_ANY, title)
{
    {
        if (func1) buttons.push_back({ wxID_ANY, btn1, func1 });
        if (func2) buttons.push_back({ wxID_ANY, btn2, func2 });
        if (func3) buttons.push_back({ wxID_ANY, btn3, func3 });
        if (func4) buttons.push_back({ wxID_ANY, btn4, func4 });
        if (func5) buttons.push_back({ wxID_ANY, btn5, func5 });
        if (func6) buttons.push_back({ wxID_ANY, btn6, func6 });
        if (func7) buttons.push_back({ wxID_ANY, btn7, func7 });
        if (func8) buttons.push_back({ wxID_ANY, btn8, func8 });
        if (func9) buttons.push_back({ wxID_ANY, btn9, func9 });
    }

    wxSize client_size{ width_button + 10, (BUTTON_HEIGHT + 5) * (int)buttons.size() + 10 };

    Dialog::SetClientSize(client_size);

    for (uint i = 0; i < buttons.size(); i++)
    {
        wxButton *btn = new wxButton(this, buttons[i].id, buttons[i].label, { 5, 5 + ((int)i * (BUTTON_HEIGHT + 5)) }, { width_button, BUTTON_HEIGHT });
        buttons[i].id = btn->GetId();
    }

    Bind(wxEVT_BUTTON, &MenuDialog::OnEventButton, this);
}


void MenuDialog::OnEventButton(wxCommandEvent &event)
{
    for (auto &btn : buttons)
    {
        if (btn.id == event.GetId())
        {
            btn.func();
            break;
        }
    }
}
