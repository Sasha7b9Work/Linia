// 2023/08/11 11:28:33 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Dialogs/CardInfoDialog.h"
#include "Utils/GlobalFunctions.h"
#include "Panels/Dialogs/UpgradeDialog.h"
#include "Communicator/ComPort.h"
#include "Reader/Card.h"
#include "DataBase/DataBaseCards.h"
#include "Reader/SettingsMaster.h"
#include "Reader/PasswordSender.h"
#include "Panels/Dialogs/KeysSL0Dialog.h"


CardInfoDialog *CardInfoDialog::self = nullptr;

const wxSize PanelUnknownCard::size(290, 200);
const wxSize PanelUserCard::size{290, 250};
const wxSize PanelMasterCard::size{290, 595};


wxStaticText *PanelCard::AddLine(bool append, wxBoxSizer *box, const wxString &string, const wxString &hint)
{
    wxStaticText *st = new wxStaticText(this, wxID_ANY, string);
    box->Add(st);
    box->AddSpacer(12);
    if (append)
    {
        values.push_back(st);
    }
    if (hint[0])
    {
        st->SetToolTip(hint);
    }
    return st;
}


CardInfoDialog::CardInfoDialog() :
    AutoMovedDialog(_L("Информация о карте"))
{
    self = this;

    boxUnknownCard = new wxBoxSizer(wxHORIZONTAL);
    panelUnknownCard = new PanelUnknownCard(this);
    boxUnknownCard->Add(panelUnknownCard);

    boxUserCard = new wxBoxSizer(wxHORIZONTAL);
    panelUserCard = new PanelUserCard(this);
    boxUserCard->Add(panelUserCard);

    boxMasterCard = new wxBoxSizer(wxHORIZONTAL);
    panelMasterCard = new PanelMasterCard(this);
    boxMasterCard->Add(panelMasterCard);

    Bind(wxEVT_BUTTON, &CardInfoDialog::OnEventButton, this);

    SetSizer(boxMasterCard);
    SetSizer(boxUserCard);

    Fill();
}


CardInfoDialog::~CardInfoDialog()
{
    self = nullptr;
}


void CardInfoDialog::Fill()
{
    if (self)
    {
        self->panelUnknownCard->Show(false);
        self->panelUserCard->Show(false);
        self->panelMasterCard->Show(false);

        if (Card::IsMaster())
        {
            self->panelMasterCard->Fill();

            self->panelMasterCard->Show(true);

            self->SetClientSize(PanelMasterCard::size);
        }
        else if(Card::IsUser())
        {
            self->panelUserCard->Fill();

            self->panelUserCard->Show(true);

            self->SetClientSize(PanelUserCard::size);
        }
        else if (Card::IsUnknown())
        {
            self->panelUnknownCard->Fill();

            self->panelUnknownCard->Show(true);

            self->SetClientSize(PanelUnknownCard::size);
        }
    }
}


void CardInfoDialog::OnEventButton(wxCommandEvent &event)
{
    int id = event.GetId();

    if (id == ID_BUTTON_CLOSE_INFO_CARD)
    {
        Close();
    }
    else if (id == ID_BUTTON_KEYS_SL0)
    {
        KeysSL0Dialog::self->ShowModal();
    }
}


PanelUnknownCard::PanelUnknownCard(wxWindow *window) : PanelCard(window)
{
    wxBoxSizer *box1 = new wxBoxSizer(wxHORIZONTAL);

    box1->AddSpacer(d);

    box1->Add(CreateColumn(
        false,
        _L("Тип карты") + " :",
        _L("ID") + " :"));

    box1->AddSpacer(d);

    box1->Add(CreateColumn(true));

    wxBoxSizer *box3 = new wxBoxSizer(wxVERTICAL);
    box3->AddSpacer(25);
    buttonKeys = new wxButton(this, ID_BUTTON_KEYS_SL0, _L("Дополнительно"), wxDefaultPosition, { 240, BUTTON_HEIGHT });
    box3->Add(buttonKeys);
    box3->AddSpacer(25);
    box3->Add(new wxButton(this, ID_BUTTON_CLOSE_INFO_CARD, _L("Закрыть"), wxDefaultPosition, { 240, BUTTON_HEIGHT }));
    box3->AddSpacer(25);

    wxBoxSizer *box4 = new wxBoxSizer(wxHORIZONTAL);
    box4->AddSpacer(25);
    box4->Add(box3);

    wxBoxSizer *box = new wxBoxSizer(wxVERTICAL);

    box->Add(box1);
    box->Add(box4);

    SetSizer(box);

    SetSize(size);
}


wxBoxSizer *PanelUnknownCard::CreateColumn(
    bool append,
    const wxString &string0,
    const wxString &string1)
{
    wxBoxSizer *box = new wxBoxSizer(wxVERTICAL);

    box->AddSpacer(15);

    valueTypeCard = AddLine(append, box, string0, "");
    valueID = AddLine(append, box, string1, "");

    return box;
}


void PanelUnknownCard::Fill()
{
    if (Card::IsInserted())
    {
        if (Card::IsUnknown())
        {
            valueTypeCard->SetLabel(Card::GetTypeCard());
            valueID->SetLabel(Card::GetUID().GetDoubleID());
        }
    }
    else
    {
        for (auto elem : values)
        {
            elem->SetLabel("");
        }
    }

    wxString t = Card::GetTypeCard();

    buttonKeys->Show(t.Length() > 5 &&
        t[t.Length() - 1] == '0' &&         // SL0
        t[t.Length() - 2] == 'L');
}


PanelUserCard::PanelUserCard(wxWindow *window) : PanelCard(window)
{
    wxBoxSizer *box1 = new wxBoxSizer(wxHORIZONTAL);

    box1->AddSpacer(d);

    box1->Add(CreateColumn(
        false,
        _L("Назначение") + " :",
        _L("Тип карты") + " :",
        _L("ID") + " :",
        _L("Номер") + " :",
        _L("Пароль") + " :",
        _L("Дата создания") + " :"));

    box1->AddSpacer(d);

    box1->Add(CreateColumn(true));

    wxBoxSizer *box2 = new wxBoxSizer(wxHORIZONTAL);
    box2->AddSpacer(25);

    wxBoxSizer *box3 = new wxBoxSizer(wxHORIZONTAL);
    box3->AddSpacer(25);
    box3->Add(new wxButton(this, ID_BUTTON_CLOSE_INFO_CARD, _L("Закрыть"), wxDefaultPosition, { 240, BUTTON_HEIGHT }));

    wxBoxSizer *box = new wxBoxSizer(wxVERTICAL);

    box->Add(box1);
    box->Add(box2);
    box->AddSpacer(d);
    box->Add(box3);

    SetSizer(box);

    SetSize(size);
}


PanelMasterCard::PanelMasterCard(wxWindow *window) : PanelCard(window)
{
    wxBoxSizer *box1 = new wxBoxSizer(wxHORIZONTAL);

    box1->AddSpacer(d);

    box1->Add(CreateColumn(
        false,
        _L("Назначение") + " :",        "",
        _L("Тип карты") + " :",         "",
        _L("ID") + " :",                "",
        _L("Пароль") + " :",            "",
        _L("Пароль системы") + " :",    "",
        _L("Цвет RED") + " :",          "",
        _L("Цвет GREEN") + " :",        "",
        _L("Режим ЭКО") + " :",         "",
        _L("Режим OSDP") + " :",        "",
        _L("Адрес OSDP") + " :",        "",
        _L("Скорость OSDP") + " :",     "",
        _L("Мелодия") + " Beep :",      "",
        _L("Мелодия") + " Green :",     "",
        _L("Мелодия") + " Red :",       "",
        _L("Автоном") + " :",           "",
        "WIEGAND :",                    "",
        _L("Только SL3") + " :",        _L("Hint disable less SL3"),
        _L("Датчик чувств") + " :",     "",
        _L("Датчик номер") + " :",      ""
        ));

    box1->AddSpacer(d);

    box1->Add(CreateColumn(true));

    wxBoxSizer *box2 = new wxBoxSizer(wxHORIZONTAL);
    box2->AddSpacer(25);

    wxBoxSizer *box3 = new wxBoxSizer(wxHORIZONTAL);
    box3->AddSpacer(25);
    box3->Add(new wxButton(this, ID_BUTTON_CLOSE_INFO_CARD, _L("Закрыть"), wxDefaultPosition, { 240, BUTTON_HEIGHT }));

    wxBoxSizer *box = new wxBoxSizer(wxVERTICAL);

    box->Add(box1);
    box->Add(box2);
    box->AddSpacer(10);
    box->Add(box3);

    SetSizer(box);

    SetSize(size);
}


wxBoxSizer *PanelUserCard::CreateColumn(
    bool append,
    const wxString &string0,
    const wxString &string1,
    const wxString &string2,
    const wxString &string3,
    const wxString &string4,
    const wxString &string5)
{
    wxBoxSizer *box = new wxBoxSizer(wxVERTICAL);

    box->AddSpacer(15);

    valueFunction = AddLine(append, box, string0, "");
    valueTypeCard = AddLine(append, box, string1, "");
    valueID = AddLine(append, box, string2, "");
    valueNumber = AddLine(append, box, string3, "");
    valuePassword = AddLine(append, box, string4, "");
    valueDate = AddLine(append, box, string5, "");

    return box;
}


wxBoxSizer *PanelMasterCard::CreateColumn(
    bool append,
    const wxString &function,     const wxString &hintFunction,
    const wxString &type,         const wxString &hintType,
    const wxString &ID,           const wxString &hintID,
    const wxString &password,     const wxString &hintPassword,
    const wxString &newPasword,   const wxString &hintNewPassword,
    const wxString &colorRED,     const wxString &hintColorRED,
    const wxString &colorGREEN,   const wxString &hintColorGREEN,
    const wxString &modeECO,      const wxString &hintModeECO,
    const wxString &enableOSDP,   const wxString &hintEnableOSDP,
    const wxString &addressOSDP,  const wxString &hintAddressOSDP,
    const wxString &baudRateOSDP, const wxString &hintBaudRateOSDP,
    const wxString &soundBeep,    const wxString &hintSoundBeep,
    const wxString &soundGreen,   const wxString &hintSoundGreen,
    const wxString &soundRed,     const wxString &hintSoundRed,
    const wxString &offlineMode,  const wxString &hintOfflineMode,
    const wxString &wiegand,      const wxString &hintWiegand,
    const wxString &onlySL3,      const wxString &hintOnlySL3,
    const wxString &sens,         const wxString &hintSens,
    const wxString &number,       const wxString &hintNumber
)
{
    wxBoxSizer *box = new wxBoxSizer(wxVERTICAL);

    box->AddSpacer(15);

    valueFunction = AddLine(append, box, function, hintFunction);
    valueTypeCard = AddLine(append, box, type, hintType);
    valueID = AddLine(append, box, ID, hintID);
    valuePassword = AddLine(append, box, password, hintPassword);
    valueNewPassword = AddLine(append, box, newPasword, hintNewPassword);
    valueColorRed = AddLine(append, box, colorRED, hintColorRED);
    valueColorGreen = AddLine(append, box, colorGREEN, hintColorGREEN);
    valueModeECO = AddLine(append, box, modeECO, hintModeECO);
    valueEnabledOSDP = AddLine(append, box, enableOSDP, hintEnableOSDP);
    valueAddressOSDP = AddLine(append, box, addressOSDP, hintAddressOSDP);
    valueBaudRateOSDP = AddLine(append, box, baudRateOSDP, hintBaudRateOSDP);
    valueSoundBeep = AddLine(append, box, soundBeep, hintSoundBeep);
    valueSoundGreen = AddLine(append, box, soundGreen, hintSoundGreen);
    valueSoundRed = AddLine(append, box, soundRed, hintSoundRed);
    valueOfflineMode = AddLine(append, box, offlineMode, hintOfflineMode);
    valueWiegand = AddLine(append, box, wiegand, hintWiegand);
    valueOnlySL3 = AddLine(append, box, onlySL3, hintOnlySL3);
    valueAntibreakSens = AddLine(append, box, sens, hintSens);
    valueAntibreakNumber = AddLine(append, box, number, hintNumber);

    return box;
}


void PanelUserCard::Fill()
{
    if (Card::IsInserted())
    {
        if (Card::IsUser())
        {
            CardInfo info;

            bool exist_info = CardsDB::ReadCard(Card::GetUID().GetFactory(), &info);

            valueFunction->SetLabel(_L("Пользовательская карта"));
            valueID->SetLabel(Card::GetUID().GetDoubleID());
            valueNumber->SetLabel(PasswordSender::CurrentPassword().empty() ? wxString() : wxString::Format("%llu", Card::GetNumber()));
            valuePassword->SetLabel(PasswordSender::CurrentPassword());
            valueDate->SetLabel(exist_info ? info.time : _L("Нет в БД"));
            valueTypeCard->SetLabel(Card::GetTypeCard());
        }
    }
    else
    {
        for (auto elem : values)
        {
            elem->SetLabel("");
        }
    }
}


void PanelMasterCard::Fill()
{
    if (Card::IsInserted())
    {
        if (Card::IsMaster())
        {
            CardInfo info;

            valueFunction->SetLabel(_L("Мастер-карта"));
            valueID->SetLabel(Card::GetUID().GetDoubleID());
            valuePassword->SetLabel(PasswordSender::CurrentPassword());
            valueTypeCard->SetLabel(Card::GetTypeCard());

            const SettingsMaster &set = Card::GetSettingsMaster();

            const int s = 20;

            painterRED = new Painter(this, { 105, 160 }, { s, s }, TypePainter::Color_);
            painterGREEN = new Painter(this, { 105, 189 }, { s, s }, TypePainter::Color_);

            valueNewPassword->SetLabel(wxString::Format("%llu", set.Password()));
            valueColorRed->SetLabel(ColorValue(set.ColorRed()));
            painterRED->SetColor(set.ColorRed());
            painterRED->Enable(true);
            valueColorGreen->SetLabel(ColorValue(set.ColorGreen()));
            painterGREEN->SetColor(set.ColorGreen());
            painterGREEN->Enable(true);
            valueModeECO->SetLabel(set.IsEnabledLPCD() ? _L("Вкл") : _L("Откл"));
            valueEnabledOSDP->SetLabel(set.IsEnabledOSDP() ? _L("Вкл") : _L("Откл"));
            valueAddressOSDP->SetLabel(wxString::Format("%u", set.AddressOSDP()));
            valueBaudRateOSDP->SetLabel(wxString::Format("%u", set.BaudRateOSDP().ToRAW()));
            WriteSound(valueSoundBeep, set, TypeSound::Beep);
            WriteSound(valueSoundGreen, set, TypeSound::Green);
            WriteSound(valueSoundRed, set, TypeSound::Red);
            valueOfflineMode->SetLabel(set.IsOfflineModeAldowed() ? _L("Вкл") : _L("Откл"));

            valueWiegand->SetLabel(wxString::Format("%d  %s  %s  %s  %s  %s",
                set.GetWiegandValue(),
                (set.IsWiegandFullGUID() ? _L("Да") : _L("Нет")),
                (set.IsWiegandControlBitsEnabled() ? _L("Да") : _L("Нет")),
                (set.IsWiegandControlBitsParityStandard() ? _L("Нет") : _L("Да")),
                (set.IsWiegandReverseOrderBits() ? _L("Да") : _L("Нет")),
                (set.IsWiegnadDiscard_NUID_LSB() ? _L("Да") : _L("Нет"))));

            {
                wxString hint = wxString::Format("%s : %d\n%s : %s\n%s : %s\n%s : %s\n%s : %s\n%s : %s",
                    _L("Количество передаваемых бит"), set.GetWiegandValue(),
                    _L("Передавать полный GUID"), set.IsWiegandFullGUID() ? _L("Вкл") : _L("Откл"),
                    _L("Наличие контрольных бит"), set.IsWiegandControlBitsEnabled() ? _L("Вкл") : _L("Откл"),
                    _L("Инверсия контрольных бит"), set.IsWiegandControlBitsParityStandard() ? _L("Откл") : _L("Вкл"),
                    _L("Обратный порядок бит"), set.IsWiegandReverseOrderBits() ? _L("Вкл") : _L("Откл"),
                    _L("Отбрасывать младший байт в NUID"), set.IsWiegnadDiscard_NUID_LSB() ? _L("Вкл") : _L("Откл"));

                valueWiegand->SetToolTip(hint);
            }

            valueOnlySL3->SetLabel(set.GetOnlySL3() ? _L("Да") : _L("Нет"));

            uint8 rawAntiBreak = set.GetAntibreakSensRAW();

            if (rawAntiBreak == 0)
            {
                valueAntibreakSens->SetLabel(_L("Откл"));
            }
            else if (rawAntiBreak == 1)
            {
                valueAntibreakSens->SetLabel(_L("Макс"));
            }
            else if (rawAntiBreak == 15)
            {
                valueAntibreakSens->SetLabel(_L("Мин"));
            }
            else
            {
                valueAntibreakSens->SetLabel(wxString::Format("%d", 14 - set.GetAntibreakSensRAW() + 1));
            }

            valueAntibreakNumber->SetLabel(wxString::Format("%u", set.GetAntibreakNumber()));
        }
    }
    else
    {
        for (auto elem : values)
        {
            elem->SetLabel("");
        }

        SAFE_DELETE(painterGREEN);
        SAFE_DELETE(painterRED);
    }
}


void PanelMasterCard::WriteSound(wxStaticText *text, const SettingsMaster &set, TypeSound::E type)
{
    if (set.Volume(type) == 0)
    {
        text->SetLabel(_L("Откл"));
    }
    else
    {
        text->SetLabel(_L("Номер") + wxString::Format(": %d, ", set.Melody(type) + 1) + _L("Громкость") + wxString::Format(": %d", set.Volume(type)));
    }
}


wxString PanelMasterCard::ColorValue(const Color &color)
{
    return wxString::Format("R: %2.1f  G: %2.1f  B: %2.1f     %2.1f",
        (float)color.GetRed() / 255.0f,
        (float)color.GetGreen() / 255.0f,
        (float)color.GetBlue() / 255.0f,
        color.GetBrightnessF()
    );
}
