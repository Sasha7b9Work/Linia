// 2023/08/31 11:42:36 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Utils/String.h"


struct ReaderInfo
{
    wxString product_type;
    wxString serial_number;             // Серийный номер
    wxString version_hardware;          // Аппаратная версия
    wxString version_loader;            // Версия загрузчика
    wxString version_software;
    wxString data_manufacture;
    wxString info;
    bool     eco_enabled;               // Режим ЭКО (LPCD)
    wxString osdp_address;              // адрес устройства OSDP
    wxString osdp_baudrate;             // скорость обмена устройства OSDP
    wxString time_build;                // дата сборки прошивки
};


namespace Reader
{
    bool IsConnected();

    void UpdateOnReceive(pchar);

    void Send(pchar);

    void SendFormat(const char *format, ...);

    void SendBuffer(void *buffer, int num_bytes);

    ReaderInfo GetInfo();

    // Время последнего отключения (со старта программы)
    uint TimeFromDisconnect();

    void Disconnect();
}
