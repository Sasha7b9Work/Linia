// 2025/03/19 10:04:36 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Dialogs/KeysSL0Dialog.h"
#include "Panels/Dialogs/EnterKeysSL0Dialog.h"
#include "Settings/Settings.h"
#include "Utils/StringUtils.h"
#include "Reader/Reader.h"
#include "Utils/GlobalFunctions.h"


KeysSL0Dialog *KeysSL0Dialog::self = nullptr;


void KeysSL0Dialog::Create()
{
    if (!self)
    {
        self = new KeysSL0Dialog();
    }
}


void KeysSL0Dialog::Delete()
{
    SAFE_DELETE(self);
}


KeysSL0Dialog::KeysSL0Dialog() :
    AutoMovedDialog(_L("Ключи Mifare Plus"))
{
    self = this;

    wxSize client_size(300, 330);

    AutoMovedDialog::SetSize(client_size);

    wxBoxSizer *box = new wxBoxSizer(wxVERTICAL);

    int d = 20;

    box->AddSpacer(d);

    box->Add(CreateBoxButton(ID_BUTTON_KEYS_SL0_MANUAL, _L("Ввод значений ключей")));                   box->AddSpacer(d);
    box->Add(CreateBoxButton(ID_BUTTON_KEYS_SL0_SAVE_FILE, _L("Сохранить ключи в файл")));              box->AddSpacer(d);
    box->Add(CreateBoxButton(ID_BUTTON_KEYS_SL0_READ_FILE, _L("Считать ключи из файла")));              box->AddSpacer(d);
    box->Add(CreateBoxButton(ID_BUTTON_KEYS_SL0_WRITE_TO_CARD, _L("Записать ключи на карту")));         box->AddSpacer(d);
    box->Add(CreateBoxButton(ID_BUTTON_KEYS_SL0_WRITE_TO_READER, _L("Записать ключи в считыватель")));  box->AddSpacer(d);
    box->Add(CreateBoxButton(ID_BUTTON_CLOSE, _L("Закрыть")));

    SetSizer(box);

    Bind(wxEVT_BUTTON, &KeysSL0Dialog::OnEventButton, this);

    Bind(wxEVT_CHAR_HOOK, &KeysSL0Dialog::OnEventKeyHook, this);

    SaveCurrentKeys(old_keys);
}


wxBoxSizer *KeysSL0Dialog::CreateBoxButton(int id, const wxString &title)
{
    wxBoxSizer *box = new wxBoxSizer(wxHORIZONTAL);

    box->AddSpacer(25);

    box->Add(new wxButton(this, id, title, wxDefaultPosition, { 240, BUTTON_HEIGHT }));

    return box;
}


void KeysSL0Dialog::OnEventKeyHook(wxKeyEvent &event)
{
    if (event.GetKeyCode() == 0x1b) // Escape
    {
        Close();
    }

    event.Skip();
}


void KeysSL0Dialog::OnEventButton(wxCommandEvent &event)
{
    int id = event.GetId();

    if (id == ID_BUTTON_KEYS_SL0_MANUAL)
    {
        EnterKeysSL0Dialog::self->ShowModal();

        if (!KeyValuesAreTheSame(old_keys))
        {
            keys_is_saved = false;
        }

        SaveCurrentKeys(old_keys);
    }
    else if (id == ID_BUTTON_KEYS_SL0_READ_FILE)
    {
        wxFileDialog dialog(this, _L("Загрузить ключи"), wxEmptyString, wxEmptyString, "*.*", wxFD_OPEN);

        if (dialog.ShowModal() == wxID_OK)
        {
            if (LoadFromFile(dialog.GetPath()))
            {
                wxMessageBox(_L("Ключи успешно загружены из файла") + " " + dialog.GetPath() + ".", _L("Сообщение"), wxICON_INFORMATION);
            }
            else
            {
                wxMessageBox(_L("Ошибка загрузки ключей из файла") + " " + dialog.GetPath() + ".", _L("Ошибка"), wxICON_ERROR);
            }
        }
    }
    else if (id == ID_BUTTON_KEYS_SL0_SAVE_FILE)
    {
        wxFileDialog dialog(this, _L("Сохранить ключи"), wxEmptyString, wxEmptyString, "*.*", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

        if (dialog.ShowModal() == wxID_OK)
        {
            if (SaveToFile(dialog.GetPath()))
            {
                wxMessageBox(_L("Ключи успешно сохранены в файле") + " " + dialog.GetPath() + ".", _L("Сообщение"), wxICON_INFORMATION);

                keys_is_saved = true;
            }
            else
            {
                wxMessageBox(_L("Ошибка сохранения ключей в файл") + " " + dialog.GetPath() + ".", _L("Ошибка"), wxICON_ERROR);
            }
        }
    }
    else if (id == ID_BUTTON_KEYS_SL0_WRITE_TO_CARD)
    {
        SetStateAllButtons(false);

        // #KEYSMFSL0 WRITE[OK/FAIL]

        String message("#KEYSMFSL0 WRITE CARD %s %s %s %s %s CRC32=",
            gset.keysSL0[0].bitset.ToASCII().c_str().AsChar(),
            gset.keysSL0[1].bitset.ToASCII().c_str().AsChar(),
            gset.keysSL0[2].bitset.ToASCII().c_str().AsChar(),
            gset.keysSL0[3].bitset.ToASCII().c_str().AsChar(),
            gset.keysSL0[4].bitset.ToASCII().c_str().AsChar());

        message.Append(String("%08X", SU::CalculateCRC32(message.c_str(), message.Size())).c_str());

        Reader::Send(message.c_str());
    }
    else if (id == ID_BUTTON_KEYS_SL0_WRITE_TO_READER)
    {
        SetStateAllButtons(false);

        // #KEYSMFSL0 WRITE [OK/FAIL]

        String message("#KEYSMFSL0 WRITE READER %s %s %s %s %s CRC32=",
            gset.keysSL0[0].bitset.ToASCII().c_str().AsChar(),
            gset.keysSL0[1].bitset.ToASCII().c_str().AsChar(),
            gset.keysSL0[2].bitset.ToASCII().c_str().AsChar(),
            gset.keysSL0[3].bitset.ToASCII().c_str().AsChar(),
            gset.keysSL0[4].bitset.ToASCII().c_str().AsChar());

        message.Append(String("%08X", SU::CalculateCRC32(message.c_str(), message.Size())).c_str());

        Reader::Send(message.c_str());
    }
    else if (id == ID_BUTTON_CLOSE)
    {
        if (!keys_is_saved)
        {
            wxString message = _L("Текущие ключи будут потеряны.");
            message += "\n";
            message += _L("Если закрыть окно сейчас, то ключи не будут сохранены.");
            message += "\n";
            message += _L(" ");
            message += _L("Закрыть всё равно?");

            wxMessageDialog dlg(this, message, _L("Внимание"), wxCENTRE | wxICON_WARNING | wxCANCEL);

            if (dlg.ShowModal() == wxID_OK)
            {
                Close();
            }
        }
        else
        {
            Close();
        }
    }
}


bool KeysSL0Dialog::LoadFromFile(const wxString &file_name)
{
    wxTextFile file;

    if (!file.Open(file_name) || file.GetLineCount() != 5)
    {
        return false;
    }

    if (wxFileName(file_name).GetSize() != 170)
    {
        return false;
    }

    for (uint i = 0; i < sizeof(gset.keysSL0) / sizeof(gset.keysSL0[0]); i++)
    {
        wxString line = file.GetLine(i);

        gset.keysSL0[i].bitset.FromASCII(line.c_str().AsChar());
    }

    return true;
}


bool KeysSL0Dialog::SaveToFile(const wxString &file_name)
{
    wxTextFile file;

    if (wxFileExists(file_name))
    {
        if (!file.Open(file_name))
        {
            return false;
        }

        file.Clear();
    }
    else
    {
        if (!file.Create(file_name))
        {
            return false;
        }
    }

    for (uint i = 0; i < sizeof(gset.keysSL0) / sizeof(gset.keysSL0[0]); i++)
    {
        file.AddLine(gset.keysSL0[i].bitset.ToASCII());
    }

    file.Write();

    return true;
}


void KeysSL0Dialog::SaveCurrentKeys(BitSet128 keys[5])
{
    for (int i = 0; i < 5; i++)
    {
        keys[i] = gset.keysSL0[i].bitset;
    }
}


bool KeysSL0Dialog::KeyValuesAreTheSame(BitSet128 keys[5]) const
{
    for (int i = 0; i < 5; i++)
    {
        if (gset.keysSL0[i].bitset != keys[i])
        {
            return false;
        }
    }

    return true;
}


void KeysSL0Dialog::SetStateAllButtons(bool state)
{
    GF::FindAnyButton(this, ID_BUTTON_KEYS_SL0_MANUAL)->Enable(state);
    GF::FindAnyButton(this, ID_BUTTON_KEYS_SL0_READ_FILE)->Enable(state);
    GF::FindAnyButton(this, ID_BUTTON_KEYS_SL0_SAVE_FILE)->Enable(state);
    GF::FindAnyButton(this, ID_BUTTON_KEYS_SL0_WRITE_TO_CARD)->Enable(state);
    GF::FindAnyButton(this, ID_BUTTON_KEYS_SL0_WRITE_TO_READER)->Enable(state);
}


void KeysSL0Dialog::ShowInfoDialog(bool success)
{
    if (success)
    {
        wxMessageDialog dlg(this, _L("Ключи записаны успешно."), _L("Информфация"), wxCENTRE | wxICON_INFORMATION);

        dlg.ShowModal();
    }
    else
    {
        wxMessageDialog dlg(this, _L("При записи ключей произошла ошибка. Ключи не записаны."), _L("Ошибка"), wxCENTRE | wxICON_ERROR);

        dlg.ShowModal();
    }
}
