// 2025/6/4 10:25:39 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


// Визуальное представление регистра


class PanelRegister : public wxPanel
{
    friend class PainterRegister;

public:

    // Если reverse_bits, то биты идут в обратном порядке - первый на экране нулевой
    PanelRegister(wxWindow *parent, const wxString &title, int bit_depth, bool reverse_bits);

private:

    int bit_depth = 0;
    int reverse_bits = 0;
};
