// 2023/10/04 19:21:13 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls/Painter.h"
#include "Controls/Buttons.h"
#include "Settings/Value.h"
#include "Panels/Dialogs/AutoMovedDialog.h"


class ColorDialog : public AutoMovedDialog
{
public:

    static ColorDialog *red;
    static ColorDialog *green;

    static void Create(ButtonColor *buttonRED, ButtonColor *buttonGREEN);
    static void Delete();

    // Сохраняет установленные в считывателе цвета
    static void StoreReaderColors(const wxString &red, const wxString &green);

    virtual int ShowModal() override;

    ButtonColor *button;

private:

    ColorDialog(TypeColor::E, ButtonColor *);
    virtual ~ColorDialog();

    Painter *painter;

    TypeColor::E type_color;

    float ch_red = 0.0f;
    float ch_green = 0.0f;
    float ch_blue = 0.0f;
    float brightness = 0.0f;

    bool first = true;

    wxTimer timer;

    wxBoxSizer *CreateSlider(const wxString &, int id);

    void OnEventButton(wxCommandEvent &);

    wxSlider *GetSlider(int);

    // Возвращает максимальное значение из red, green, blue
    uint8 GetMaxColor();

    // Возвращает цвет, установленный слайдерами
    Color GetColor();

    void SetColor(const Color &);

    Color system_red{ 0, 0, 0 };
    Color system_green{ 0, 0, 0 };
    void StoreSystemColors(Color &_red, Color &_green);
    void RestoreSystemColors(const Color &_red, const Color &_green);

    void OnTimer(wxTimerEvent &);

    // Восстанавливает установленные в считывателе цвета
    static void RestoreReaderColors();

    static Color reader_red;
    static Color reader_green;

    ValueColor &CurrentValueColor();
};

