// 2025/6/4 10:27:32 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PanelDebug/Notebook/Register.h"
#include "Panels/PanelDebug/Notebook/PainterRegister.h"
#include "Controls/TextControls.h"
#include "Utils/StringUtils.h"
#include "Panels/PanelDebug/Notebook/CommandsCombo.h"
#include "IPPP/Device/IDevice.h"
#include "Panels/PanelDebug/Notebook/NotebookDebug.h"
#include "Panels/PanelDebug/Notebook/PageChip.h"
#include "Controls/Knob.h"


// Функции для комбобоксов выбора режимов
namespace ComboRange
{
    static void UpdateState(std::vector<ModeDescripion> &, std::vector<CheckBoxBit *> chbox, CommandsCombo *);
}


Register::Register(wxWindow *parent, const wxString &_title, Chip *_chip, bool need_knob) :
    wxPanel(parent, wxID_ANY, wxDefaultPosition, { WIDTH, HEIGHT }, wxTAB_TRAVERSAL | wxSIMPLE_BORDER),
    chip(_chip)
{
    wxPanel::SetName("Register");

    new wxStaticText(this, wxID_ANY, _title + " " + chip->GetNameDevice(), {10, 10});

    wxSize size_button{ 90, 25 };

    int x = 690;

    btnSend = new wxButton(this, wxID_ANY, "Записать", { x, 0 }, size_button);

    windows.push_back(btnSend);

    x -= size_button.x + 5;

    btnAutoSend = new wxToggleButton(this, wxID_ANY, "Автозапись", { x, 0 }, size_button);

    windows.push_back(btnAutoSend);

    x -= size_button.x + 5;

    int y0 = 40;

    {
        painter = new PainterRegister(this, this, { 10, y0 });

        for (auto box : chbox)
        {
            box->Bind(wxEVT_CHECKBOX, &Register::OnEventCheckBox, this);
        }
    }

    Bind(wxEVT_RIGHT_DOWN, [](wxMouseEvent &event)
        {
            event.Skip(); // Пропустить событие дальше (родителю)
        });

    wxArrayString names;
    for (int i = 0; i < chip->BitDepth(); i++)
    {
        names.push_back(wxString::Format("D%d", i));
    }

    SetNamesBits(names);

    Bind(wxEVT_BUTTON, &Register::OnEventButton, this);
    Bind(wxEVT_TOGGLEBUTTON, &Register::OnEventToggleButton, this);
    Bind(wxEVT_TIMER, &Register::OnEventTimerAutoSend, this);

    timerAutoSend.SetOwner(this, timerAutoSend.GetId());

    if (need_knob)
    {
        const int SIZE = 50;

        const int d = 10;

        knob = new KnobWidget(painter, wxID_ANY, 0, 100, 50, { painter->GetSize().x - SIZE - d - 20, d }, { SIZE, SIZE });

        knob->Bind(wxEVT_SLIDER, &RegAD5543::OnEventKnob, this);

        slider_value = new SliderInt(painter, { painter->BitX(chip->BitDepth() - 1, chip->BitDepth()), 75 }, chip->BitDepth() * 20, 0, 100, "");

        slider_value->Bind(wxEVT_SLIDER, &RegAD5543::OnEventSlider, this);
    }
}


void Register::SetNamesBits(const wxArrayString &_names)
{
    names_bits = _names;
}


void Register::AppendModes(const wxString &title, const std::vector<ModeDescripion> &mode_desc)
{
    for (uint i = 0; i < 10; i++)
    {
        if (modes[i].size() == 0)
        {
            modes[i] = mode_desc;
            title_modes[i] = title;
            CreateControlMode((int)i);
            break;
        }
    }

    int dH = 30;

    painter->IncreaseHeight(dH);

    IncreaseHeight(dH);

    PageChip *page_chip = (PageChip *)GetParent();

    page_chip->Rebuild();
}


void Register::IncreaseHeight(int dH)
{
    wxSize size = GetSize();
    size.y += dH;

    SetMinSize(size);
    SetMaxSize(size);

    SetSize(size);

    Layout();
}


void Register::SetDescriptionBits(int index, const std::vector<StructDescription> &_desc)
{
    desc[index] = _desc;

    if (index == 0)
    {
        bool need_dec = NeedTextCtrlDEC();

        for (auto &elem : desc[0])
        {
            if (elem.field.need_text_ctrl_dec)
            {
                int num_bit = elem.first_bit + elem.num_bits - 1;

                int x = painter->BitX(num_bit, chip->BitDepth());

                int num_y = elem.desc[0] ? 3 : 2;

                elem.field.text_ctrl_dec = new TextCtrlNumber(painter, wxID_ANY, "",
                    { x, (PainterRegister::W_B + 1) * num_y },
                    { PainterRegister::W_B * elem.num_bits + 1, 20 },
                    0, (1 << elem.num_bits) - 1);

                elem.field.text_ctrl_dec->Bind(wxEVT_TEXT, &Register::OnEventTextCtrl, this);
            }

            if (elem.field.commands.size())
            {
                int num_bit = elem.first_bit + elem.num_bits - 1;

                int x = painter->BitX(num_bit, chip->BitDepth()) + 1;

                wxArrayString names;
                for (auto &com : elem.field.commands)
                {
                    names.push_back(com.CreateFullLine(elem));
                }

                wxArrayString tooltips;
                for (auto &com : elem.field.commands)
                {
                    tooltips.push_back(com.CreateTooltip(elem));
                }

                int y = (PainterRegister::W_B + 1) * (need_dec ? 4 : 3) - 1;

                elem.field.combo = new CommandsCombo(painter, elem.hint,
                    { x, y },
                    PainterRegister::W_B * elem.num_bits - 1, names, tooltips, "Register");

                elem.field.combo->left_align = true;

                elem.field.combo->Bind(wxEVT_COMBOBOX, &Register::OnEventComboField, this);
            }
        }
    }

    UpdateDecFields();
}


void Register::CreateControlMode(int i)
{
    int x = 10 + i * 300;
    int y = 110;

    new wxStaticText(painter, wxID_ANY, title_modes[i], { x, y });

    wxArrayString names;
    for (auto &mode : modes[i])
    {
        names.push_back(mode.name);
    }

    wxArrayString tooltips;
    for (auto &mode : modes[i])
    {
        tooltips.push_back(mode.hint);
    }

    combo_modes[i] = new CommandsCombo(painter, title_modes[i], { x, y + 25 }, 250, names, tooltips, title_modes[i]);

    combo_modes[i]->Bind(wxEVT_COMBOBOX, &Register::OnEventComboMode, this);
}


void Register::OnEventTextCtrl(wxCommandEvent &event)
{
    int id = event.GetId();

    for (auto &d : desc[0])
    {
        if (d.field.need_text_ctrl_dec)
        {
            if (id == d.field.text_ctrl_dec->GetId())
            {
                wxString str = d.field.text_ctrl_dec->GetValue();

                long value = 0;
                str.ToLong(&value);

                int index = d.first_bit;        // Индекс первого бита (младшего)

                for (int i = 0; i < d.num_bits; i++)
                {
                    bool bit = value & 1;

                    chbox[(uint)index++]->SetValue(bit);

                    value >>= 1;
                }

                break;
            }
        }
    }

    event.Skip();

    UpdateComboCommandsAndModes();
}


void Register::OnEventToggleButton(wxCommandEvent &event)
{
    int id = event.GetId();

    if (btnAutoSend && id == btnAutoSend->GetId())
    {
        SetActiveAcross(event.GetInt() == 0, btnAutoSend);

        if (event.GetInt())
        {
            timerAutoSend.Start(1000);
        }
        else
        {
            timerAutoSend.Stop();
        }
    }

    event.Skip();
}


void Register::OnEventButton(wxCommandEvent &event)
{
    int id = event.GetId();

    if (id == btnSend->GetId())
    {
        chip->WriteValueToDevice(GetValue());
    }
}


void Register::OnEventTimerAutoSend(wxTimerEvent &)
{
    chip->WriteValueToDevice(GetValue());
}


void Register::SetActiveAcross(bool active, wxWindow *_wnd)
{
    for (auto wnd : windows)
    {
        if (wnd != _wnd)
        {
            wnd->Enable(active);
        }
    }

    for (auto &d : desc[0])
    {
        if (d.field.text_ctrl_dec)
        {
            d.field.text_ctrl_dec->Enable(active);
        }
        if(d.field.combo)
        {
            d.field.combo->Enable(active);
        }
    }

    for (auto *chb : chbox)
    {
        chb->Enable(active);
    }

    NotebookDebug::self->EnableSwitching(active);
}


bool Register::NeedTextCtrlDEC() const
{
    for (uint num_desc = 0; num_desc < 2; num_desc++)
    {
        const std::vector<StructDescription> &d = desc[num_desc];

        if (d.size() > 0)
        {
            for (uint i = 0; i < d.size(); i++)
            {
                if (d[i].field.need_text_ctrl_dec)
                {
                    return true;
                }
            }
        }
    }

    return false;
}


void Register::UpdateDecFields()
{
    if (!NeedTextCtrlDEC())
    {
        return;
    }

    for (int i = 0; i < (int)chbox.size(); i++)                         // Перебираем все биты
    {
        for (auto &d : desc[0])
        {
            if (d.field.need_text_ctrl_dec)
            {
                if (i >= d.first_bit && i < d.first_bit + d.num_bits)   // Нашли описатель поля, в которое входит данный бит
                {
                    uint value = d.CalculateValue(chbox);

                    d.field.text_ctrl_dec->SetValue(wxString::Format("%u", value));
                }
            }
        }
    }
}


uint StructDescription::CalculateValue(std::vector<CheckBoxBit *> &chbox)
{
    uint value = 0;

    int counter = 0;

    for (int bit = first_bit; bit < first_bit + num_bits; bit++)
    {
        if (chbox[(uint)bit]->IsChecked())
        {
            value |= (1 << counter);
        }

        counter++;
    }

    return value;
}


void Register::OnEventCheckBox(wxCommandEvent &event)
{
    UpdateDecFields();

    UpdateComboCommandsAndModes();

    event.Skip();
}


wxString StructDescription::CommandStruct::CreateFullLine(StructDescription &d) const
{
    return SU::BinToString(value, d.num_bits) + " - " + desc;
}


wxString StructDescription::CommandStruct::CreateTooltip(StructDescription &) const
{
    return desc;
}


void Register::OnEventComboField(wxCommandEvent &event)
{
    int id = event.GetId();

    for (auto &d : desc[0])
    {
        if (d.field.commands.size())
        {
            if (d.field.combo->GetId() == id)
            {
                int num_bit = d.first_bit;
                uint16 value = d.field.commands[(uint)event.GetInt()].value;

                for (int i = 0; i < d.num_bits; i++)
                {
                    chbox[(uint)num_bit++]->SetValue((value & (1 << i)) != 0);
                }

                break;
            }
        }
    }

    UpdateDecFields();

    event.Skip();
}


void Register::OnEventComboMode(wxCommandEvent &event)
{
    int id = event.GetId();

    int num_mode = -1;

    for (int i = 0; i < 5; i++)
    {
        if (combo_modes[i])
        {
            if (combo_modes[i]->GetId() == id)
            {
                num_mode = i;
                break;
            }
        }
        else
        {
            break;
        }
    }

    if (num_mode == -1)
    {
        return;
    }

    ModeDescripion &mode = modes[num_mode][(uint)event.GetInt()];

    for (uint i = 0; i < mode.state.size(); i++)
    {
        StateBit &state = mode.state[i];

        chbox[(uint)state.num]->SetValue(state.state);
    }

    UpdateComboCommandsAndModes();
}


void Register::UpdateComboCommandsAndModes()
{
    for (auto &d : desc[0])
    {
        if (d.field.commands.size())
        {
            uint value = d.CalculateValue(chbox);

            bool exist_value = false;

            for (uint i = 0; i < d.field.commands.size(); i++)
            {
                if (exist_value)
                {
                    break;
                }

                if (value == d.field.commands[i].value)
                {
                    d.field.combo->SetCurrentSelection((int)i);

                    exist_value = true;
                }
            }

            if(!exist_value)
            {
                d.field.combo->SetInvalidChoice();
            }
        }
    }

    for (int num_combo = 0; num_combo < 5; num_combo++)
    {
        if (combo_modes[num_combo])
        {
            ComboRange::UpdateState(modes[num_combo], chbox, combo_modes[num_combo]);
        }
    }

    SetValueToKnob();
}


void ComboRange::UpdateState(std::vector<ModeDescripion> &mode_desc, std::vector<CheckBoxBit *> chbox, CommandsCombo *combo)
{
    for (uint num_desc = 0; num_desc < mode_desc.size(); num_desc++)
    {
        ModeDescripion &_desc = mode_desc[num_desc];

        std::vector<StateBit> &state_bit_array = _desc.state;

        bool current_state = true;          // Если после следующего цикла это значение будет оставаться правдой,
                                            // это означает, что биты регистра соответствуют данному std::vector<StateBit>

        for (uint num_bit = 0; num_bit < state_bit_array.size(); num_bit++)
        {
            StateBit &state_bit = state_bit_array[num_bit];

            if (chbox[(uint)state_bit.num]->IsChecked() != state_bit.state)
            {
                current_state = false;
                break;
            }
        }

        if (current_state)
        {
            combo->SetCurrentSelection((int)num_desc);
            return;
        }
    }

    combo->SetInvalidChoice();
}


RegAD5543::RegAD5543(wxWindow *_parent, Chip *_chip) :
    Register(_parent, "AD5543", _chip, true)
{
}

void Register::OnEventKnob(wxCommandEvent &event)
{
    if (event.GetId() == knob->GetId())
    {
        int max_value = (1 << chip->BitDepth()) - 1;

        uint new_value = (uint)(max_value * event.GetInt() / 100);

        if (GetValue() != new_value)
        {
            SetValue(new_value);
        }
    }

    event.Skip();
}


void Register::OnEventSlider(wxCommandEvent &event)
{
    /*
    if (event.GetId() == slider_value->GetId())
    {
        int max_value = (1 << chip->BitDepth()) - 1;

        uint new_value = (uint)(max_value * event.GetInt() / 100);

        if (GetValue() != new_value)
        {
            SetValue(new_value);
        }
    }
    */

    event.Skip();
}


RegAD5531::RegAD5531(wxWindow *_parent, Chip *_chip) :
    Register(_parent, "AD5531", _chip, true)
{
}


bool Register::Enable(bool enable)
{
    painter->SetEnabled(enable);

    return wxPanel::Enable(enable);
}


uint Register::GetValue() const
{
    uint result = 0;

    for (uint i = 0; i < chbox.size(); i++)
    {
        if (chbox[i]->IsChecked())
        {
            result |= (1 << i);
        }
    }

    return result;
}


void Register::SetValue(uint new_value)
{
    for (uint i = 0; i < chbox.size(); i++)
    {
        chbox[i]->SetValue((new_value & (1 << i)) != 0);
    }

    UpdateComboCommandsAndModes();
    UpdateDecFields();
}


CheckBoxBit::CheckBoxBit(wxWindow *parent, const wxPoint &pos, const wxSize &size) :
    Painter(parent, pos, size)
{
    RePaint();

    Bind(wxEVT_LEFT_DOWN, &CheckBoxBit::OnEventLeftClick, this);
}


void CheckBoxBit::SetValue(bool new_value)
{
    value = new_value;

    RePaint();
}


bool CheckBoxBit::IsChecked() const
{
    return value;
}


bool CheckBoxBit::Enable(bool enable)
{
    bool result = Painter::Enable(enable);

    RePaint();

    return result;
}


void CheckBoxBit::RePaint()
{
    BeginPaint(IsEnabled() ? *wxWHITE : *wxLIGHT_GREY);

    wxFont font(7, wxFONTFAMILY_DEFAULT,
        wxFONTSTYLE_NORMAL,
        wxFONTWEIGHT_BOLD);

    font.SetPointSize(10);

    gc->SetFont(font, *wxBLACK);

    gc->DrawText(value ? "1" : "0", 7, 0);

    EndPaint();
}


void CheckBoxBit::OnEventLeftClick(wxMouseEvent &)
{
    value = !value;

    RePaint();

    GF::SendCommandEvent(this, wxEVT_CHECKBOX, value ? 1 : 0);
}


void Register::Pack()
{
    Config::WriteUint(chip->GetNameDevice(), GetValue());
}


void Register::Unpack()
{
    SetValue(Config::ReadUint(chip->GetNameDevice()));

    SetValueToKnob();
}


void Register::SetValueToKnob()
{
    if (knob)
    {
        knob->Unbind(wxEVT_SLIDER, &Register::OnEventKnob, this);
        slider_value->Unbind(wxEVT_SLIDER, &Register::OnEventSlider, this);

        int max_value = (1 << chip->BitDepth()) - 1;

        int new_value = (int)((float)GetValue() * 100.0f / (float)max_value + 0.5f);

        knob->SetValue(new_value);

        slider_value->SetValue(new_value);

        knob->Bind(wxEVT_SLIDER, &Register::OnEventKnob, this);
        slider_value->Bind(wxEVT_SLIDER, &Register::OnEventSlider, this);
    }
}
