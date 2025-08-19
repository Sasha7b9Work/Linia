// 2025/8/9 09:54:32 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls/ButtonsCombo.h"
#include "Panels/Panel03_Config/PanelScheme/PainterScheme.h"
#include "Controls/BmpButtonsCombo.h"


// На панели "Коммутация" объединяет изображение гнезда и комбобокса с вариантами
class Jack : public wxPanel
{
public:
    Jack(wxWindow *parent, const wxPoint &position, pchar file_jack_bmp, const wxArrayString &);
private:
    ButtonsCombo *combo = nullptr;
    PainterBMP *painterBMP = nullptr;
};


class PanelScheme : public wxPanel
{
    friend struct Category;

public:

    PanelScheme(wxPanel *parent, const int x, int w, int h);

    static PanelScheme *self;

private:

    ButtonsCombo *comboC = nullptr,     //  /
        *comboB = nullptr,              //  | Комбобоксы на месте для отрисовки
        *comboE = nullptr,              //  |
        *comboS = nullptr,              //  /
        *comboTest = nullptr;

    wxButton *btnLoad = nullptr;

    Jack *jackB = nullptr;
    Jack *jackC = nullptr;
    PainterBMP *painterJackS = nullptr;
    PainterBMP *painterJackE = nullptr;

    BmpButtonsCombo *comboCategory = nullptr;

    PainterScheme *painter = nullptr;                 // Здесь будет нарисована схема

    void OnEventButton(wxCommandEvent &);
    void OnEventComboBox(wxCommandEvent &);

    // Нарисовать схему в соответствии с установками
    void BuildPanel();
};
