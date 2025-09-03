// 2025/09/03 09:50:37 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Communicator/GPIO/GPIO.h"
#include "Controls/Painter.h"


class PageTestsGPIO : public wxPanel
{
public:

    PageTestsGPIO(wxNotebook *parent);

    static PageTestsGPIO *self;

    void Init();

    void DeInit();

private:

    wxString NamePin(Pin::E) const;

    int NumPin(Pin::E) const;

    // in - на этом пине висит лампочка
    // out - сюда ставим переключатель, чтобы управлять лампочкой
    wxPanel *CreatePanelPin(wxWindow *, PinIn &, PinOut &);

    struct StructGPIO
    {
        StructGPIO(PinIn &_in, PinOut &_out) : in(_in), out(_out) { }
        wxButton   *button = nullptr;
        wxTextCtrl *txtState = nullptr;
        PinIn      &in;
        PinOut     &out;
    };

    std::vector<StructGPIO> gpio;

    void OnEventButton(wxCommandEvent &);

    static void ThreadFunc();

    static bool thread_is_running;

    std::thread *thread = nullptr;
};
