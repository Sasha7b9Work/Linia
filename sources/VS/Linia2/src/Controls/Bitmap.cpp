// 2025/08/10 13:41:02 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Controls/Bitmap.h"


std::map<wxString, Bitmap> Bitmap::bitmaps;


Bitmap::Bitmap(const wxString &file_bitmap)
{
    wxString path = wxString("resources/") + file_bitmap;

    if (file_bitmap[file_bitmap.size() - 1] == 'p')
    {
        wxImage image;

        if (!image.LoadFile(path, wxBITMAP_TYPE_BMP))
        {
            LOG_ERROR("Не удалось загрузить файл изображения %s", file_bitmap.c_str().AsChar());
        }

        bitmap = wxBitmap(image);

        bitmap.SetMask(new wxMask(bitmap, *wxWHITE));
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
}


Bitmap Bitmap::Get(const wxString &file_name)
{
    if (bitmaps.find(file_name) == bitmaps.end())
    {
        bitmaps[file_name] = Bitmap(file_name);
    }

    return bitmaps[file_name];
}


wxSize Bitmap::GetSize() const
{
    return bitmap.GetSize();
}


wxBitmap &Bitmap::GetBitmap()
{
    return bitmap;
}
