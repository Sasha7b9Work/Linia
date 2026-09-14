// 2025/8/20 19:59:36 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once
#pragma warning(push, 0)
    #include <wx/statbox.h>
    #include <wx/dcbuffer.h>
#pragma warning(pop)


class StaticBox : public wxStaticBox
{
public:
    StaticBox(wxWindow *parent, const wxString &_title, const wxSize &size = wxDefaultSize) :
        wxStaticBox(parent, wxID_ANY,
#ifdef WIN32
            _title,
#else
            "",
#endif
            wxDefaultPosition, size, wxBORDER_RAISED), title(_title)
    {
        wxStaticBox::SetName(parent->GetName() + wxString{ "_static_box" });

        if (font == wxNullFont)
        {
            font = GetFont();
            font.SetWeight(wxFONTWEIGHT_BOLD);
            //            font.SetPointSize(font.GetPointSize() + 1);
        }

#ifdef __WXGTK__
        // Своя отрисовка рамки и заголовка
        SetBackgroundStyle(wxBG_STYLE_PAINT);
        Bind(wxEVT_PAINT, &StaticBox::OnPaintGtk, this);
        Bind(wxEVT_ERASE_BACKGROUND, [](wxEraseEvent &) { } );
#endif
    }

    static wxFont &TitleFont()
    {
        return font;
    }

private:

#ifdef __WXGTK__
    void OnPaintGtk(wxPaintEvent &)
    {

        fprintf(stderr, "StaticBox::OnPaintGtk: size=%dx%d, title='%s'\n",
            GetClientSize().x, GetClientSize().y,
            (const char *)title.utf8_str());

        wxAutoBufferedPaintDC dc(this);

        const wxSize sz = GetClientSize();
        const int margin = FromDIP(2);
        const int titleH = TitleFont().GetPixelSize().GetHeight() + FromDIP(4);
        const int rectX = margin;
        const int rectY = titleH / 2;
        const int rectW = sz.x - 2 * margin;
        const int rectH = sz.y - rectY - margin;

        if (rectW <= 0 || rectH <= 0)
            return;

        // Рисуем только рамку и заголовок.
        // Фон под дочерними контролами не затираем.
        dc.SetPen(wxPen(wxColour(127, 127, 127), 1));
        dc.SetBrush(*wxTRANSPARENT_BRUSH);
        dc.DrawRectangle(rectX, rectY, rectW, rectH);

        if (!title.IsEmpty())
        {
            dc.SetFont(TitleFont());
            dc.SetTextForeground(wxColour(80, 80, 80));
            dc.SetBackgroundMode(wxBRUSHSTYLE_TRANSPARENT);

            const wxSize textSz = dc.GetTextExtent(title);
            const int padX = FromDIP(4);
            const int textX = rectX + FromDIP(6);
            const int textY = rectY - textSz.y / 2;

            // Затираем линию рамки фоном панели — только под текстом
            dc.SetPen(*wxTRANSPARENT_PEN);
            dc.SetBrush(wxBrush(GetBackgroundColour()));
            dc.DrawRectangle(textX - padX, textY,
                textSz.x + 2 * padX, textSz.y);

            dc.DrawText(title, textX, textY);
        }
    }
#endif

    static wxFont font;
    wxString title;
};
