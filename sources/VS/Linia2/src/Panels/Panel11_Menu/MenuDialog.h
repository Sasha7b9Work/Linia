// 2025/7/12 10:56:27 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Utils/GlobalFunctions.h"


class MenuDialog : public wxDialog
{
public:

    static const wxSize SIZE_BUTTON;

    MenuDialog(const wxString &title,
        const wxString &btn1, int id1, void (*func1)(),
        const wxString &btn2 = "", int id2 = 0, void (*func2)() = nullptr,
        const wxString &btn3 = "", int id3 = 0, void (*func3)() = nullptr,
        const wxString &btn4 = "", int id4 = 0, void (*func4)() = nullptr,
        const wxString &btn5 = "", int id5 = 0, void (*func5)() = nullptr,
        const wxString &btn6 = "", int id6 = 0, void (*func6)() = nullptr);

    virtual int ShowModal() override
    {
        wxPoint pos = GF::GetCoordCenter(GetSize());

        SetPosition(pos);

        return wxDialog::ShowModal();
    }

private:

    struct ButtonStruct
    {
        int      id;
        wxString label;
        void     (*func1)();
    };

    std::vector<ButtonStruct> buttons;
};
