// 2026/09/11 22:14:14 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "GUI/Controls/StaticBox.h"
#include "GUI/Controls/Button.h"
#pragma warning(push, 0)
    #include <wx/window.h>
    #include <wx/dcclient.h>
    #include <wx/panel.h>
    #include <wx/timer.h>
#pragma warning(disable)


struct WarningLabel
{
public:
    WarningLabel(wxStaticText *_label) : label(_label)
    {
        timer.SetOwner(label);
        label->Bind(wxEVT_TIMER, &WarningLabel::OnEventTimer, this);
    }

    ~WarningLabel()
    {
        timer.Stop();
    }

    void Enable()
    {
        if (!timer.IsRunning())
        {
            timer.Start(500);
        }
        visible = true;

        label->SetForegroundColour(ColorOne());
        label->SetBackgroundColour(ColorTwo());

        label->Refresh();

        label->Show(true);
    }

    void Disable()
    {
        timer.Stop();
        visible = true;
        label->Show(true);

        label->SetForegroundColour(*wxBLACK);
        label->SetBackgroundColour(label->GetParent()->GetBackgroundColour());

        label->Refresh();
    }

    wxColor ColorOne()
    {
        return label->GetParent()->GetBackgroundColour();
    }

    wxColor ColorTwo()
    {
        return *wxRED;
    }

private:
    wxStaticText *label = nullptr;
    wxTimer timer;
    bool visible = true;

    void OnEventTimer(wxTimerEvent &)
    {
        visible = !visible;

        if (visible)
        {
            label->SetForegroundColour(ColorOne());
            label->SetBackgroundColour(ColorTwo());
        }
        else
        {
            label->SetForegroundColour(ColorTwo());
            label->SetBackgroundColour(ColorOne());
        }

        label->Refresh();

        label->GetParent()->Layout();
    }
};


class Commutator : public StaticBox
{
public:

    struct Value
    {
        enum E
        {
            _Minus50V,
            _Plus50V,
            _3kV,
            Count
        };
    };

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

        txtValue = new StaticText(canvas, "", { 120, 30 }, wxALIGN_CENTER_HORIZONTAL);
        txtValue->SetPosition({ radius - txtValue->GetSize().x / 2, 90 });
        wxFont font = txtValue->GetFont();
        font.SetPointSize(25);
        txtValue->SetFont(font);

        warning_label = std::make_unique<WarningLabel>(txtValue);

        Bind(wxEVT_BUTTON, [this](wxCommandEvent &event)
            {
                int id = event.GetId();
                if (id == btnModePlus50V->GetId())
                {
                    SetValue(Value::_Plus50V);

                    warning_label->Enable();
                }
                else if (id == btnModeMinus50V->GetId())
                {
                    SetValue(Value::_Minus50V);

                    warning_label->Enable();
                }
                else if (id == btnMode3kV->GetId())
                {
                    SetValue(Value::_3kV);

                    warning_label->Disable();
                }
            });

        SetValue(Value::_3kV);
    }

    void SetValue(Value::E new_value)
    {
        value = new_value;
        if (value == Value::_Minus50V)
        {
            txtValue->SetLabel(L("- 50 В"));
        }
        else if (value == Value::_Plus50V)
        {
            txtValue->SetLabel(L("+ 50 В"));
        }
        else if (value == Value::_3kV)
        {
            txtValue->SetLabel(L("3 кВ"));
        }
    }

private:

    int radius = 0;
    wxPanel *canvas = nullptr;
    Button *btnModePlus50V = nullptr;
    Button *btnModeMinus50V = nullptr;
    Button *btnMode3kV = nullptr;
    StaticText *txtValue = nullptr;         // Здесь будет выбранное значение
    Value::E value = Value::_3kV;

    void OnEventPaint(wxPaintEvent &event)
    {
        wxPaintDC dc(canvas);

        dc.SetBrush(*wxTRANSPARENT_BRUSH);
        dc.SetPen(wxPen(*wxBLACK, 1));

        dc.DrawCircle({ radius, radius }, radius);

        event.Skip();
    }

    std::unique_ptr<WarningLabel> warning_label = nullptr;
};

