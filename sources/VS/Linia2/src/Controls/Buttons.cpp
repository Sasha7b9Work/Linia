// 2023/09/02 11:37:13 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Controls/Buttons.h"
#include "Controls/Painter.h"
#include "Utils/GlobalFunctions.h"
#include "Controls/Bitmap.h"
#include "Utils/SystemDepend.h"
#include "Application.h"

wxDEFINE_EVENT(EVT_BUTTON_BITMAP_CHOICE, ButtonBitmapChoiceEvent);




ButtonColor::ButtonColor(wxWindow *parent, int id, const wxString &title, wxPoint position, wxSize size, PainterRect *painter) :
    wxButton(parent, id, title, position, size),
    m_painter(painter)
{
    m_painter->Bind(wxEVT_LEFT_UP, &ButtonColor::OnMouseEvent, this);
}


void ButtonColor::SetColor(const Color &color)
{
    m_painter->SetColor(color);
}


void ButtonColor::OnMouseEvent(wxMouseEvent &event)
{
    if (event.IsButton())
    {
        GF::SendCommandEvent(this, wxEVT_BUTTON, 0);
    }

    event.Skip();
}


CheckButton::CheckButton(wxWindow *parent, const wxString &title, const wxPoint &pos, int width) :
    wxPanel(parent, wxID_ANY, pos)
{
    wxSize size{ width, TEXTCNTRL_HEIGHT + 3 };
    wxPanel::SetSize(size);

    button = new wxToggleButton(this, wxID_ANY, ' ' + title, { 0, 0 }, {size.x - 20, size.y}, wxBU_LEFT);

    button->SetBackgroundColour(button->GetBackgroundColour().ChangeLightness(LIGHTNESS));

#ifdef WIN32
    int y = 4;
#else
    int y = 2;
#endif

    ch_box = new wxCheckBox(this, wxID_ANY, "", { size.x - 20 + 5, y });

    Bind(wxEVT_CHECKBOX, &CheckButton::OnEventCheckBox, this);
    Bind(wxEVT_TOGGLEBUTTON, &CheckButton::OnEventToggleButton, this);
}


void CheckButton::OnEventCheckBox(wxCommandEvent &event)
{
    if (event.GetId() == ch_box->GetId())
    {
        button->SetValue(event.GetInt() != 0);

        SendEvent();
    }

    event.Skip();
}


void CheckButton::OnEventToggleButton(wxCommandEvent &event)
{
    if (event.GetId() == button->GetId())
    {
        ch_box->SetValue(button->GetValue());

        SendEvent();
    }

    event.Skip();
}


void CheckButton::SendEvent()
{
    GF::SendCommandEvent(this, wxEVT_TOGGLEBUTTON, button->GetValue() ? 1 : 0);
}


bool CheckButton::GetValue() const
{
    return button->GetValue();
}


void CheckButton::SetValue(bool value)
{
    button->SetValue(value);
    ch_box->SetValue(value);
}


void CheckButton::SetToolTip(const wxString &tool_tip)
{
    wxPanel::SetToolTip(tool_tip);
    button->SetToolTip(tool_tip);
    ch_box->SetToolTip(tool_tip);
}


ButtonBitmap::ButtonBitmap(wxWindow *parent, const wxPoint &pos, const wxSize &size, const wxString &file_bitmap, const wxString &name) :
    wxBitmapButton(parent, wxID_ANY, wxNullBitmap, pos, size, wxBU_EXACTFIT)
{
    wxBitmapButton::SetName(parent->GetName() + "_" + name);

    LOG_WRITE(wxBitmapButton::GetName().ToStdString().c_str());

    SetFileBitmap(file_bitmap);

    SetClientSize(((size == wxDefaultSize) ? bitmap.GetBitmap().GetSize() : size ) + wxSize(10, 10));

    wxBitmapButton::Update();

    wxBitmapButton::Refresh();

    wxBitmapButton::SetBackgroundColour(GetBackgroundColour().ChangeLightness(LIGHTNESS));
}


void ButtonBitmap::SetFileBitmap(const wxString &file_bitmap)
{
    bitmap = Bitmap::Get(file_bitmap);

    SetBitmap(bitmap.GetBitmap());
}


ButtonBitmapChoice::ButtonBitmapChoice(wxWindow *parent, const wxPoint &pos, const wxSize &size, const wxArrayString &_files, const wxString &_name) :
    ButtonBitmap(parent, pos, size, _files[0], _name),
    files(_files)
{
    Bind(wxEVT_BUTTON, &ButtonBitmapChoice::OnEventButton, this);
}


void ButtonBitmapChoice::OnEventButton(wxCommandEvent &event)
{
    if (event.GetId() == GetId())
    {
        choice++;

        if (choice >= (int)files.GetCount())
        {
            choice = 0;
        }

        SetCurrentValue(choice);

        GF::SendCommandEvent(this, EVT_BUTTON_BITMAP_CHOICE, choice);
    }

    event.Skip();
}


void ButtonBitmapChoice::SetCurrentValue(int value)
{
    if (value < (int)files.GetCount())
    {
        choice = value;

        SetFileBitmap(files[(size_t)choice]);
    }
    else
    {
        LOG_ERROR("Bad index");
    }
}


int ButtonBitmapChoice::GetCurrentValue() const
{
    return choice;
}
