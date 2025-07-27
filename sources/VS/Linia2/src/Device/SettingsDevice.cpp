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
    };

    return names[value];
}
