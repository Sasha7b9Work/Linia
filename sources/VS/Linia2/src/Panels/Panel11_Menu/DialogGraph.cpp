// 2025/7/12 17:26:57 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel11_Menu/DialogGraph.h"
#include "Panels/Panel05_Graph/WindowScale.h"


enum
{
    ID_MENU_SIZE_POINT = wxID_HIGHEST + 1,
    ID_MENU_STYLE_CURVE,
    ID_MENU_COLOR,
    ID_MENU_COLOR_BACKGROUND,
    ID_MENU_COLOR_GRID,
    ID_MENU_COLOR_FONT,
    ID_MENU_COLOR_CURVE,
    ID_MENU_COLOR_LINK,
    ID_MENU_COLOR_SECANT,
    ID_MENU_MARKERS,
    ID_MENU_SCALE,
    ID_MENU_SAVE,
    ID_MENU_RESET
};


DialogGraph *DialogGraph::self = nullptr;
DialogGraphColor *DialogGraphColor::self = nullptr;


DialogGraph::DialogGraph() :
    MenuDialog(_L("График"), 200,
        _L("Размер точки"), ID_MENU_SIZE_POINT, OnButtonSizePoint,
        _L("Стиль кривой"), ID_MENU_STYLE_CURVE, OnButtonStyleCurve,
        _L("Цвет"), ID_MENU_COLOR, OnButtonColor,
        _L("Маркеры"), ID_MENU_MARKERS, OnButtonMarkers,
        _L("Шкала"), ID_MENU_SCALE, OnButtonScale,
        _L("Сохранить график в архиве"), ID_MENU_SAVE, OnButtonSave,
        _L("Сброс графика Ref"), ID_MENU_RESET, OnButtonReset
    )
{
    self = this;
}


void DialogGraph::OnButtonSizePoint()
{

}


void DialogGraph::OnButtonStyleCurve()
{

}


void DialogGraph::OnButtonColor()
{
    DialogGraphColor().ShowModal();
}


void DialogGraph::OnButtonMarkers()
{

}


void DialogGraph::OnButtonScale()
{
    WindowScale().ShowModal();
}


void DialogGraph::OnButtonSave()
{

}


void DialogGraph::OnButtonReset()
{

}


DialogGraphColor::DialogGraphColor() :
    MenuDialog(_L("Цвет"), 125,
        _L("Фона"), ID_MENU_COLOR_BACKGROUND, OnButtonBackground,
        _L("Сетки"), ID_MENU_COLOR_GRID, OnButtonGrid,
        _L("Шрифта"), ID_MENU_COLOR_FONT, OnButtonFont,
        _L("Кривой"), ID_MENU_COLOR_CURVE, OnButtonCurve,
        _L("Ссылки"), ID_MENU_COLOR_LINK, OnButtonLink,
        _L("Секущей"), ID_MENU_COLOR_SECANT, OnButtonSecant
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


void DialogGraphColor::OnButtonBackground()
{
    wxColour color;

    if (SelectColor(_L("фона"), color))
    {

    }
}


void DialogGraphColor::OnButtonGrid()
{
    wxColour color;

    if (SelectColor(_L("сетки"), color))
    {

    }
}


void DialogGraphColor::OnButtonFont()
{
    wxColour color;

    if (SelectColor(_L("шрифта"), color))
    {

    }
}


void DialogGraphColor::OnButtonCurve()
{
    wxColour color;

    if (SelectColor(_L("кривой"), color))
    {

    }
}


void DialogGraphColor::OnButtonLink()
{
    wxColour color;

    if (SelectColor(_L("ссылки"), color))
    {

    }
}


void DialogGraphColor::OnButtonSecant()
{
    wxColour color;

    if (SelectColor(_L("секущей"), color))
    {

    }
}

