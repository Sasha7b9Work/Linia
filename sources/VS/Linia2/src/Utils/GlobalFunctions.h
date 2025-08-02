// 2023/08/10 19:21:10 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls/Buttons.h"


#define FIND_CHECKBUTTON(id)  GF::FindCheckButton(this, (id))
#define FIND_ANY_BUTTON(id)   GF::FindAnyButton(this, (id))
#define FIND_CHECKBOX(id)     GF::FindCheckBox(this, (id))
#define FINC_COMBOBOX(id)     GF::FindComboBox(this, (id))
#define FINC_TEXTCTRL(id)     GF::FindTextCtrl(this, (id))
#define FIND_SPINCTRL(id)     GF::FindSpinCtrl(this, (id))




namespace GF
{
    // Центрировать text относительно border
    void HorizontalAlignCenter(wxStaticText *label, wxWindow *border);

    wxStaticText *_FindStaticText(wxWindow *, int id);

    wxAnyButton *_FindAnyButton(wxWindow *, int id);

    wxToggleButton *_FindToggleButton(wxWindow *, int id);

    wxRadioButton *_FindRadioButton(const wxWindow *, int id);

    wxCheckBox *_FindCheckBox(wxWindow *, int id);

    CheckButton *_FindCheckButton(const wxWindow *, int id);

    wxComboBox *_FindComboBox(const wxWindow *, int id);

    wxTextCtrl *FindTextCtrl(wxWindow *, int id);

    wxSpinCtrl *FindSpinCtrl(wxWindow *, int id);

    void SendCommandEvent(wxWindow *, int type_event, int id, int int_value = 0);

    int IntFromTextCtrl(wxWindow *, int id);

    uint64 UInt64FromTextCtrl(wxWindow *, int id);

    void IntToTextCtrl(wxWindow *, int id, int value);

    void ShowModalInfo(wxWindow *, const wxString &);

    // В эту позицию нужно переместить окно с размерами { width, height }, чтобы оно оказалось в центре экрана
    wxPoint GetCoordCenter(const wxSize &);

    // true, если запущено на плате встраиваемого ПК
    bool IsBoardPCM();
}
