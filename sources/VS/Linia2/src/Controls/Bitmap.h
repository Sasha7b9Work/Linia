// 2025/08/10 13:41:12 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class Bitmap
{
public:

    Bitmap() { }

    Bitmap(const wxString &file_name);

    static Bitmap Get(const wxString &file_name);

    wxSize GetSize() const;

    wxBitmap &GetBitmap();

private:

    wxBitmap bitmap;

    static std::map<wxString, Bitmap> bitmaps;
};
