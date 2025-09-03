// 2025/09/03 09:50:37 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Communicator/GPIO/GPIO.h"
#include "Controls/Painter.h"


class PageTestsGPIO : public wxPanel
{
public:

    PageTestsGPIO(wxNotebook *parent);

    static PageTestsGPIO *self;

private:

    wxString NamePin(Pin::E) const;

    int NumPin(Pin::E) const;

    // in - на этом пине висит лампочка
    // out - сюда ставим переключатель, чтобы управлять лампочкой
    wxPanel *CreatePanelPin(wxWindow *, PinIn &, PinOut &);

    struct StructGPIO
    {
        wxButton   *button;
        wxTextCtrl *txtState;
    };

    std::vector<StructGPIO> gpio;
};
