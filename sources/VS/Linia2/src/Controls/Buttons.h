// 2023/09/02 11:37:24 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls/PainterRect.h"


struct Color;


class ButtonColor : public wxButton
{
public:

    ButtonColor(wxWindow *, int, const wxString &, wxPoint, wxSize, PainterRect *);

    void SetColor(const Color &);

    wxColour GetColor() const
    {
        return m_painter->GetColor();
    };

private:

    PainterRect *m_painter = nullptr;

    void OnMouseEvent(wxMouseEvent &);
};


class CheckButton : public wxPanel
{
public:

    CheckButton(wxWindow *, int, const wxString &, const wxPoint &, const wxSize &);

    bool GetValue() const;

    void SetValue(bool);

    void SetToolTip(const wxString &);

private:

    wxToggleButton *button;
    wxCheckBox *ch_box;

    static const int delta_id = 0;

    void OnEventCheckBox(wxCommandEvent &);

    void OnEventToggleButton(wxCommandEvent &);
};


class ButtonBitmap : public wxBitmapButton
{
public:

    ButtonBitmap(wxWindow *parent, const wxPoint &pos, const wxSize &, const wxString &file_bimap);

protected:

    void CreateBitmap(wxBitmap &, const wxString &file_bitmap);

private:

    wxBitmap _bitmap;
};
