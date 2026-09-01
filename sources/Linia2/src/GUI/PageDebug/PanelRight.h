// 2026/01/24 21:41:36 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once
#include "GUI/Controls/ControlDataFPGA.h"
#include "GUI/Controls/Button.h"
#include "GUI/Controls/Panel.h"


class TextCtrlNumber;


class PanelRight : public Panel
{
public:

    PanelRight(wxWindow *, PanelRight *&);

    ControlDataFPGA *data[NUMBER_ADC + 1];           // Отображение данных, считанных c платы контроллера

    void ApplicationTask();

private:

    ~PanelRight();

    Button *btnStart = nullptr;         // Запуск развёртки
    Button *btnStop = nullptr;          // Останов развёртки
    Button *btnWriteData = nullptr;     // Сохранение данных в файл

    TextCtrlNumber *txtPeriodScan = nullptr;    // Период запуска развёртки в миллисекундах

    void OnEventButton(wxCommandEvent &);

    // Сохраняет в файле настроек установленный период засылок
    uint SavePeriodScan();
};
