// 2025/7/27 21:36:06 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel11_Menu/Setup/WindowTableOffsets.h"
#include "Utils/SystemDepend.h"
#include "Device/SettingsDevice.h"


WindowTableOffsets::WindowTableOffsets() :
    Dialog(nullptr, wxID_ANY, _L("Таблица смещений и коэффициентов"), wxDefaultPosition, { WIDTH, HEIGHT })
{
    table = new TableValues(this);

    FillTable();
}


void WindowTableOffsets::FillTable()
{
    wxVector <TableStruct> values;

    for (int i = RangeI::Count - 1; i >= 0; i--)
    {
        values.push_back({ RangeI((RangeI::E)i).Name(), 0.0 });
    }

    table->SetAll(values);

    table->SetPosition({ 20, SD::Y_SB(20) });

    table->Layout();

    Layout();
}
