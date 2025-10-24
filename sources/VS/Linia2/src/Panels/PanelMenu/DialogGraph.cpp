// 2025/7/12 17:26:57 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PanelMenu/DialogGraph.h"
#include "Display/WindowScale.h"


DialogGraph *DialogGraph::self = nullptr;
DialogGraphColor *DialogGraphColor::self = nullptr;


const pchar DialogGraph::BTN_SIZE_POINT = "Размер точки";
const pchar DialogGraph::BTN_STYLE_CURVE = "Стиль кривой";
const pchar DialogGraph::BTN_COLOR = "Цвет";
const pchar DialogGraph::BTN_MARKERS = "Маркеры";
const pchar DialogGraph::BTN_SCALE = "Шкала";
const pchar DialogGraph::BTN_SAVE = "Сохранить график в архиве";
const pchar DialogGraph::BTN_RESET = "Сброс графика Ref";

const pchar DialogGraphColor::BTN_BACKGROUND = "Фона";
const pchar DialogGraphColor::BTN_GRID = "Сетки";
const pchar DialogGraphColor::BTN_FONT = "Шрифта";
const pchar DialogGraphColor::BTN_CURVE = "Кривой";
const pchar DialogGraphColor::BTN_LINK = "Ссылки";
const pchar DialogGraphColor::BTN_SECANT = "Секущей";


DialogGraph::DialogGraph() :
    MenuDialog("График", 200, { },
        BTN_SIZE_POINT, []()
        {
        },
        BTN_STYLE_CURVE, []()
        {
        },
        BTN_COLOR, []()
        {
            DialogGraphColor().ShowOnWindow(DialogGraph::self->FindButton(BTN_COLOR));
        },
        BTN_MARKERS, []()
        {
        },
        BTN_SCALE, []()
        {
            WindowScale().ShowOnWindow(DialogGraph::self->FindButton(BTN_SCALE));
        },
        BTN_SAVE, []()
        {
        },
        BTN_RESET, []()
        {
        }
    )
{
    self = this;
}


DialogGraphColor::DialogGraphColor() :
    MenuDialog("Цвет", 125, { },
        BTN_BACKGROUND, []()
        {
            wxColour color;

            if (SelectColor("фона", color))
            {

            }
        },
        BTN_GRID, []()
        {
            wxColour color;

            if (SelectColor("сетки", color))
            {

            }
        },
        BTN_FONT, []()
        {
            wxColour color;

            if (SelectColor("шрифта", color))
            {

            }
        },
        BTN_CURVE, []()
        {
            wxColour color;

            if (SelectColor("кривой", color))
            {

            }
        },
        BTN_LINK, []()
        {
            wxColour color;

            if (SelectColor("ссылки", color))
            {

            }
        },
        BTN_SECANT, []()
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
