// 2025/7/12 17:26:57 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel11_Menu/DialogGraph.h"
#include "Panels/Panel05_Graph/WindowScale.h"


DialogGraph *DialogGraph::self = nullptr;
DialogGraphColor *DialogGraphColor::self = nullptr;


DialogGraph::DialogGraph() :
    MenuDialog(_L("График"), 200,
        _L("Размер точки"), [](){},
        _L("Стиль кривой"), []() {},
        _L("Цвет"), []()
        {
            DialogGraphColor().ShowModal();
        },
        _L("Маркеры"), []() {},
        _L("Шкала"), []()
        {
            WindowScale().ShowModal();
        },
        _L("Сохранить график в архиве"), []() {},
        _L("Сброс графика Ref"), []() {}
    )
{
    self = this;
}


DialogGraphColor::DialogGraphColor() :
    MenuDialog(_L("Цвет"), 125,
        _L("Фона"), []()
        {
            wxColour color;

            if (SelectColor(_L("фона"), color))
            {

            }
        },
        _L("Сетки"), []()
        {
            wxColour color;

            if (SelectColor(_L("сетки"), color))
            {

            }
        },
        _L("Шрифта"), []()
        {
            wxColour color;

            if (SelectColor(_L("шрифта"), color))
            {

            }
        },
        _L("Кривой"), []()
        {
            wxColour color;

            if (SelectColor(_L("кривой"), color))
            {

            }
        },
        _L("Ссылки"), []()
        {
            wxColour color;

            if (SelectColor(_L("ссылки"), color))
            {

            }
        },
        _L("Секущей"), []()
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
