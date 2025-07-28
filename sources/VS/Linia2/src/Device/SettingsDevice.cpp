// 2025/7/27 22:27:36 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Device/SettingsDevice.h"


pchar RangeI::Name() const
{
    static const pchar names[Count] =
    {
        "1nA",
        "2nA",
        "5nA",
        "10nA",
        "20nA",
        "50na",
        "100nA",
        "200nA",
        "500nA",
        "1uA",
        "2uA",
        "5uA",
        "10uA",
        "20uA",
        "50uA",
        "100uA",
        "200uA",
        "500uA",
        "1mA",
        "2mA",
        "5mA",
        "10mA",
        "20mA",
        "50mA",
        "100mA",
        "200mA",
        "500mA",
        "1A",
        "2A",
        "5A",
        "10A",
        "20A",
        "50A",
        "100A",
        "200A",
        "500A"
    };

    return names[value];
}
