// 2023/09/02 11:37:13 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Controls/Buttons.h"
#include "Controls/PainterRect.h"
#include "Utils/GlobalFunctions.h"


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
        GF::SendCommandEvent(GetParent(), wxEVT_BUTTON, GetId());
    }

    event.Skip();
}


CheckButton::CheckButton(wxWindow *parent, int id, const wxString &title, const wxPoint &pos, const wxSize &size) :
    wxPanel(parent, id, pos, size)
{
    button = new wxToggleButton(this, id + delta_id, title, { 0, 0 }, { size.x - 20, size.y });

    ch_box = new wxCheckBox(this, id + delta_id + 1, "", { size.x - 20 + 5, 4 });

    Bind(wxEVT_CHECKBOX, &CheckButton::OnEventCheckBox, this);
    Bind(wxEVT_TOGGLEBUTTON, &CheckButton::OnEventToggleButton, this);
}


void CheckButton::OnEventCheckBox(wxCommandEvent &event)
{
    if (event.GetId() == ch_box->GetId())
    {
        button->SetValue(event.GetInt() != 0);

        GF::SendCommandEvent(GetParent(), wxEVT_TOGGLEBUTTON, button->GetId(), button->GetValue() ? 1 : 0);

        event.Skip();
    }
}


void CheckButton::OnEventToggleButton(wxCommandEvent &event)
{
    if (event.GetId() == button->GetId())
    {
        ch_box->SetValue(button->GetValue());

        event.Skip();
    }
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


ButtonBitmap::ButtonBitmap(wxWindow *parent, const wxPoint &pos, const wxSize &size, const wxString &file_bitmap) :
    wxBitmapButton(parent, wxID_ANY, wxNullBitmap, pos)
{
    SetFileBitmap(file_bitmap);

    SetClientSize(((size == wxDefaultSize) ? bitmap.GetSize() : size ) + wxSize(10, 10));

    Update();

    Refresh();
}


void ButtonBitmap::CreateBitmap(wxBitmap &bmap, const wxString &file_bitmap)
{
    wxString path = wxString("resources/") + file_bitmap;

    if (file_bitmap[file_bitmap.size() - 1] == 'p')
    {
        wxImage image;

        if (!image.LoadFile(path, wxBITMAP_TYPE_BMP))
        {
            LOG_ERROR("Не удалось загрузить файл изображения %s", file_bitmap.c_str().AsChar());
        }

        bmap = wxBitmap(image);

        bmap.SetMask(new wxMask(bmap, *wxWHITE));
    }
    else
    {
        wxIcon icon;

        if (!icon.LoadFile(path, wxBITMAP_TYPE_ICO))
        {
            LOG_ERROR("Не удалось загрузить файл изображения %s", file_bitmap.c_str().AsChar());
        }

        bmap.CopyFromIcon(icon);
    }
}


void ButtonBitmap::SetFileBitmap(const wxString &file_bitmap)
{
    CreateBitmap(bitmap, file_bitmap);

    SetBitmap(bitmap);
}
