// 2025/7/12 17:21:00 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Panels/Panel11_Menu/MenuDialog.h"


class DialogGraph : public MenuDialog
{
public:

    DialogGraph();

    static DialogGraph *self;

private:

    const pchar BTN_SIZE_POINT = "Размер точки";
    const pchar BTN_STYLE_CURVE = "Стиль кривой";
    const pchar BTN_COLOR = "Цвет";
    const pchar BTN_MARKERS = "Маркеры";
    const pchar BTN_SCALE = "Шкала";
    const pchar BTN_SAVE = "Сохранить график в архиве";
    const pchar BTN_RESET = "Сброс графика Ref";
};


class DialogGraphColor : public MenuDialog
{
public:

    DialogGraphColor();

    static DialogGraphColor *self;

    static bool SelectColor(const wxString &title, wxColour &);

private:

    const pchar BTN_BACKGROUND = "Фона";
    const pchar BTN_GRID = "Сетки";
    const pchar BTN_FONT = "Шрифта";
    const pchar BTN_CURVE = "Кривой";
    const pchar BTN_LINK = "Ссылки";
    const pchar BTN_SECANT = "Секущей";
};
