// 2026/09/11 22:14:14 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "GUI/Controls/StaticBox.h"
#pragma warning(push, 0)
    #include <wx/window.h>
    #include <wx/dcclient.h>
    #include <wx/panel.h>
#pragma warning(disable)


class Commutator : public StaticBox
{
public:

    Commutator(wxWindow *_parent, const wxPoint &_pos, int _width) : StaticBox(_parent, L("Коммутатор"), {_width, _width + 14})
    {
        SetPosition(_pos);

        int w = _width - 4;

        canvas = new wxPanel(this, wxID_ANY, { 2, 15 }, { w, w });

        radius = w / 2;

        canvas->Bind(wxEVT_PAINT, &Commutator::OnEventPaint, this);
    }

private:

    int radius = 0;
    wxPanel *canvas = nullptr;

    void OnEventPaint(wxPaintEvent &event)
    {
        wxPaintDC dc(canvas);

//        dc.SetBrush(*wxBLACK_BRUSH);
//        dc.SetPen(wxPen(*wxBLACK, 1));
//
//        dc.DrawRectangle({ 0, 0 }, { canvas->GetSize().x, canvas->GetSize().y });

        dc.SetBrush(*wxTRANSPARENT_BRUSH);
        dc.SetPen(wxPen(*wxBLACK, 1));

        dc.DrawCircle({ radius, radius }, radius);
    }
};
