// 2023/08/09 11:05:55 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Dialogs/PasswordDialog.h"
#include "Controls/TextControls.h"
#include "Controls/Painter.h"
#include "Utils/Configurator.h"
#include "Utils/GlobalFunctions.h"
#include "DataBase/DataBasePasswords.h"
#include "Reader/Reader.h"


class EyePassword : public Painter
{
public:
    EyePassword(wxWindow *parent, wxPoint position, wxSize size) :
        Painter(parent, position, size, TypePainter::Eye)
    {
        Bind(wxEVT_LEFT_DOWN, &EyePassword::OnMouseEvent, this);
        Bind(wxEVT_LEFT_UP, &EyePassword::OnMouseEvent, this);
    }
private:
    void OnMouseEvent(wxMouseEvent &event)
    {
        if (event.LeftUp())
        {
            opened = !opened;

            ((PasswordDialog *)GetParent())->OnMouseEye(opened);

            Refresh();
        }
    }

    virtual void OnPaint(wxPaintEvent &) wxOVERRIDE
    {
        wxPaintDC dc(this);
        wxBitmap bitmap_eye(opened ? "BITMAP_EYE_VISIBLE" : "BITMAP_EYE_NOT_VISIBLE");
        wxImage image = bitmap_eye.ConvertToImage();
        image.SetMaskColour(255, 255, 255);
        image = image.Rescale(GetSize().x, GetSize().y);
        dc.DrawBitmap(wxBitmap(image), 0, 0, true);
    }

    bool opened = false;
};


PasswordDialog::PasswordDialog(Password *_password, const wxString &title, bool warning, Type::E type) :
    AutoMovedDialog(title),
    password(_password)
{
    if (warning)
    {
        SetBackgroundColour(*wxRED);
    }

    wxSize size_button{ 100, BUTTON_HEIGHT };

    wxBoxSizer *box_vertical = new wxBoxSizer(wxVERTICAL);

    if (type == Type::ThreeFields)
    {
        box_vertical->AddSpacer(20);
        txt_password_factory = new TextCtrlPassword(this, ID_TEXTCNTRL_PASSWORD_FACTORY);
        box_vertical->Add(txt_password_factory);
        txt_password_factory->SetLabelText(_L("Заводской пароль"));
        txt_password_factory->SetEditable(false);

        new EyePassword(this, { 120, 23 }, { 15, 15 });
    }

    box_vertical->AddSpacer(20);
    txt_password_one = new TextCtrlPassword(this, ID_TEXTCNTRL_PASSWORD_ONE);
    txt_password_one->Bind(wxEVT_TEXT_PASTE, &PasswordDialog::OnEventTextPaste, this);
    box_vertical->Add(txt_password_one);
    box_vertical->AddSpacer(20);
    txt_password_two = new TextCtrlPassword(this, ID_TEXTCNTRL_PASSWORD_TWO);
    txt_password_two->Bind(wxEVT_TEXT_PASTE, &PasswordDialog::OnEventTextPaste, this);
    box_vertical->Add(txt_password_two);
    box_vertical->AddSpacer(20);
    wxButton *button = new wxButton(this, ID_BUTTON_APPLY_PASSWORD, _L("Установить"), wxDefaultPosition, size_button);
    if (warning)
    {
        button->SetBackgroundColour(*wxRED);
    }
    box_vertical->Add(button);

    wxBoxSizer *box = new wxBoxSizer(wxHORIZONTAL);
    box->AddSpacer(20);
    box->Add(box_vertical);

    SetSizer(box);

    SetFixedSize({ 20 * 2 + FindWindow(ID_TEXTCNTRL_PASSWORD_ONE)->GetSize().x, 20 * 4 + FindWindow(ID_TEXTCNTRL_PASSWORD_ONE)->GetSize().y * 2 + FindWindow(ID_BUTTON_APPLY_PASSWORD)->GetSize().y + ((type == Type::ThreeFields) ? 40 : 0) });

    Bind(wxEVT_BUTTON, &PasswordDialog::OnEventButton, this);

    txt_password_one->SetUInt64(password->value);
    txt_password_two->SetUInt64(password->value);

    FindWindow(ID_BUTTON_APPLY_PASSWORD)->SetFocus();

    MoveToMousePosition();
}


void PasswordDialog::OnEventButton(wxCommandEvent &event)
{
    int id = event.GetId();

    if (id == ID_BUTTON_APPLY_PASSWORD)
    {
        uint64 value1 = 0;
        uint64 value2 = 0;

        txt_password_one->GetLineText(0).ToULongLong(&value1);
        txt_password_two->GetLineText(0).ToULongLong(&value2);

        if (value1 == value2)
        {
            password->value = value1;

            Config::WriteUInt64(password->key, password->value);

            if (password == &gset.password_card)
            {
                PasswDB::Append(password->value);
            }

            Close();
        }
        else
        {
            wxMessageDialog dlg(this, _L("Пароли не совпадают."), _L("Ошибка"), wxCENTRE | wxICON_ERROR);

            dlg.ShowModal();

            txt_password_one->SetFocus();
        }
    }
}


void PasswordDialog::OnMouseEye(bool open)
{
    if (open)
    {
        txt_password_factory->SetUInt64(password_factory.value);
    }
    else
    {
        txt_password_factory->SetLabelText(_L("Заводской пароль"));
    }
}


void PasswordDialog::OnEventTextPaste(wxCommandEvent &)
{
}
