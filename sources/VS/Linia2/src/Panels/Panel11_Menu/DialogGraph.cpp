// 2025/7/12 17:26:57 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel11_Menu/DialogGraph.h"
#include "Panels/Panel05_Graph/WindowScale.h"


DialogGraph *DialogGraph::self = nullptr;
DialogGraphColor *DialogGraphColor::self = nullptr;


DialogGraph::DialogGraph() :
    MenuDialog("График", 200, { },
        "Размер точки", [](){},
        "Стиль кривой", []() {},
        "Цвет", []()
        {
            DialogGraphColor().ShowModal();
        },
        "Маркеры", []() {},
        "Шкала", []()
        {
            WindowScale().ShowModal();
        },
        "Сохранить график в архиве", []() {},
        "Сброс графика Ref", []() {}
    )
{
    self = this;
}


DialogGraphColor::DialogGraphColor() :
    MenuDialog("Цвет", 125, { },
        "Фона", []()
        {
            wxColour color;

            if (SelectColor("фона", color))
            {

            }
        },
        "Сетки", []()
        {
            wxColour color;

            if (SelectColor("сетки", color))
            {

            }
        },
        "Шрифта", []()
        {
            wxColour color;

            if (SelectColor("шрифта", color))
            {

            }
        },
        "Кривой", []()
        {
            wxColour color;

            if (SelectColor("кривой", color))
            {

            }
        },
        "Ссылки", []()
        {
            wxColour color;

            if (SelectColor("ссылки", color))
            {

            }
        },
        "Секущей", []()
        {
            wxColour color;

            if (SelectColor("секущей", color))
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

    dialog.SetTitle(wxString("Цвет ") + title);

    if (dialog.ShowModal() == wxID_OK)
    {
        color = dialog.GetColourData().GetColour();

        return true;
    }

    return false;
}
