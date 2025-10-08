// 2025/09/03 09:50:37 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Communicator/GPIO/GPIO.h"
#include "Controls/Painter.h"


class PageTestsGPIO : public wxPanel
{
public:

    PageTestsGPIO(wxNotebook *parent);

    static PageTestsGPIO *self;

    // Вызывается при заходе на панель отадки
    void Init();

    // Вызывается при открытой панели отладки
    void Update();

    // Вызывается при выходе из панели отладки
    void DeInit();

private:

    // UART
    wxButton *btnSendUART = nullptr;
    wxToggleButton *btnAutoUART = nullptr;    // По этой кнопке начинается автоматическая передача по UART
    wxTextCtrl *txtRecvUART = nullptr;
    wxTextCtrl *txtSendUART = nullptr;
    std::vector<uint8> bytesUART;
    std::mutex mutex_str_UART;

    // SPI
    wxButton *btnSendSPI = nullptr;

    // FPGA
    wxTextCtrl *_txtNumberMeas = nullptr;
    wxTextCtrl *_txtNumberErrors = nullptr;
    int valueMeas = 0;
    int numErrors = 0;

    // Ручка
    wxTextCtrl *_txtKA = nullptr;
    wxTextCtrl *_txtKB = nullptr;
    int valueKA = 0;
    int valueKB = 0;

    wxString NamePin(Pin::E) const;

    int NumPin(Pin::E) const;

    // in - на этом пине висит лампочка
    wxPanel *CreatePanelPinOut(wxWindow *, PinOut *);
    wxPanel *CreatePanelPinIn(wxWindow *, PinIn *);

    struct StructOutGPIO
    {
        StructOutGPIO(PinOut *_out) : pin(_out) { }
        wxButton   *button = nullptr;
        wxTextCtrl *_txtStatePull = nullptr;     // Здесь отображается состояние после опроса
        int value_pull = 0;
        wxTextCtrl *_txtStateInt = nullptr;      // Здесь отображается состояние после срабатывания функции обратного вызова
        PinOut     *pin;
    };

    struct StructInGPIO
    {
        StructInGPIO(PinIn *_in) : pin(_in) { }
        wxTextCtrl *_txtStatePull = nullptr;     // Здесь отображается состояние после опроса
        wxTextCtrl *_txtStateInt = nullptr;      // Здесь отображается состояние после срабатывания функции обратного вызова
        int value_int = 0;
        PinIn      *pin;
    };

    std::vector<StructOutGPIO> gpio_out;
    std::vector<StructInGPIO> gpio_in;

    void OnEventButton(wxCommandEvent &);
    void OnEventToggleButton(wxCommandEvent &);

    static void CallbackOnStart(bool);
    static void CallbackOnStop(bool);
    static void CallbackOnDAT_F0(bool);
    static void CallbackOnDAT_F1(bool);
    static void CallbackOnDAT_F2(bool);
    static void CallbackOnDAT_F3(bool);
    static void CallbackOnFIFO_FULL(bool);
    static void CallbackonREQ_RD(bool);

    void OnChangeStatePin(PinIn *, bool state);
    void OnChangeStatePin(PinOut *, bool state);

    static void FuncUpdateUART();     // Обработка принятых данных по UART в Update

    static void ThreadFunc();   // Главный поток
    static void ThreadFuncFPGA();     // Обработка ПЛИС в главном потоке
    static void ThreadFuncEncoder();  // Обработка ручки в главном потоке
    static bool thread_is_running;
    std::thread *_thread = nullptr;

    // Поток на передачу по UART
    static void ThreadFuncAutoUART();
    static bool thread_autoUART_is_running;
    std::thread *thread_UART = nullptr;

    // Функция на приём символа по UART
    static void FuncRecvUART(uint8);
};
