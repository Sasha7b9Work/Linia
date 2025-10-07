// 2025/6/4 10:25:39 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls/ButtonsCombo.h"
#include "Panels/PanelDebug/PagesBlocks/PainterRegister.h"
#include "Controls/TextControls.h"
#include "Panels/PanelDebug/CommandsCombo.h"


// Визуальное представление регистра


struct StructDescription
{
    struct CommandStruct
    {
        uint16   value;  // Значение
        wxString desc;   // Описание данного значения
        wxString CreateFullLine(StructDescription &) const;
    };
    int      first_bit;
    int      num_bits;
    wxString desc;
    struct DecField
    {
        bool need_text_ctrl = false;            // если true, то есть возможность вводить десятичное значение
        bool need_commands = false;             // если true, то есть возможность выбирать команды
        std::vector<CommandStruct> commands;     // Сюда ложим нужные команды, чтобы потом создать combo
        TextCtrlNumber *text_ctrl = nullptr;
        CommandsCombo *combo = nullptr;
    };
    DecField field;
};


class PanelRegister : public wxPanel
{
    friend class PainterRegister;

public:

    static const int WIDTH = 800;
    static const int HEIGHT = 150;

    PanelRegister(wxWindow *parent, const wxString &title, int bit_depth);

    void SetNamesBits(const wxArrayString &);

    void SetDescriptionBits(int index, const std::vector<StructDescription> &);

private:

    int bit_depth = 0;

    PainterRegister *painter = nullptr;

    wxArrayString names_bits;                   // Названия битов

    std::vector<StructDescription> desc[2];     // Описания групп битов

    std::vector<wxCheckBox *> chbox;

    void OnEventTextCtrl(wxCommandEvent &);
    void OnEventCheckBox(wxCommandEvent &);
    void OnEventCombo(wxCommandEvent &);
};


class RegFPGA : public PanelRegister
{
public:

    RegFPGA(wxWindow *_parent, const wxString &_title, int _bit_depth) :
        PanelRegister(_parent, _title, _bit_depth)
    {
    }
};
