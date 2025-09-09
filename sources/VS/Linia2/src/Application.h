// 2023/07/04 17:46:31 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


/*

    1. В таблице коэффициентов нету источника TypeDSet

*/


class Application : public wxApp
{
    friend class Deivce;

public:
    virtual bool OnInit() wxOVERRIDE;

    static Application *self;

    void Disable();

private:

    // Эти функции вызываеются из Device -----------------------------------------
    // Приём байта от контроллера по UART
    void OnReceiveUART(uint8);

    // Поворот ручки
    void OnGovernor(int rotate);

    // Нажатие/отпускание кнопки СТАРТ
    void OnButtonStart(bool press);

    // Нажатие/отпускание кнопки СТОП
    void OnButtonStop(bool press);

    wxTimer timer;

    virtual int OnExit() wxOVERRIDE;

    void OnTimer(wxTimerEvent &);
};
