// 2026/09/11 22:14:14 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "GUI/Controls/StaticBox.h"
#include "GUI/Controls/Button.h"
#pragma warning(push, 0)
    #include <wx/window.h>
    #include <wx/dcclient.h>
    #include <wx/panel.h>
#pragma warning(disable)


class Commutator : public StaticBox
{
public:

    // _width должен быть нечётным
    Commutator(wxWindow *_parent, const wxPoint &_pos, int _width) : StaticBox(_parent, L("Коммутатор"), {_width, _width + 14})
    {
        SetPosition(_pos);

        int w = _width - 4;

        canvas = new wxPanel(this, wxID_ANY, { 2, 15 }, { w, w });

        radius = w / 2;

#ifndef WIN32
        radius -= 6;
#endif

        canvas->Bind(wxEVT_PAINT, &Commutator::OnEventPaint, this);

        wxSize size{ 44, BUTTON_HEIGHT };

        btnModePlus50V = new Button(canvas, L("+50 В"), size);
        btnModePlus50V->SetPosition({ radius + size.x / 2, 50 });

        btnModeMinus50V = new Button(canvas, L("-50 В"), size);
        btnModeMinus50V->SetPosition({ radius - size.x / 2 - size.x, btnModePlus50V->GetPosition().y });

        btnMode3kV = new Button(canvas, L("3 кВ"), size);
        btnMode3kV->SetPosition({ radius - size.x / 2, 10 });
    }

private:

    int radius = 0;
    wxPanel *canvas = nullptr;
    Button *btnModePlus50V = nullptr;
    Button *btnModeMinus50V = nullptr;
    Button *btnMode3kV = nullptr;

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

        event.Skip();
    }
};
