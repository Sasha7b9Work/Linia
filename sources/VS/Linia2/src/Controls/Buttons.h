// 2023/09/02 11:37:24 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls/Painter.h"
#include "Controls/Bitmap.h"
#include "Controls/Controls.h"


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

    CheckButton(wxWindow *, const wxString &, const wxPoint &, int width);

    bool GetValue() const;

    void SetValue(bool);

    void SetToolTip(const wxString &);

private:

    wxToggleButton *button;
    wxCheckBox *ch_box;

    static const int delta_id = 0;

    void OnEventCheckBox(wxCommandEvent &);
    void OnEventToggleButton(wxCommandEvent &);

    // Послать событие переключения кнопки
    void SendEvent();
};


class ButtonBitmap : public wxBitmapButton
{
public:

    ButtonBitmap(wxWindow *parent, const wxPoint &pos, const wxSize &, const wxString &file_bimap, const wxString &name);

    void SetFileBitmap(const wxString &file_bitmap);

private:

    Bitmap bitmap;
};


// На кнопке при нажатиях переключаются картинки
class ButtonBitmapChoice : public ButtonBitmap
{
public:

    ButtonBitmapChoice(wxWindow *parent, const wxPoint &pos, const wxSize &, const wxArrayString &files, const wxString &name);

    void SetCurrentValue(int);

    int GetCurrentValue() const;

private:

    wxArrayString files;

    int choice = 0;

    void OnEventButton(wxCommandEvent &);
};


class ButtonBitmapChoiceEvent : public wxCommandEvent
{
public:
    ButtonBitmapChoiceEvent(wxEventType eventType, int id) : wxCommandEvent(eventType, id) { }
    ButtonBitmapChoiceEvent(const ButtonBitmapChoiceEvent &other) : wxCommandEvent(other) { }

    virtual wxEvent *Clone() const
    {
        return new ButtonBitmapChoiceEvent(*this);
    }
};


wxDECLARE_EVENT(EVT_BUTTON_BITMAP_CHOICE, ButtonBitmapChoiceEvent);

