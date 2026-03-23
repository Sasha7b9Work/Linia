// 2023/07/04 17:46:31 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


/*

    1. В таблице коэффициентов нету источника TypeDSet

*/


class Application : public wxApp
{
    friend class Deivce;
    friend class EmulatorIPPP;

public:
    virtual bool OnInit() wxOVERRIDE;

    void Disable();

private:

    wxTimer timer;

    virtual int OnExit() wxOVERRIDE;

    // Таймер главного цикла
    void OnTimer(wxTimerEvent &);

public:

    // Эти функции вызываются из Device -----------------------------------------
    // Приём байта от контроллера по UART
    void OnReceiveUART(uint8);

    // Поворот ручки
    void OnGovernor(int rotate);

    // Нажатие/отпускание кнопки СТАРТ
    void OnButtonStart(bool press);

    // Нажатие/отпускание кнопки СТОП
    void OnButtonStop(bool press);

    // Эти функции вызываются из IPPP --------------------------------------------
    void OnReadData(
        std::array<int, 100> &data1,
        std::array<int, 100> &data2,
        std::array<int, 100> &data3,
        std::array<int, 100> &data4
    );

    //-----------------------------------------------------------------------------
};
