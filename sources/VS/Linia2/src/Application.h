// 2023/07/04 17:46:31 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class Application : public wxApp
{
public:
    virtual bool OnInit() wxOVERRIDE;

    static Application *self;

    static wxString file_name_config;       // Имя файла конфигурации

    void Disable();

private:

    wxTimer timer;

    virtual int OnExit() wxOVERRIDE;

    void OnTimer(wxTimerEvent &);
};
