// 2025/6/4 10:27:32 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PanelDebug/PagesBlocks/Register.h"
#include "Panels/PanelDebug/PagesBlocks/PainterRegister.h"
#include "Controls/TextControls.h"
#include "Utils/StringUtils.h"
#include "Panels/PanelDebug/CommandsCombo.h"


Register::Register(wxWindow *parent, const wxString &_title, const wxString &_name, int _bit_depth, bool _sended, bool _received) :
    wxPanel(parent, wxID_ANY, wxDefaultPosition, { WIDTH, HEIGHT }, wxTAB_TRAVERSAL | wxSIMPLE_BORDER),
    bit_depth(_bit_depth),
    nameSTM32(_name),
    sended(_sended),
    received(_received)
{
    wxPanel::SetName("Register");

    new wxStaticText(this, wxID_ANY, _title + " " + _name, {10, 10});

    wxSize size_button{ 90, 25 };

    int x = 690;

    if (sended)
    {
        btnSend = new wxButton(this, wxID_ANY, "Записать", { x, 0 }, size_button);

        windows.push_back(btnSend);

        x -= size_button.x + 5;

        btnAutoSend = new wxToggleButton(this, wxID_ANY, "Автозапись", { x, 0 }, size_button);

        windows.push_back(btnAutoSend);

        x -= size_button.x + 5;
    }

    if (received)
    {
        btnReceptin = new wxToggleButton(this, wxID_ANY, "Чтение", { x, 0 }, size_button);

        windows.push_back(btnReceptin);

        x -= size_button.x + 5;
    }

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
    for (int i = 0; i < bit_depth; i++)
    {
        names.push_back(wxString::Format("D%d", i));
    }

    SetNamesBits(names);

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

                int x = painter->BitX(num_bit, bit_depth) - 4;

                elem.field.text_ctrl = new TextCtrlNumber(painter, wxID_ANY, "", { x, (PainterRegister::W_B + 1) * 3 }, { PainterRegister::W_B * elem.num_bits, 20 }, 0, (1 << elem.num_bits) - 1);

                elem.field.text_ctrl->Bind(wxEVT_TEXT, &Register::OnEventTextCtrl, this);
            }

            if (elem.field.need_commands)
            {
                int num_bit = elem.first_bit + elem.num_bits - 1;

                int x = painter->BitX(num_bit, bit_depth) - 4;

                wxArrayString names;
                for (auto &com : elem.field.commands)
                {
                    names.push_back(com.CreateFullLine(elem));
                }

                elem.field.combo = new CommandsCombo(painter, elem.desc, { x, (PainterRegister::W_B + 1) * 3 }, PainterRegister::W_B * elem.num_bits, names, "Register");

                elem.field.combo->left_align = true;

                elem.field.combo->Bind(wxEVT_COMBOBOX, &Register::OnEventCombo, this);
            }
        }
    }
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

                event.Skip();

                return;
            }
        }
    }

    event.Skip();
}


void Register::OnEventToggleButton(wxCommandEvent &event)
{
    int id = event.GetId();

    if (btnAutoSend && id == btnAutoSend->GetId())
    {
        SetActiveAcross(event.GetInt() == 0, btnAutoSend);
    }
    else if (btnReceptin && id == btnReceptin->GetId())
    {
        SetActiveAcross(event.GetInt() == 0, btnReceptin);
    }

    event.Skip();
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
}


void Register::OnEventCheckBox(wxCommandEvent &event)
{
    int id = event.GetId();

    for (int i = 0; i < (int)chbox.size(); i++)
    {
        if (chbox[(uint)i]->GetId() == id)                                  // Нашли данный бит
        {
            for (auto &d : desc[0])
            {
                if (d.field.need_text_ctrl)
                {
                    if (i >= d.first_bit && i < d.first_bit + d.num_bits)   // Нашли описатель поля, в которое входит данный бит
                    {
                        int value = 0;

                        int counter = 0;

                        for (int bit = d.first_bit; bit < d.first_bit + d.num_bits; bit++)
                        {
                            if (chbox[(uint)bit]->IsChecked())
                            {
                                value |= (1 << counter);
                            }

                            counter++;
                        }

                        d.field.text_ctrl->SetValue(wxString::Format("%d", value));

                        event.Skip();

                        return;
                    }
                }

            }
        }
    }

    event.Skip();
}


wxString StructDescription::CommandStruct::CreateFullLine(StructDescription &d) const
{
    return SU::BinToString(value, d.num_bits) + " - " + desc;
}


void Register::OnEventCombo(wxCommandEvent &event)
{
    int id = event.GetId();

    for (auto &d : desc[0])
    {
        if (d.field.need_commands)
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

    event.Skip();
}


RegAD5543::RegAD5543(wxWindow *_parent, const wxString &_name) :
    Register(_parent, "AD5543", _name, 12, true, false)
{

}


RegAD5531::RegAD5531(wxWindow *_parent, const wxString &_name) :
    Register(_parent, "AD5531", _name, 12, true, false)
{

}


RegAD7691::RegAD7691(wxWindow *_parent, const wxString &_name) :
    Register(_parent, "AD7691", _name, 18, false, true)
{

}


bool Register::Enable(bool enable)
{
    painter->SetEnabled(enable);

    return wxPanel::Enable(enable);
}
