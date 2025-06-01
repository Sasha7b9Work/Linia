// 2025/03/19 15:25:19 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Dialogs/EnterKeysSL0Dialog.h"
#include "Controls/ControlFunctions.h"


EnterKeysSL0Dialog *EnterKeysSL0Dialog::self = nullptr;


void EnterKeysSL0Dialog::Create(Key128 *key9000, Key128 *key9001, Key128 *key9002, Key128 *key9003, Key128 *key9004)
{
    if (!self)
    {
        self = new EnterKeysSL0Dialog(key9000, key9001, key9002, key9003, key9004);
    }
}


void EnterKeysSL0Dialog::Delete()
{
    SAFE_DELETE(self);
}


EnterKeysSL0Dialog::EnterKeysSL0Dialog(Key128 *key9000, Key128 *key9001, Key128 *key9002, Key128 *key9003, Key128 *key9004) :
    AutoMovedDialog(_L("Ключи безопасности"))
{
    wxBoxSizer *box = new wxBoxSizer(wxVERTICAL);

    wxSize client_size(260, 530);
    AutoMovedDialog::SetSize(client_size);

    keys[0] = key9000;
    keys[1] = key9001;
    keys[2] = key9002;
    keys[3] = key9003;
    keys[4] = key9004;

    int d = 20;

    box->AddSpacer(5);
    box->Add(CF::CreateFieldTextKey128(this, ID_TEXTCNTRL_KEY_9000, &controls[0][0], &controls[0][1], *keys[0], names[0]), 0, wxALIGN_CENTER);
    box->AddSpacer(d);
    box->Add(CF::CreateFieldTextKey128(this, ID_TEXTCNTRL_KEY_9001, &controls[1][0], &controls[1][1], *keys[1], names[1]), 0, wxALIGN_CENTER);
    box->AddSpacer(d);
    box->Add(CF::CreateFieldTextKey128(this, ID_TEXTCNTRL_KEY_9002, &controls[2][0], &controls[2][1], *keys[2], names[2]), 0, wxALIGN_CENTER);
    box->AddSpacer(d);
    box->Add(CF::CreateFieldTextKey128(this, ID_TEXTCNTRL_KEY_9003, &controls[3][0], &controls[3][1], *keys[3], names[3]), 0, wxALIGN_CENTER);
    box->AddSpacer(d);
    box->Add(CF::CreateFieldTextKey128(this, ID_TEXTCNTRL_KEY_9004, &controls[4][0], &controls[4][1], *keys[4], names[4]), 0, wxALIGN_CENTER);

    box->AddSpacer(d);

    {
        wxSize size_button = { 92, BUTTON_HEIGHT };
        wxBoxSizer *box_buttons = new wxBoxSizer(wxHORIZONTAL);
        box_buttons->AddSpacer(20);
        box_buttons->Add(new wxButton(this, ID_BUTTON_CLOSE, _L("Закрыть"), wxDefaultPosition, size_button));
        box_buttons->AddSpacer(20);
        box_buttons->Add(new wxButton(this, ID_BUTTON_SAVE, _L("Сохранить"), wxDefaultPosition, size_button));
        box->Add(box_buttons);
    }

    SetSizer(box);

    Bind(wxEVT_BUTTON, &EnterKeysSL0Dialog::OnEventButton, this);

    Bind(wxEVT_CHAR_HOOK, &EnterKeysSL0Dialog::OnEventKeyHook, this);
}


void EnterKeysSL0Dialog::OnEventKeyHook(wxKeyEvent &event)
{
    if (event.GetKeyCode() == 0x1b) // Escape
    {
        Close();
    }

    event.Skip();
}


void EnterKeysSL0Dialog::OnEventButton(wxCommandEvent &event)
{
    if (event.GetId() == ID_BUTTON_CLOSE)
    {
        Close();
    }
    else if (event.GetId() == ID_BUTTON_SAVE)
    {
        int result = CheckInputData();

        if (result == 0)
        {
            SetKeysAsCurrent();

            Close();
        }
        else
        {
            int num_field = (result >> 4);

            if ((result & 0x0f) == 1)
            {
                wxString message = _L("Поле");
                message += " \"";
                message += names[num_field];
                message += "\" ";
                message += _L("не совпадает");

                wxMessageDialog dlg(this, message, _L("Ошибка"), wxCENTRE | wxICON_ERROR);

                dlg.ShowModal();
            }
            else if((result & 0x0f) == 2)
            {
                wxString message = _L("Поле");
                message += " \"";
                message += names[num_field];
                message += "\" ";
                message += _L("заполнено не полностью.");

                wxMessageDialog dlg(this, message, _L("Ошибка"), wxCENTRE | wxICON_ERROR);

                dlg.ShowModal();
            }
        }
    }
}


void EnterKeysSL0Dialog::SetKeysAsCurrent()
{
    for (int i = 0; i < 5; i++)
    {
        keys[i]->bitset = controls[i][0]->key.bitset;
    }
}


int EnterKeysSL0Dialog::CheckInputData()
{
    for (int num_key = 0; num_key < 5; num_key++)
    {
        for (int i = 0; i < 2; i++)
        {
            controls[num_key][i]->CalculateKey();

            if (!controls[num_key][i]->IsFieldCompletelyFilled())
            {
                return (num_key << 4) + 2;
            }
        }

        if (std::memcmp(
            &controls[num_key][0]->key.bitset,
            &controls[num_key][1]->key.bitset,
            sizeof(controls[num_key][1]->key.bitset)) != 0)
        {
            return (num_key << 4) + 1;
        }
    }

    return 0;
}
