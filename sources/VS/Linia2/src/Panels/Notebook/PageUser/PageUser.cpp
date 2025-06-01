// 2023/07/04 17:52:16 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Dialogs/PasswordDialog.h"
#include "Settings/Settings.h"
#include "Utils/GlobalFunctions.h"
#include "Utils/Configurator.h"
#include "Reader/Reader.h"
#include "Utils/GlobalFunctions.h"
#include "Panels/ConsoleRS232.h"
#include "DataBase/DataBaseCards.h"
#include "Utils/StringUtils.h"
#include "Reader/Indicator.h"
#include "Panels/Notebook/PageUser/PageUser.h"
#include <vector>
#include <algorithm>


PageUser *PageUser::self = nullptr;


PageUser::PageUser(wxNotebook *parent) : Page(parent, _L("Карты доступа"))
{
    self = this;

    int y = FIRST_X + 15;

    int BOX_X = FIRST_X;

    wxColor col_back = GetBackgroundColour();

    wxStaticBox *boxGeneration = new wxStaticBox(this, wxID_ANY, _L("Генерация номера карты"), { BOX_X, FIRST_X }, { WIDTH_BOX, 140 });

    {
        wxArrayString choices;
        choices.Add(_L("По возрастанию"));
        choices.Add(_L("По убыванию"));

        y += 2;

        new wxStaticText(boxGeneration, wxID_ANY, _L("Диапазон генерации"), { FIRST_X, Y(y)});

        y += 25;

        new wxCheckBox(boxGeneration, ID_CHECKBOX_RANGE, "", { FIRST_X, Y(y + 3) });

        int width_text = 70;

        (new TextCtrlNumbersLimits(boxGeneration, ID_TEXTCNTRL_NUMBER_FIRST, SET::USER::number_first.Get(), 1, 0xFFFFFF,
            { FIRST_X + 20, Y(y) }, { width_text, TEXTCNTRL_HEIGHT }))->Bind(wxEVT_KILL_FOCUS, &PageUser::OnFocusEvent, this);

        (new TextCtrlNumbersLimits(boxGeneration, ID_TEXTCNTRL_NUMBER_LAST, SET::USER::number_last.Get(), 1, 0xFFFFFF,
            { FIRST_X + 20 + width_text + 7, Y(y) }, { width_text, TEXTCNTRL_HEIGHT }))->Bind(wxEVT_KILL_FOCUS, &PageUser::OnFocusEvent, this);

        y += 35;

        new wxStaticText(boxGeneration, wxID_ANY, _L("Номер следующей карты"), { FIRST_X, Y(y) });

        y += 20;

        (new TextCtrlNumbersLimits(boxGeneration, ID_TEXTCNTRL_NUMBER_NEXT, SET::USER::number_next.Get(), 1, 0xFFFFFF,
            { FIRST_X, Y(y) }, { width_text, TEXTCNTRL_HEIGHT }))->Bind(wxEVT_CHAR, &PageUser::OnKeyEvent, this);
    }

    progress.painter = new PainterProgress(this, { X_0, 180 }, { WIDTH_BOX, 20 });

    {
        wxPanel *panel = new wxPanel(this, wxID_ANY, { BOX_X - 5, 285 }, { WIDTH_BOX + 10, 155 });

        panel->SetBackgroundColour(*wxRED);

        wxStaticBox *boxWrite = new wxStaticBox(panel, wxID_ANY, _L("Запись на карту"), { 5, 5 }, { WIDTH_BOX, 145 });

        boxWrite->SetBackgroundColour(col_back);

        (new wxCheckBox(boxWrite, ID_CHECKBOX_WRITE_CLIENT_PASSWORD, _L("Действующего пароля"), { 10, Y_0 + 10 }))->SetValue(true);

        new wxCheckBox(boxWrite, ID_CHECKBOX_WRITE_CLIENT_NEW_NUMBER, _L("Нового номера карты"), { 10, Y_1 });

        (new wxButton(boxWrite, ID_BUTTON_WRITE_USER_CARD, _L("Записать на карту"), { X_0, Y_2 - 5 }, BUTTON_SIZE))->Enable(false);
    }

    wxStaticBox *boxNumbers = new wxStaticBox(this, ID_STATICBOX_NUMBERS_CARDS, _L("Созданные номера карт"), { 200, 10 }, { WIDTH_BOX, 430 });

    {
        list_cards = new ListCards(boxNumbers, wxID_ANY, { 1, Y(20) }, { WIDTH_BOX - 2, 438 }, wxLC_REPORT | wxNO_BORDER);
    }

    Bind(wxEVT_BUTTON, &PageUser::OnEventButton, this);
    Bind(wxEVT_CHECKBOX, &PageUser::OnEventCheckBox, this);
    Bind(wxEVT_TEXT, &PageUser::OnEventText, this);
}


void PageUser::OnEventButton(wxCommandEvent &event)
{
    int id = event.GetId();

    if (id == ID_BUTTON_WRITE_USER_CARD)
    {
        Indicator::Task();

        bool write_new_card = false;

        if (CardsDB::ExistUID(Card::GetUID()))
        {
            if (SET::USER::write_client_number.Get())
            {
                wxMessageDialog dlg(this,
                    _L("Карта с таким UID уже есть в базе данных.") + "\n" + _L("Перезаписать?"),
                    _L("Внимание!!!"), wxCENTRE | wxICON_WARNING | wxCANCEL);

                if (dlg.ShowModal() != wxID_OK)
                {
                    return;
                }
            }
        }
        else
        {
            if (!SET::USER::write_client_number.Get())
            {
                wxMessageDialog dlg(this,
                    _L("Карты нет в базе данных.") + "\n" + _L("В неё будет записан новый номер."),
                    _L("Внимание!!!"), wxCENTRE | wxICON_WARNING | wxCANCEL);

                if (dlg.ShowModal() != wxID_OK)
                {
                    return;
                }

                write_new_card = true;
            }
        }

        FindWindow(ID_BUTTON_WRITE_USER_CARD)->Enable(false);

        if (SET::USER::write_client_number.Get() || write_new_card)
        {
            wxString str_number = FINC_TEXTCTRL(ID_TEXTCNTRL_NUMBER_NEXT)->GetValue();

            uint64 number = 0;

            str_number.ToULongLong(&number);

            if (CardsDB::ExistNumber(number))
            {
                wxMessageDialog dlg(this,
                    _L("Карта с номером") + wxString::Format(" %llu ", number) + _L("уже есть в базе данных.") + "\n" + _L("Записать ещё одну?"),
                    _L("Внимание!!!"), wxCENTRE | wxICON_WARNING | wxCANCEL);

                if (dlg.ShowModal() != wxID_OK)
                {
                    return;
                }
            }

            String message("#MAKE USER NUMBER=%s PWD=%s CRC32=",
                str_number.c_str().AsChar(),
                gset.password_current.ToString().c_str().AsChar());

            message.Append(String("%08X", SU::CalculateCRC32(message.c_str(), message.Size())).c_str());

            Reader::Send(message.c_str());
        }
        else
        {
            String message("#MAKE USER PWD=%s CRC32=", gset.password_current.ToString().c_str().AsChar());

            message.Append(String("%08X", SU::CalculateCRC32(message.c_str(), message.Size())).c_str());

            Reader::Send(message.c_str());
        }
    }
}


void PageUser::OnKeyEvent(wxKeyEvent &event)
{
    if (event.GetId() == ID_TEXTCNTRL_NUMBER_NEXT)
    {
        int code = event.GetKeyCode();

        if (code >= 0x30 && code <= 0x39)
        {
            FIND_CHECKBOX(ID_CHECKBOX_RANGE)->SetValue(false);
            FindWindow(ID_TEXTCNTRL_NUMBER_FIRST)->Enable(false);
            FindWindow(ID_TEXTCNTRL_NUMBER_LAST)->Enable(false);
            SET::USER::enabled_range_generation.SetFromControl();
        }
    }

    event.Skip();
}


void PageUser::OnEventEnterPage()
{
    ((TextCtrlNumbersLimits *)GF::FindTextCtrl(this, ID_TEXTCNTRL_NUMBER_FIRST))->LimitValue();
    ((TextCtrlNumbersLimits *)GF::FindTextCtrl(this, ID_TEXTCNTRL_NUMBER_LAST))->LimitValue();
    ((TextCtrlNumbersLimits *)GF::FindTextCtrl(this, ID_TEXTCNTRL_NUMBER_NEXT))->LimitValue();
}


void PageUser::OnFocusEvent(wxFocusEvent &event)
{
    int id = event.GetId();

    int first = GF::IntFromTextCtrl(this, ID_TEXTCNTRL_NUMBER_FIRST);
    int last = GF::IntFromTextCtrl(this, ID_TEXTCNTRL_NUMBER_LAST);
    int next = GF::IntFromTextCtrl(this, ID_TEXTCNTRL_NUMBER_NEXT);

    if (id == ID_TEXTCNTRL_NUMBER_FIRST)
    {
        if (first > last)
        {
            GF::IntToTextCtrl(this, ID_TEXTCNTRL_NUMBER_LAST, first);
        }

        if (next < first)
        {
            GF::IntToTextCtrl(this, ID_TEXTCNTRL_NUMBER_NEXT, first);
        }
    }
    else if (id == ID_TEXTCNTRL_NUMBER_LAST)
    {
        if (last < first)
        {
            GF::IntToTextCtrl(this, ID_TEXTCNTRL_NUMBER_FIRST, last);
        }

        if (next > last)
        {
            GF::IntToTextCtrl(this, ID_TEXTCNTRL_NUMBER_NEXT, last);
        }
    }
    else if (id == ID_TEXTCNTRL_NUMBER_NEXT)
    {
        if (FIND_CHECKBOX(ID_CHECKBOX_RANGE)->IsChecked())
        {
            if (next < first)
            {
                GF::IntToTextCtrl(this, ID_TEXTCNTRL_NUMBER_NEXT, first);
            }
            if (next > last)
            {
                GF::IntToTextCtrl(this, ID_TEXTCNTRL_NUMBER_NEXT, last);
            }
        }
    }

    event.Skip();
}


void PageUser::OnEventCheckBox(wxCommandEvent &event)
{
    int id = event.GetId();

    bool checked = event.IsChecked();

    if (id == ID_CHECKBOX_WRITE_CLIENT_PASSWORD)
    {
        if (!event.IsChecked())
        {
            FIND_CHECKBOX(ID_CHECKBOX_WRITE_CLIENT_PASSWORD)->SetValue(true);
        }
    }
    else if (id == ID_CHECKBOX_RANGE)
    {
        SET::USER::enabled_range_generation.SetFromControl();

        FindWindow(ID_TEXTCNTRL_NUMBER_FIRST)->Enable(checked);
        FindWindow(ID_TEXTCNTRL_NUMBER_LAST)->Enable(checked);

        if (checked)
        {
            wxString _text = FINC_TEXTCTRL(ID_TEXTCNTRL_NUMBER_FIRST)->GetLineText(0);
            FINC_TEXTCTRL(ID_TEXTCNTRL_NUMBER_NEXT)->SetLabelText(_text);
        }
    }
    else if (id == ID_CHECKBOX_WRITE_CLIENT_NEW_NUMBER)
    {
        SET::USER::write_client_number.SetFromControl();
    }
}


void PageUser::SetNextCardNumber()
{
    uint64 new_number = GF::UInt64FromTextCtrl(this, ID_TEXTCNTRL_NUMBER_NEXT) + 1;

    wxString new_value = wxString::Format("%llu", new_number);

    FINC_TEXTCTRL(ID_TEXTCNTRL_NUMBER_NEXT)->SetValue(new_value);
}


void PageUser::OnEventText(wxCommandEvent &event)
{
    event.Skip();

    int id = event.GetId();

    if (id == ID_TEXTCNTRL_NUMBER_FIRST)
    {
        SET::USER::number_first.SetFromControl();
    }
    else if (id == ID_TEXTCNTRL_NUMBER_NEXT)
    {
        SET::USER::number_next.SetFromControl();
    }
    else if (id == ID_TEXTCNTRL_NUMBER_LAST)
    {
        SET::USER::number_last.SetFromControl();
    }
}


PainterProgress::PainterProgress(wxWindow *parent, wxPoint position, wxSize size) :
    Painter(parent, position, size, TypePainter::Count)
{
    bitmap = new wxBitmap(size.x, size.y);
}


void PainterProgress::OnPaint(wxPaintEvent &)
{
    wxPaintDC dc(this);
    dc.DrawBitmap(*bitmap, 0, 0);
}


void PainterProgress::SetValue(float)
{

}


void PageUser::Progress::ResetAndShow()
{

}


void PageUser::Progress::Update(float)
{

}


void PageUser::Progress::Hide()
{
    painter->Hide();
}
