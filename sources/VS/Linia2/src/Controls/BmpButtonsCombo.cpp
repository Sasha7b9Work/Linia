// 2025/8/9 11:41:08 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Controls/BmpButtonsCombo.h"


class ButtonPopup : public wxPopupTransientWindow
{
public:
    ButtonPopup(wxWindow *parent) : wxPopupTransientWindow(parent)
    {
        wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);

        for (int i = 1; i <= 3; ++i)
        {
            wxButton *btn = new wxButton(this, wxID_ANY, wxString::Format("Option %d", i));
            btn->Bind(wxEVT_BUTTON, &ButtonPopup::OnButtonClick, this);
            sizer->Add(btn, 0, wxEXPAND | wxALL, 5);
        }

        SetSizer(sizer);
        Fit();
    }

private:
    void OnButtonClick(wxCommandEvent &event)
    {
        wxMessageBox("Выбрано: " + ((wxButton *)event.GetEventObject())->GetLabel());
        Dismiss();
    }
};


BmpButtonsCombo::BmpButtonsCombo(wxWindow *parent, const wxPoint &pos, const wxSize &size, const wxString &file_bitmap) :
    wxBitmapButton(parent, wxID_ANY, wxNullBitmap, pos)
{
    Bind(wxEVT_BUTTON, &BmpButtonsCombo::OnButtonClicked, this);

    wxString path = wxString("resources/") + file_bitmap;

    if (file_bitmap[file_bitmap.size() - 1] == 'p')
    {
        wxImage image;

        if (!image.LoadFile(path, wxBITMAP_TYPE_BMP))
        {
            LOG_ERROR("Не удалось загрузить файл изображения %s", file_bitmap.c_str().AsChar());
        }

        bitmap = wxBitmap(image);
    }
    else
    {
        wxIcon icon;

        if (!icon.LoadFile(path, wxBITMAP_TYPE_ICO))
        {
            LOG_ERROR("Не удалось загрузить файл изображения %s", file_bitmap.c_str().AsChar());
        }

        bitmap.CopyFromIcon(icon);
    }

    bitmap.SetMask(new wxMask(bitmap, *wxWHITE));

    SetBitmap(bitmap);

    SetClientSize(size + wxSize(2, 2));
    Update();

    Refresh();
}


void BmpButtonsCombo::OnButtonClicked(wxCommandEvent &event)
{
    ButtonPopup *popup = new ButtonPopup(this);
    wxPoint pos = ClientToScreen(wxPoint(-50, -50));
    popup->Position(pos, wxSize(0, 0));
    popup->Popup();
}
