// 2025/8/9 11:41:08 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Controls/BmpButtonsCombo.h"


BmpButtonsCombo::BmpButtonsCombo(wxWindow *parent, const wxPoint &pos, const wxSize &size, const wxString &file_bitmap) :
    wxButton(parent, wxID_ANY, "", pos)
{
    Bind(wxEVT_PAINT, &BmpButtonsCombo::OnEventPaint, this);
    Bind(wxEVT_LEFT_DOWN, &BmpButtonsCombo::OnLeftDown, this);
    Bind(wxEVT_LEFT_UP, &BmpButtonsCombo::OnLeftUp, this);
    Bind(wxEVT_LEAVE_WINDOW, &BmpButtonsCombo::OnLeaveWindow, this);

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

    SetClientSize(size + wxSize(20, 20));
    Update();

    Refresh();
}


void BmpButtonsCombo::OnEventPaint(wxPaintEvent &event)
{
    wxPaintDC dc(this);

    int flags = 0;
    if (left_is_down) flags |= wxCONTROL_PRESSED;
    if (!IsEnabled()) flags |= wxCONTROL_DISABLED;

    // 1. Рисуем стандартный фон кнопки
    wxRendererNative::Get().DrawPushButton(this, dc, GetClientRect(), flags);

    // 2. Центрируем и рисуем битмап
    if (bitmap.IsOk())
    {
        wxSize btnSize = GetClientSize();
        wxSize bmpSize = bitmap.GetSize();

        int x = (btnSize.x - bmpSize.x) / 2;
        int y = (btnSize.y - bmpSize.y) / 2;

        dc.DrawBitmap(bitmap, x, y, true);
    }

//    event.Skip(false); // Важно для корректной работы
}


void BmpButtonsCombo::OnLeftDown(wxMouseEvent &event)
{
    left_is_down = true;

    Refresh();

    event.Skip();
}


void BmpButtonsCombo::OnLeftUp(wxMouseEvent &event)
{
    left_is_down = true;

    Refresh();

    event.Skip();
}


void BmpButtonsCombo::OnLeaveWindow(wxMouseEvent &event)
{
    left_is_down = false;

    Refresh();

    event.Skip();
}
