// 2025/8/9 09:54:32 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls/ButtonsCombo.h"
#include "Panels/Panel03_Config/PanelScheme/PainterScheme.h"
#include "Controls/BmpButtonsCombo.h"
#include "Device/Tests/Tests.h"


class ComboJack : public ButtonsCombo
{
public:
    ComboJack(Channel::E, wxWindow *parent, const wxString &title, const wxPoint &pos, int width, const wxArrayString &labels);
    // Установить видимость или невидимость в зависимости от текущих установок
    void SetVisibility();
private:
    Channel::E channel;
};


// На панели "Коммутация" объединяет изображение гнезда и комбобокса с вариантами
class Jack : public wxPanel
{
public:
    Jack(Channel::E, wxWindow *parent, const wxPoint &position, pchar file_jack_bmp, const wxArrayString * = nullptr);
    // Установить видимость или невидимость в зависимости от текущих установок
    void SetVisibility();
private:
    ButtonsCombo *combo = nullptr;
    PainterBMP *painterBMP = nullptr;
    Channel::E channel = Channel::Count;
};


class PanelScheme : public wxPanel
{
    friend struct Category;

public:

    PanelScheme(wxPanel *parent, const int x, int w, int h);

    static PanelScheme *self;

private:

    wxButton *btnLoad = nullptr;

    ButtonsCombo *comboTest = nullptr;

    ComboJack *comboC = nullptr,        //  /
        *comboB = nullptr,              //  | Комбобоксы на месте для отрисовки
        *comboE = nullptr,              //  |
        *comboS = nullptr;              //  /

    Jack *jack[Channel::Count];

    BmpButtonsCombo *comboCategory = nullptr;

    PainterScheme *painter = nullptr;                 // Здесь будет нарисована схема

    void OnEventButton(wxCommandEvent &);
    void OnEventComboBox(wxCommandEvent &);

    // Нарисовать схему в соответствии с установками
    void BuildPanel();
};
