// 2025/7/27 21:36:06 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel11_Menu/Setup/WindowTableOffsets.h"
#include "Utils/SystemDepend.h"
#include "Device/SettingsDevice.h"


WindowTableOffsets::WindowTableOffsets() :
    Dialog(nullptr, wxID_ANY, _L("Таблица смещений и коэффициентов"), wxDefaultPosition, { WIDTH, HEIGHT })
{
    table = new TableValues(this);

    int x = 500;
    int y = 20;

    new wxStaticText(this, wxID_ANY, "Tаблица", { x - 20, SD::Y_SB(y) });

    wxArrayString choices;
    choices.Add("смещений");
    choices.Add("коэффициентов");

    new wxComboBox(this, ID_OFFSET_COMBO_TYPE, choices[0], { x + 40, SD::Y_SB(y - 3) }, { 90, TEXTCNTRL_HEIGHT }, choices, wxCB_READONLY);

    y = CreateLabelGroup(this, x, y + 20, "Канал С");
    y = CreateRadioButton(this, x, y, ID_OFFSET_RB_CHAN_C_MEAS_I, "Измеритель I");
    y = CreateRadioButton(this, x, y, ID_OFFSET_RB_CHAN_C_MEAS_U, "Измеритель U");
    GF::FindRadioButton(this, ID_OFFSET_RB_CHAN_C_MEAS_I)->SetFocus();

    y = CreateLabelGroup(this, x, y, "Канал B");
    y = CreateRadioButton(this, x, y, ID_OFFSET_RB_CHAN_B_MEAS_I, "Измеритель I");
    y = CreateRadioButton(this, x, y, ID_OFFSET_RB_CHAN_B_MEAS_U, "Измеритель U");
    y = CreateRadioButton(this, x, y, ID_OFFSET_RB_CHAN_B_SOURCE_I, "Источник I");
    y = CreateRadioButton(this, x, y, ID_OFFSET_RB_CHAN_B_SOURCE_U, "Источник U");
    y = CreateRadioButton(this, x, y, ID_OFFSET_RB_CHAN_B_LIMIT_I, "Ограничение I");
    y = CreateRadioButton(this, x, y, ID_OFFSET_RB_CHAN_B_LIMIT_U, "Ограничение U");

    y = CreateLabelGroup(this, x, y, "Канал S");
    y = CreateRadioButton(this, x, y, ID_OFFSET_RB_CHAN_S_MEAS_I, "Измеритель I");
    y = CreateRadioButton(this, x, y, ID_OFFSET_RB_CHAN_S_MEAS_U, "Измеритель U");
    y = CreateRadioButton(this, x, y, ID_OFFSET_RB_CHAN_S_SOURCE_I, "Источник I");
    y = CreateRadioButton(this, x, y, ID_OFFSET_RB_CHAN_S_SOURCE_U, "Источник U");
    y = CreateRadioButton(this, x, y, ID_OFFSET_RB_CHAN_S_LIMIT_I, "Ограничение I");
    y = CreateRadioButton(this, x, y, ID_OFFSET_RB_CHAN_S_LIMIT_U, "Ограничение U");

    y = 450;

    new wxButton(this, ID_OFFSET_BTN_CANCEL, "Применить", { 50, y }, { BUTTON_WIDTH, BUTTON_HEIGHT });
    new wxButton(this, ID_OFFSET_BTN_APPLY, "Отменить", { 250, y }, { BUTTON_WIDTH, BUTTON_HEIGHT });
}
