// 2023/07/04 17:46:31 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once
#include "Communicator/ClientHTTP/ClientHTTP.h"
#pragma warning(push, 0)
    #include <wx/app.h>
    #include <wx/timer.h>
#pragma warning(pop)



class Application : public wxApp
{
    friend class Deivce;
    friend class EmulatorIPPP;

public:

    virtual bool OnInit() wxOVERRIDE;

    ClientHTTP logger{ "188.127.240.34", 8080 };

private:

    wxTimer timer;          // По этому таймеру работает главный цикл

    virtual int OnExit() wxOVERRIDE;

public:

    void ReInit();

    // Эти функции вызываются из Device -----------------------------------------
    // Приём байта от контроллера по UART
    void OnReceiveUART(uint8);

    // Поворот ручки
    void OnGovernor(int rotate);

    // Нажатие/отпускание кнопки СТАРТ
    void OnButtonStart(bool press);

    // Нажатие/отпускание кнопки СТОП
    void OnButtonStop(bool press);
};
