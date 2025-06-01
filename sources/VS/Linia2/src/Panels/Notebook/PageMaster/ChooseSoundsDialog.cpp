// 2023/11/16 15:48:24 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Reader/Reader.h"
#include "Utils/Configurator.h"
#include "Utils/GlobalFunctions.h"
#include "Settings/Settings.h"
#include "Utils/SystemDepend.h"
#include "Panels/Notebook/PageMaster/ChooseSoundsDialog.h"


ChooseSoundsDialog *ChooseSoundsDialog::self = nullptr;


void ChooseSoundsDialog::Create()
{
    self = new ChooseSoundsDialog();
}


void ChooseSoundsDialog::Delete()
{
    SAFE_DELETE(self);
}


int ChooseSoundsDialog::ShowModal()
{
    return AutoMovedDialog::ShowModal();
}


ChooseSoundsDialog::ChooseSoundsDialog() :
    AutoMovedDialog(_L("Выбор звуков"))
{
    int x0 = 20;
    int y0 = 30;

    new wxStaticText(this, wxID_ANY, _L("Тип"), { 120, 10 });

    new wxStaticText(this, wxID_ANY, _L("Громкость"), { 200, 10 });

    for (int i = 0; i < 3; i++)
    {
        CreateMelody(x0, y0 + i * SD::D::ChooseSound::d(), i);
    }

    new wxButton(this, wxID_CANCEL, _L_CLOSE, { 35, SD::D::ChooseSound::y() }, BUTTON_SIZE_SMALL);

    new wxButton(this, wxID_OK, _L_SAVE, { 160, SD::D::ChooseSound::y() }, BUTTON_SIZE_SMALL);

    Bind(wxEVT_BUTTON, &ChooseSoundsDialog::OnEventButton, this);

    AutoMovedDialog::SetFixedSize(SD::D::ChooseSound::Size());

    MoveToMousePosition();
}


void ChooseSoundsDialog::CreateMelody(int x, int y, int number)
{
    wxArrayString choices;
    for (int i = 0; i < 10; i++)
    {
        choices.Add(wxString::Format("# %d", i + 1));
    }

    static const pchar types[3] =
    {
        "Beep",
        "Green",
        "Red"
    };

    new wxStaticText(this, wxID_ANY, wxString::Format("%s %s", _L("Мелодия"), types[number]), {x, y});

    (new wxComboBox(this, ID_COMBOBOX_MELODY + number, choices[(uint)SET::MASTER::melody[number].GetIndex()],
        SD::D::ChooseSound::PosCombo(x, y), SD::D::ChooseSound::SizeCombo(), choices, wxCB_READONLY))->Bind(wxEVT_COMBOBOX, &ChooseSoundsDialog::OnEventComboBox, this);

    wxSpinCtrl *spin = new wxSpinCtrl(this, ID_SPINCNTRL_VOLUME + number, SET::MASTER::volume[number].ToString(), SD::D::ChooseSound::PosSpin(x, y), SD::D::ChooseSound::SizeSpin(), wxSP_ARROW_KEYS, 0, 3);

    spin->Bind(wxEVT_SPINCTRL, &ChooseSoundsDialog::OnEventSpinCtrl, this);
} //-V773


void ChooseSoundsDialog::OnEventComboBox(wxCommandEvent &event)
{
    Play(event.GetId() - ID_COMBOBOX_MELODY);
}


void ChooseSoundsDialog::OnEventSpinCtrl(wxCommandEvent &event)
{
    Play(event.GetId() - ID_SPINCNTRL_VOLUME);
}


void ChooseSoundsDialog::OnEventButton(wxCommandEvent &event)
{
    if (event.GetId() == wxID_OK)
    {
        for (int i = 0; i < 3; i++)
        {
            SET::MASTER::melody[i].SetFromControl();
            SET::MASTER::volume[i].SetFromControl();
        }
    }
    else if (event.GetId() == wxID_CANCEL)
    {
        for (int i = 0; i < 3; i++)
        {
            SET::MASTER::melody[i].LoadToControl();
            SET::MASTER::volume[i].LoadToControl();
        }
    }

    event.Skip();
}


void ChooseSoundsDialog::Play(int number)
{
    int index_melody = FINC_COMBOBOX(ID_COMBOBOX_MELODY + number)->GetCurrentSelection();
    int index_volume = FIND_SPINCTRL(ID_SPINCNTRL_VOLUME + number)->GetValue();

    Reader::Send(wxString::Format("#SOUND PLAY NUMBER=%d VOLUME=%d", index_melody, index_volume));
}


bool ChooseSoundsDialog::CurrentSettingsEqualFactory() const
{
    for (int i = 0; i < 3; i++)
    {
        if (!SET::MASTER::melody[i].ValueIsDefault() ||
            !SET::MASTER::volume[i].ValueIsDefault()
            )
        {
            return false;
        }
    }

    return true;
}


void ChooseSoundsDialog::ResetToFactory(bool factory)
{
    for (int i = 0; i < 3; i++)
    {
        SET::MASTER::melody[i].ResetToFactory(factory);
        SET::MASTER::volume[i].ResetToFactory(factory);
    }
}
