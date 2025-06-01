// 2023/10/04 19:21:30 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Reader/Reader.h"
#include "Settings/Settings.h"
#include "Reader/Indicator.h"
#include "Utils/SystemDepend.h"
#include "Panels/Notebook/PageMaster/ColorDialog.h"


ColorDialog *ColorDialog::red;
ColorDialog *ColorDialog::green;


Color ColorDialog::reader_red;
Color ColorDialog::reader_green;


void ColorDialog::Create(ButtonColor *buttonRED, ButtonColor *buttonGREEN)
{
    red = new ColorDialog(TypeColor::Red, buttonRED);
    green = new ColorDialog(TypeColor::Green, buttonGREEN);
}


void ColorDialog::Delete()
{
    SAFE_DELETE(red);
    SAFE_DELETE(green);
}


int ColorDialog::ShowModal()
{
    SetColor(CurrentValueColor().Get());

    Reader::SendFormat("#LED ON %s", (type_color == TypeColor::Red) ? "RED" : "GREEN");

    return AutoMovedDialog::ShowModal();
}


ColorDialog::ColorDialog(TypeColor::E type, ButtonColor *_button) :
    AutoMovedDialog((type == TypeColor::Red) ? _L("Цвет Red считывателя") : _L("Цвет Green считывателя")),
    button(_button),
    type_color(type)
{
    Reader::Send("#CONFIG?");

    StoreSystemColors(system_red, system_green);

    painter = new Painter(this, { 360, 20 }, { 100, 200 }, TypePainter::Color_);

    wxBoxSizer *box_vertical = new wxBoxSizer(wxVERTICAL);

    int spacer = 20;

    box_vertical->AddSpacer(spacer);

    box_vertical->Add(CreateSlider(_L("Красный"), ID_SLIDER_RED));

    box_vertical->AddSpacer(spacer);

    box_vertical->Add(CreateSlider(_L("Зелёный"), ID_SLIDER_GREEN));

    box_vertical->AddSpacer(spacer);

    box_vertical->Add(CreateSlider(_L("Синий"), ID_SLIDER_BLUE));

    box_vertical->AddSpacer(spacer);

    box_vertical->Add(CreateSlider(_L("Яркость"), ID_SLIDER_BRIGHTNESS));

    wxBoxSizer *box = new wxBoxSizer(wxVERTICAL);

    box->AddSpacer(spacer);

    box->Add(box_vertical);

    box->AddSpacer(spacer);

    wxBoxSizer *box_buttons = new wxBoxSizer(wxHORIZONTAL);

    box_buttons->AddSpacer(150);

    box_buttons->Add(new wxButton(this, wxID_CANCEL, _L_CLOSE, wxDefaultPosition, BUTTON_SIZE_SMALL));

    box_buttons->AddSpacer(spacer);

    box_buttons->Add(new wxButton(this, wxID_OK, _L_SAVE, wxDefaultPosition, BUTTON_SIZE_SMALL));

    box->Add(box_buttons);

    SetSizer(box);

    AutoMovedDialog::SetFixedSize(SD::D::ColorDialog::Size());

    Bind(wxEVT_BUTTON, &ColorDialog::OnEventButton, this);

    Bind(wxEVT_TIMER, &ColorDialog::OnTimer, this, ID_TIMER_COLOR_DIALOG);

    timer.SetOwner(this, ID_TIMER_COLOR_DIALOG);

    timer.Start(100);
}


ColorDialog::~ColorDialog()
{
    
}


ValueColor &ColorDialog::CurrentValueColor()
{
    return (type_color == TypeColor::Green) ? SET::MASTER::color_green : SET::MASTER::color_red;
}


wxBoxSizer *ColorDialog::CreateSlider(const wxString &title, int id)
{
    wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);

    wxSlider *slider = new wxSlider(this, id, 0, 0, 255, wxDefaultPosition, SD::D::ColorDialog::SizeSlider(), wxSL_HORIZONTAL | wxSL_VALUE_LABEL);

    sizer->AddSpacer(SD::D::ColorDialog::Spacer());

    sizer->Add(new wxStaticText(this, wxID_ANY, title, wxDefaultPosition, { 75, 25 }));

    sizer->Add(slider);

    return sizer;
}


void ColorDialog::OnTimer(wxTimerEvent &)
{
    if (!IsShown())
    {
        return;
    }

    static Color prev_color_red(GetColor());
    static Color prev_color_green(GetColor());

    painter->SetColor(GetColor());

    Color color_red = GetColor();
    Color color_green = GetColor();

    if (type_color == TypeColor::Red)
    {
        color_green = SET::MASTER::color_green.Get();
    }
    else
    {
        color_red = SET::MASTER::color_red.Get();
    }

    if (first)
    {
        first = false;

        Reader::SendFormat("#LED SET RED %08X", color_red.value);
        Reader::SendFormat("#LED SET GREEN %08X", color_green.value);
    }
    else
    {
        if (prev_color_red.value != color_red.value || prev_color_green.value != color_green.value)
        {
            Reader::SendFormat("#LED SET RED %08X", color_red.value);
            Reader::SendFormat("#LED SET GREEN %08X", color_green.value);
        }
    }

    prev_color_red = color_red;
    prev_color_green = color_green;
}


void ColorDialog::OnEventButton(wxCommandEvent &event)
{
    if (event.GetId() == wxID_OK)
    {
        CurrentValueColor().Set(GetColor());
        CurrentValueColor().Save();
        button->SetColor(GetColor());
    }

    RestoreReaderColors();

    Indicator::On();

    EndModal(event.GetId());
}


Color ColorDialog::GetColor()
{
    uint8 r = (uint8)GetSlider(ID_SLIDER_RED)->GetValue();
    uint8 g = (uint8)GetSlider(ID_SLIDER_GREEN)->GetValue();
    uint8 b = (uint8)GetSlider(ID_SLIDER_BLUE)->GetValue();
    uint8 bright = (uint8)GetSlider(ID_SLIDER_BRIGHTNESS)->GetValue();

    return Color(r, g, b, bright);
}


void ColorDialog::SetColor(const Color &color)
{
    GetSlider(ID_SLIDER_RED)->SetValue((int)color.GetRed());
    GetSlider(ID_SLIDER_GREEN)->SetValue((int)color.GetGreen());
    GetSlider(ID_SLIDER_BLUE)->SetValue((int)color.GetBlue());
    GetSlider(ID_SLIDER_BRIGHTNESS)->SetValue((int)color.GetBrightness());

    painter->SetColor(GetColor());
}


wxSlider *ColorDialog::GetSlider(int id)
{
    return (wxSlider *)FindWindow(id);
}


uint8 ColorDialog::GetMaxColor()
{
    float result = ch_red;

    if (ch_green > result)
    {
        result = ch_green;
    }

    if (ch_blue > result)
    {
        result = ch_blue;
    }

    return (uint8)(result + 0.5f);
}


void ColorDialog::StoreSystemColors(Color &_red, Color &_green)
{
    _red = SET::MASTER::color_red.Get();
    _green = SET::MASTER::color_green.Get();
}


void ColorDialog::RestoreSystemColors(const Color &_red, const Color &_green)
{
    SET::MASTER::color_red.Set(_red);
    SET::MASTER::color_green.Set(_green);
}


void ColorDialog::StoreReaderColors(const wxString &_red, const wxString &_green)
{
    reader_red = std::strtoul(_red.c_str(), nullptr, 16);

    reader_green = std::strtoul(_green.c_str(), nullptr, 16);
}


void ColorDialog::RestoreReaderColors()
{
    Reader::SendFormat("#LED SET RED %08X", reader_red.value);
    Reader::SendFormat("#LED SET GREEN %08X", reader_green.value);
}
