// 2025/6/4 10:25:39 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls/ButtonsCombo.h"
#include "Panels/PanelDebug/Notebook/PainterRegister.h"
#include "Controls/TextControls.h"
#include "Panels/PanelDebug/Notebook/CommandsCombo.h"
#include "Controls/Painter.h"
#include "IPPP/Real/Chips.h"
#include "Controls/Knob.h"
#include "Controls/Slider.h"


// Визуальное представление регистра

class CheckBoxBit : public Painter
{
public:
    CheckBoxBit(wxWindow *, const wxPoint &, const wxSize &);

    void SetValue(bool);

    bool IsChecked() const;

    virtual bool Enable(bool) override;

private:

    bool value = false;

    void RePaint();

    void OnEventLeftClick(wxMouseEvent &);
};


// Описание группы бит
struct StructDescription
{
    struct CommandStruct
    {
        uint16   value;  // Значение
        wxString desc;   // Описание данного значения
        wxString CreateFullLine(StructDescription &) const;
        wxString CreateTooltip(StructDescription &) const;
    };
    int      first_bit;
    int      num_bits;
    wxString desc;
    wxString hint;
    struct DecField
    {
        bool need_text_ctrl_dec = false;            // если true, то есть возможность вводить десятичное значение
        std::vector<CommandStruct> commands;        // Сюда ложим нужные команды, чтобы потом создать combo
        TextCtrlNumber *text_ctrl_dec = nullptr;    // Здесь находятся десятичные значения
        CommandsCombo *combo = nullptr;             // А здесь находятся команды
    } field;

    // Рассчитать значение в соотвествие с чекбоксами
    uint CalculateValue(std::vector<CheckBoxBit *> &);
};


struct StateBit
{
    int num;        // Номер бита
    bool state;     // Состояние, в которое должен быть установлен бит
};


// Описание режима - при его выборе заданные биты устанавливаются в заданные положения
struct ModeDescripion
{
    wxString name;                  // Это название будет на кнопки органа управления
    wxString hint;                  // Более развёрнутое описание - будет выведено при наведении мыши на орган управления
    std::vector<StateBit> state;    // При выборе данного режима биты будут установлены в данные состояния
};


class Register : public wxPanel
{
    friend class PainterRegister;

public:

    static const int WIDTH = 800;
    static const int HEIGHT = 155;

    Register(wxWindow *parent, const wxString &_title,      // Это написано на изображении
        Chip *,
        bool need_knob);                                    // Нужна ли ручка значения

    void SetNamesBits(const wxArrayString &);

    void SetDescriptionBits(int index, const std::vector<StructDescription> &);

    void AppendModes(const wxString &title, const std::vector<ModeDescripion> &);

    virtual bool Enable(bool) override;

    void SetValue(uint);
    void Pack();
    void Unpack();

    // Записать значение в аппаратуру
    void WriteValue();

    const Chip *GetChip() const
    {
        return chip;
    }

private:

    KnobWidget *knob = nullptr;                 // Ручка установки значения
    SliderInt *slider_value = nullptr;          // Ползунок установки значения
    Chip *chip = nullptr;
    PainterRegister *painter = nullptr;
    wxCheckBox *chbSawDAC = nullptr;            // При включении в ЦАП будет засылаться пила
    int direction_saw = 1;                      // Направление изменения пилы

    wxButton *btnSend = nullptr;                // Однократная засылка
    wxToggleButton *btnAutoSend = nullptr;      // Если кнопка нажата, то каждую секунду происходит запись в данный регистр
    wxTimer timerAutoSend;                      // По этому таймеру будут автозасылки

    std::vector<wxWindow *> windows;

    wxArrayString names_bits;                   // Названия битов

    std::vector<StructDescription> desc[2];     // Описания групп битов

    std::vector<ModeDescripion> modes[5];                   // Описания режимов
    wxString title_modes[5];                                // Названия режимов
    // А это элемент управления для выбора режимов
    CommandsCombo *combo_modes[5] = { nullptr, nullptr, nullptr, nullptr, nullptr };

    std::vector<CheckBoxBit *> chboxes;

    // Создать элемент управления для выбора режима
    void CreateControlMode(int i);

    uint GetValue() const;

    // Возвращает true, если нужно выводить текстовое поле для десятичного значения хотя бы у одной группы бит
    bool NeedTextCtrlDEC() const;

    // Привести десятичные поля в соответствие с битовыми
    void UpdateDecFields();

    // Привести комбобоксы команд и режимов в соотвествие с битовыми полями
    void UpdateComboCommandsAndModes();

    // Увеличить высоту на dH
    void IncreaseHeight(int dH);

    void SetValueToKnob();

    void OnEventTextCtrl(wxCommandEvent &);
    void OnEventCheckBox(wxCommandEvent &);
    // Управление состоянием групп битов
    void OnEventComboField(wxCommandEvent &);
    // Выбор режима
    void OnEventComboMode(wxCommandEvent &);
    void OnEventToggleButton(wxCommandEvent &);
    void OnEventButton(wxCommandEvent &);
    void OnEventTimerAutoSend(wxTimerEvent &);

    // Все элементы кроме wnd будут установлены в состояние active
    void SetActiveAcross(bool active, wxWindow *wnd);

    void OnEventKnob(wxCommandEvent &);

    void OnEventSlider(wxCommandEvent &);
};


class RegFPGA : public Register
{
public:

    RegFPGA(wxWindow *_parent, Chip *_chip) :
        Register(_parent, "", _chip, false)
    {
    }
};


class RegDAC : public Register
{
public:
    RegDAC(wxWindow *, Chip *_chip);
};


class RegAD5543 : public RegDAC
{
public:

    // Если need_knob, то рисуем ручку
    RegAD5543(wxWindow *, Chip *_chip);
};


class RegAD5531 : public Register
{
public:

    RegAD5531(wxWindow *, Chip *_chip);
};
