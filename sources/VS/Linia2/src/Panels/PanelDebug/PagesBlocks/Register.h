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


class Register : public wxPanel
{
    friend class PainterRegister;

public:

    static const int WIDTH = 800;
    static const int HEIGHT = 150;

    Register(wxWindow *parent, const wxString &_title,     // Это написано на изображении
        const wxString &_name,                                  // А это имя для работы с контроллером
        int bit_depth);

    void SetNamesBits(const wxArrayString &);

    void SetDescriptionBits(int index, const std::vector<StructDescription> &);

private:

    wxButton *btnSend = nullptr;
    wxToggleButton *btnAutoSend = nullptr;

    int bit_depth = 0;

    wxString nameSTM32;                         // Под этим именем идёт работа с платой контроллера

    PainterRegister *painter = nullptr;

    wxArrayString names_bits;                   // Названия битов

    std::vector<StructDescription> desc[2];     // Описания групп битов

    std::vector<wxCheckBox *> chbox;

    void OnEventTextCtrl(wxCommandEvent &);
    void OnEventCheckBox(wxCommandEvent &);
    void OnEventCombo(wxCommandEvent &);
    void OnEventToggleButton(wxCommandEvent &);
};


class RegFPGA : public Register
{
public:

    RegFPGA(wxWindow *_parent, const wxString & _name, int _bit_depth) :
        Register(_parent, "", _name, _bit_depth)
    {
    }
};


class RegAD5543 : public Register
{
public:

    RegAD5543(wxWindow *, const wxString &);
};


class RegAD5531 : public Register
{
public:

    RegAD5531(wxWindow *, const wxString &);
};
