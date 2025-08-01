// 2025/7/27 21:36:06 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel11_Menu/Setup/WindowTableOffsets.h"
#include "Utils/SystemDepend.h"
#include "Device/SettingsDevice.h"
#include "Utils/GlobalFunctions.h"
#include "Utils/Math.h"


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

    Bind(wxEVT_RADIOBUTTON, &WindowTableOffsets::OnEventRadioButton, this);
    Bind(wxEVT_BUTTON, &WindowTableOffsets::OnEventButton, this);
    Bind(wxEVT_COMBOBOX, &WindowTableOffsets::OnEventCombobox, this);

    {
        // Включаем соотвествующие поля

        int id = ID_OFFSET_COMBO_TYPE;

        wxCommandEvent evt(wxEVT_COMBOBOX, id);
        evt.SetInt(GF::FindComboBox(this, ID_OFFSET_COMBO_TYPE)->GetCurrentSelection());
        this->ProcessWindowEvent(evt);
    }
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

                Field field;

                field.name = new wxStaticText(panel, wxID_ANY, range.Name(), { x, y }, size1, wxALIGN_CENTER);

                x += size1.x;

                field.value = new wxTextCtrl(panel, wxID_ANY, "", {x, y}, size2);

                field.range = range.value;

                fields_U.push_back(field);

                range++;
            }
        }
    }
}


void WindowTableOffsets::CreateFields_I(wxPanel *panel)
{
    const int d = 10;

    wxSize size1{ 90, TEXTCNTRL_HEIGHT };
    wxSize size2{ 130, TEXTCNTRL_HEIGHT };

    int dh = 1;

    wxSize size{ (size1.x + size2.x) * 2 + d, (size1.y + dh) * ((int)(RangeI::Count + 1) / 2 + 1) };

    SetSizeHints(size);

    new wxStaticText(panel, wxID_ANY, "Диапазон", { 0, 0 }, size1, wxALIGN_CENTER);
    new wxStaticText(panel, wxID_ANY, "Значение", { size1.x, 0 }, size2, wxALIGN_CENTER);
    new wxStaticText(panel, wxID_ANY, "Диапазон", { size1.x + size2.x + d, 0 }, size1, wxALIGN_CENTER);
    new wxStaticText(panel, wxID_ANY, "Значение", { d + (size1.x * 2) + size2.x, 0 }, size2, wxALIGN_CENTER);

    const int num_rows = (int)(RangeI::Count + 1) / 2;

    const int num_cols = 2;

    RangeI range = RangeI((RangeI::E)0);

    for (int col = 0; col < num_cols; col++)
    {
        for (int row = 0; row < num_rows; row++)
        {
            if (range.value < RangeI::Count)
            {
                int x = col * (size1.x + size2.x + d);
                int y = size1.y + row * (size1.y + dh);

                Field field;

                field.name = new wxStaticText(panel, wxID_ANY, "", {x, y}, size1, wxALIGN_CENTER);

                x += size1.x;

                field.value = new wxTextCtrl(panel, wxID_ANY, "", { x, y }, size2);

                field.range = range.value;

                fields_I.push_back(field);

                range++;
            }
        }
    }
}


void WindowTableOffsets::OnEventButton(wxCommandEvent &)
{

}


void WindowTableOffsets::OnEventRadioButton(wxCommandEvent &)
{
    FillFields();
}


void WindowTableOffsets::OnEventCombobox(wxCommandEvent &)
{
    FillFields();
}


void WindowTableOffsets::FillFields()
{
    bool show_I = IsChecked(ID_OFFSET_RB_CHAN_C_MEAS_I) ||
        IsChecked(ID_OFFSET_RB_CHAN_B_MEAS_I) ||
        IsChecked(ID_OFFSET_RB_CHAN_B_SOURCE_I) ||
        IsChecked(ID_OFFSET_RB_CHAN_B_LIMIT_I) ||
        IsChecked(ID_OFFSET_RB_CHAN_S_MEAS_I) ||
        IsChecked(ID_OFFSET_RB_CHAN_S_SOURCE_I) ||
        IsChecked(ID_OFFSET_RB_CHAN_S_LIMIT_I);

    int index = GF::FindComboBox(this, ID_OFFSET_COMBO_TYPE)->GetCurrentSelection();

    if (index == 0)
    {
        FillOffsets(GetTypeMeasure(), show_I);
    }
    else if (index == 1)
    {
        FillK(GetTypeMeasure(), show_I);
    }
    else
    {
        LOG_ERROR("Bad index");
    }

    if (show_I)
    {
        ShowFieldsI(true);
        ShowFieldsU(false);
    }
    else
    {
        ShowFieldsU(true);
        ShowFieldsI(false);
    }
}


bool WindowTableOffsets::IsChecked(int id_radiobutton) const
{
    return GF::FindRadioButton(this, id_radiobutton)->GetValue();
}


void WindowTableOffsets::FillOffsets(DSet::Type::E type, bool show_I)
{
    if (show_I)
    {
        for (uint i = 0; i < RangeI::Count; i++)
        {
            const CalK &cal = DSet::Get(type, (RangeI::E)i);

            fields_I[i].value->SetValue(GetStringValue(cal.offset));
        }
    }
    else
    {
        for (uint i = 0; i < RangeU::Count; i++)
        {
            const CalK &cal = DSet::Get(type, (RangeU::E)i);

            fields_U[i].value->SetValue(GetStringValue(cal.offset));
        }
    }
}


void WindowTableOffsets::FillK(DSet::Type::E type, bool show_I)
{
    if (show_I)
    {
        for (uint i = 0; i < RangeI::Count; i++)
        {
            const CalK &cal = DSet::Get(type, (RangeI::E)i);

            fields_I[i].value->SetValue(GetStringValue(cal.k));
        }
    }
    else
    {
        for (uint i = 0; i < RangeU::Count; i++)
        {
            const CalK &cal = DSet::Get(GetTypeMeasure(), (RangeU::E)i);

            fields_U[i].value->SetValue(GetStringValue(cal.k));
        }
    }
}


wxString WindowTableOffsets::GetStringValue(double value) const
{
    return wxString::Format("%.15f", value);
}


DSet::Type::E WindowTableOffsets::GetTypeMeasure() const
{
    static const int id[DSet::Type::Count][2] =
    {
        { ID_OFFSET_RB_CHAN_C_MEAS_I,   ID_OFFSET_RB_CHAN_C_MEAS_U },
        { ID_OFFSET_RB_CHAN_B_MEAS_I,   ID_OFFSET_RB_CHAN_B_MEAS_U },
        { ID_OFFSET_RB_CHAN_B_SOURCE_I, ID_OFFSET_RB_CHAN_B_SOURCE_U },
        { ID_OFFSET_RB_CHAN_B_LIMIT_I,  ID_OFFSET_RB_CHAN_B_LIMIT_U },
        { ID_OFFSET_RB_CHAN_S_MEAS_I,   ID_OFFSET_RB_CHAN_S_MEAS_U },
        { ID_OFFSET_RB_CHAN_S_SOURCE_I, ID_OFFSET_RB_CHAN_S_SOURCE_U },
        { ID_OFFSET_RB_CHAN_S_LIMIT_I,  ID_OFFSET_RB_CHAN_S_LIMIT_U }
    };

    for (int i = 0; i < DSet::Type::Count; i++)
    {
        if (IsChecked(id[i][0]) || IsChecked(id[i][1]))
        {
            return (DSet::Type::E)i;
        }
    }

    LOG_ERROR("Can not find type measure");

    return DSet::Type::Count;
}


void WindowTableOffsets::ShowFieldsI(bool show)
{
    using namespace Math;

    DSet::Type::E type = GetTypeMeasure();

    for (auto &field : fields_I)
    {
        field.name->Show(show);
        field.value->Show(show);

        if (show)
        {
            field.name->SetLabel(RangeI((RangeI::E)field.range).Name(GetTypeMeasure() == DSet::Type::ChanC_Meas ? RowI::_125 : RowI::_124));

            bool condition = InRange<int>(field.range, RangeI::Min(type), RangeI::Max(type));

            if (!condition)
            {
                field.value->SetLabelText("");
            }

            field.name->Enable(condition);
            field.value->Enable(condition);
        }
    }
}


void WindowTableOffsets::ShowFieldsU(bool show)
{
    using namespace Math;

    DSet::Type::E type = GetTypeMeasure();

    for (auto &field : fields_U)
    {
        field.name->Show(show);
        field.value->Show(show);

        if (show)
        {
            bool condition = InRange<int>(field.range, RangeU::Min(type), RangeU::Max(type));

            if (!condition)
            {
                field.value->SetLabelText("");
            }

            field.name->Enable(condition);
            field.value->Enable(condition);
        }
    }
}
