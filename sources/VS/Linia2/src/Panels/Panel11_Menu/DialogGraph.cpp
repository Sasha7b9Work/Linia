// 2025/7/12 17:26:57 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel11_Menu/DialogGraph.h"
#include "Panels/Panel05_Graph/WindowScale.h"


DialogGraph *DialogGraph::self = nullptr;
DialogGraphColor *DialogGraphColor::self = nullptr;


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
            DialogGraphColor().ShowModal();
        },
        BTN_MARKERS, []()
        {
        },
        BTN_SCALE, []()
        {
            WindowScale().ShowModal();
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
