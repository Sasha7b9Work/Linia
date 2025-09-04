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

    wxString NamePin(Pin::Type) const;

    int NumPin(Pin::Type) const;

    // in - на этом пине висит лампочка
    wxPanel *CreatePanelPinOut(wxWindow *, PinOut &);
    wxPanel *CreatePanelPinIn(wxWindow *, PinIn &);

    struct StructOutGPIO
    {
        StructOutGPIO(PinOut &_out) : pin(_out) { }
        wxButton   *button = nullptr;
        wxTextCtrl *txtState = nullptr;
        PinOut     &pin;
    };

    struct StructInGPIO
    {
        StructInGPIO(PinIn &_in) : pin(_in) { }
        wxTextCtrl *txtState = nullptr;
        PinIn      &pin;
    };

    std::vector<StructOutGPIO> gpio_out;
    std::vector<StructInGPIO> gpio_in;

    void OnEventButton(wxCommandEvent &);

    static void ThreadFunc();

    static bool thread_is_running;

    std::thread *thread = nullptr;
};
