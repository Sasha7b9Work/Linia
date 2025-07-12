// 2025/7/12 10:56:03 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel11_Menu/MenuDialog.h"


const wxSize MenuDialog::SIZE_BUTTON { 120, 30 };


MenuDialog::MenuDialog(const wxString &title,
    const wxString &btn1, int id1, void (*func1)(),
    const wxString &btn2, int id2, void (*func2)(),
    const wxString &btn3, int id3, void (*func3)(),
    const wxString &btn4, int id4, void (*func4)(),
    const wxString &btn5, int id5, void (*func5)(),
    const wxString &btn6, int id6, void (*func6)()) :
    wxDialog(nullptr, wxID_ANY, title)
{
    {
        if (func1) buttons.push_back({ id1, btn1, func1 });
        if (func2) buttons.push_back({ id2, btn2, func2 });
        if (func3) buttons.push_back({ id3, btn3, func3 });
        if (func4) buttons.push_back({ id4, btn4, func4 });
        if (func5) buttons.push_back({ id5, btn5, func5 });
        if (func6) buttons.push_back({ id6, btn6, func6 });
    }

    wxSize client_size{ SIZE_BUTTON.x + 10, (SIZE_BUTTON.y + 5) * (int)buttons.size() + 10 };

    wxDialog::SetClientSize(client_size);

    for (uint i = 0; i < buttons.size(); i++)
    {
        new wxButton(this, buttons[i].id, buttons[i].label, { 5, 5 + ((int)i * (SIZE_BUTTON.y + 5)) }, SIZE_BUTTON);
    }
}

