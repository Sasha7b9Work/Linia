// 2025/7/12 17:26:57 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel11_Menu/DialogGraph.h"
#include "Panels/Panel05_Graph/WindowScale.h"


DialogGraph *DialogGraph::self = nullptr;
DialogGraphColor *DialogGraphColor::self = nullptr;


DialogGraph::DialogGraph() :
    MenuDialog(_L("График"), 200,
        _L("Размер точки"), ID_MENU_SIZE_POINT, [](){},
        _L("Стиль кривой"), ID_MENU_STYLE_CURVE, []() {},
        _L("Цвет"), ID_MENU_COLOR, []()
        {
            DialogGraphColor().ShowModal();
        },
        _L("Маркеры"), ID_MENU_MARKERS, []() {},
        _L("Шкала"), ID_MENU_SCALE, []()
        {
            WindowScale().ShowModal();
        },
        _L("Сохранить график в архиве"), wxID_SAVE, []() {},
        _L("Сброс графика Ref"), wxID_RESET, []() {}
    )
{
    self = this;
}


DialogGraphColor::DialogGraphColor() :
    MenuDialog(_L("Цвет"), 125,
        _L("Фона"), ID_MENU_COLOR_BACKGROUND, []()
        {
            wxColour color;

            if (SelectColor(_L("фона"), color))
            {

            }
        },
        _L("Сетки"), ID_MENU_COLOR_GRID, []()
        {
            wxColour color;

            if (SelectColor(_L("сетки"), color))
            {

            }
        },
        _L("Шрифта"), ID_MENU_COLOR_FONT, []()
        {
            wxColour color;

            if (SelectColor(_L("шрифта"), color))
            {

            }
        },
        _L("Кривой"), ID_MENU_COLOR_CURVE, []()
        {
            wxColour color;

            if (SelectColor(_L("кривой"), color))
            {

            }
        },
        _L("Ссылки"), ID_MENU_COLOR_LINK, []()
        {
            wxColour color;

            if (SelectColor(_L("ссылки"), color))
            {

            }
        },
        _L("Секущей"), ID_MENU_COLOR_SECANT, []()
        {
            wxColour color;

            if (SelectColor(_L("секущей"), color))
            {

            }
        }
    )
{
    self = this;
}


bool DialogGraphColor::SelectColor(const wxString &title, wxColour &color)
{
    wxColourData colourData;
    colourData.SetChooseFull(true);

    wxColourDialog dialog(DialogGraphColor::self, &colourData);

    dialog.SetTitle(_L("Цвет") + " " + title);

    if (dialog.ShowModal() == wxID_OK)
    {
        color = dialog.GetColourData().GetColour();

        return true;
    }

    return false;
}
