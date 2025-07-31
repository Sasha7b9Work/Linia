// 2025/7/27 21:36:06 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel11_Menu/Setup/WindowTableOffsets.h"
#include "Utils/SystemDepend.h"
#include "Device/SettingsDevice.h"


WindowTableOffsets::WindowTableOffsets() :
    Dialog(nullptr, wxID_ANY, _L("Таблица смещений и коэффициентов"), wxDefaultPosition, { WIDTH, HEIGHT })
{
    CreateFields();

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

    new wxButton(this, ID_OFFSET_BTN_CANCEL, "Применить", { 50, HEIGHT - 100 }, { BUTTON_WIDTH, BUTTON_HEIGHT });
    new wxButton(this, ID_OFFSET_BTN_APPLY, "Отменить", { 250, HEIGHT - 100 }, { BUTTON_WIDTH, BUTTON_HEIGHT });
}


void WindowTableOffsets::CreateFields()
{
    wxPanel *panel = new wxPanel(this, wxID_ANY, { 10, 20 }, { 450, 460 });

    CreateFields_U(panel);
    CreateFields_I(panel);
}


void WindowTableOffsets::CreateFields_U(wxPanel *panel)
{
    const int d = 10;

    wxSize size1{ 90, TEXTCNTRL_HEIGHT };
    wxSize size2{ 130, TEXTCNTRL_HEIGHT };

    int dh = 1;

    wxSize size{ (size1.x + size2.x) * 2 + d, (size1.y + dh) * ((int)(RangeU::Count + 1) / 2 + 1) };

    SetSizeHints(size);

    new wxStaticText(panel, wxID_ANY, "Диапазон", { 0, 0 }, size1, wxALIGN_CENTER);
    new wxStaticText(panel, wxID_ANY, "Значение", { size1.x, 0 }, size2, wxALIGN_CENTER);
    new wxStaticText(panel, wxID_ANY, "Диапазон", { size1.x + size2.x + d, 0 }, size1, wxALIGN_CENTER);
    new wxStaticText(panel, wxID_ANY, "Значение", { d + (size1.x * 2) + size2.x, 0 }, size2, wxALIGN_CENTER);

    const int num_rows = (int)(RangeU::Count + 1) / 2;

    const int num_cols = 2;

    RangeU range = RangeU((RangeU::E)0);

    for (int col = 0; col < num_cols; col++)
    {
        for (int row = 0; row < num_rows; row++)
        {
            if (range.value < RangeU::Count)
            {
                int x = col * (size1.x + size2.x + d);
                int y = size1.y + row * (size1.y + dh);

                new wxStaticText(panel, wxID_ANY, range.Name(), { x, y }, size1, wxALIGN_CENTER);

                x += size1.x;

                new wxTextCtrl(panel, wxID_ANY, wxString::Format("%.15f", 0.0), { x, y }, size2);

                range++;
            }
        }
    }
}


void WindowTableOffsets::CreateFields_I(wxPanel *)
{

}

