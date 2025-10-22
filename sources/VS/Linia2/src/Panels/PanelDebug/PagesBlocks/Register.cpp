// 2025/6/4 10:27:32 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PanelDebug/PagesBlocks/Register.h"
#include "Panels/PanelDebug/PagesBlocks/PainterRegister.h"
#include "Controls/TextControls.h"
#include "Utils/StringUtils.h"
#include "Panels/PanelDebug/PagesBlocks/CommandsCombo.h"
#include "IPPP/Device/IDevice.h"
#include "Panels/PanelDebug/NotebookDebug.h"
#include "Panels/PanelDebug/PagesBlocks/PageChip.h"


Register::Register(wxWindow *parent, const wxString &_title, Chip *_chip) :
    wxPanel(parent, wxID_ANY, wxDefaultPosition, { WIDTH, HEIGHT }, wxTAB_TRAVERSAL | wxSIMPLE_BORDER),
    chip(_chip)
{
    wxPanel::SetName("Register");

    new wxStaticText(this, wxID_ANY, _title + " " + chip->GetNameSTM32(), {10, 10});

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
}


void Register::SetNamesBits(const wxArrayString &_names)
{
    names_bits = _names;
}


void Register::SetDescriptionBits(int index, const std::vector<StructDescription> &_desc)
{
    desc[index] = _desc;

    if (index == 0)
    {
        for (auto &elem : desc[0])
        {
            if (elem.field.need_text_ctrl)
            {
                int num_bit = elem.first_bit + elem.num_bits - 1;

                int x = painter->BitX(num_bit, chip->BitDepth());

                elem.field.text_ctrl = new TextCtrlNumber(painter, wxID_ANY, "", { x, (PainterRegister::W_B + 1) * 3 }, { PainterRegister::W_B * elem.num_bits + 1, 20 }, 0, (1 << elem.num_bits) - 1);

                elem.field.text_ctrl->Bind(wxEVT_TEXT, &Register::OnEventTextCtrl, this);
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

                elem.field.combo = new CommandsCombo(painter, elem.hint, { x, (PainterRegister::W_B + 1) * 4 - 1}, PainterRegister::W_B * elem.num_bits - 1, names, tooltips, "Register");

                elem.field.combo->left_align = true;

                elem.field.combo->Bind(wxEVT_COMBOBOX, &Register::OnEventCombo, this);
            }
        }
    }

    UpdateDecFields();
}


void Register::OnEventTextCtrl(wxCommandEvent &event)
{
    int id = event.GetId();

    for (auto &d : desc[0])
    {
        if (d.field.need_text_ctrl)
        {
            if (id == d.field.text_ctrl->GetId())
            {
                wxString str = d.field.text_ctrl->GetValue();

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

    UpdateComboCommands();
}


void Register::OnEventToggleButton(wxCommandEvent &event)
{
    int id = event.GetId();

    if (btnAutoSend && id == btnAutoSend->GetId())
    {
        SetActiveAcross(event.GetInt() == 0, btnAutoSend);
    }

    event.Skip();
}


void Register::OnEventButton(wxCommandEvent &event)
{
    int id = event.GetId();

    if (id == btnSend->GetId())
    {
        chip->WriteValueToSTM32(GetValue());
    }
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
        if (d.field.text_ctrl)
        {
            d.field.text_ctrl->Enable(active);
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

    ((PageChip *)GetParent())->GetButtonReturn()->Enable(active);

    NotebookDebug::self->EnableSwitching(active);
}


void Register::UpdateDecFields()
{
    for (int i = 0; i < (int)chbox.size(); i++)                         // Перебираем все биты
    {
        for (auto &d : desc[0])
        {
            if (d.field.need_text_ctrl)
            {
                if (i >= d.first_bit && i < d.first_bit + d.num_bits)   // Нашли описатель поля, в которое входит данный бит
                {
                    uint value = d.CalculateValue(chbox);

                    d.field.text_ctrl->SetValue(wxString::Format("%u", value));
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

    UpdateComboCommands();

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


void Register::OnEventCombo(wxCommandEvent &event)
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


void Register::UpdateComboCommands()
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
}


RegAD5543::RegAD5543(wxWindow *_parent, Chip *_chip) :
    Register(_parent, "AD5543", _chip)
{

}


RegAD5531::RegAD5531(wxWindow *_parent, Chip *_chip) :
    Register(_parent, "AD5531", _chip)
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

    UpdateComboCommands();
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
    Config::WriteUint(chip->GetNameSTM32(), GetValue());
}


void Register::Unpack()
{
    SetValue(Config::ReadUint(chip->GetNameSTM32()));
}
