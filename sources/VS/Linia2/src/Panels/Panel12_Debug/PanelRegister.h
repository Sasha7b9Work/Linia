// 2025/6/4 10:25:39 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


// Визуальное представление регистра


struct StructDescription
{
    int      first_bit;
    int      num_bits;
    wxString desc;
};


class PanelRegister : public wxPanel
{
    friend class PainterRegister;

public:

    static const int WIDTH = 800;
    static const int HEIGHT = 250;

    // Если reverse_bits, то биты идут в обратном порядке - первый на экране нулевой
    // Если additional_modes, то добавляется строка дополнительных режимов
    PanelRegister(wxWindow *parent, const wxString &title, int bit_depth, bool reverse_bits, bool additional_modes);

    void SetNamesBits(const wxArrayString &);

    void SetDescriptionBits(int index, const std::vector<StructDescription> &);

private:

    int bit_depth = 0;
    int reverse_bits = 0;
    bool additional_modes = false;

    wxArrayString names_bits;                   // Названия битов

    std::vector<StructDescription> desc[2];     // Описания групп битов
};
