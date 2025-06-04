// 2025/6/4 10:25:39 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


// Визуальное представление регистра


struct StructDescription
{
    int      first_byte;
    int      num_bytes;
    wxString desc;
};


class PanelRegister : public wxPanel
{
    friend class PainterRegister;

public:

    // Если reverse_bits, то биты идут в обратном порядке - первый на экране нулевой
    PanelRegister(wxWindow *parent, const wxString &title, int bit_depth, bool reverse_bits);

    void SetNamesBits(const wxArrayString &);

    void SetDescriptionBits(int index, const std::vector<StructDescription> &);

private:

    int bit_depth = 0;
    int reverse_bits = 0;

    wxArrayString names_bits;                   // Названия битов

    std::vector<StructDescription> desc[2];     // Описания групп битов
};
