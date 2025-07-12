// 2025/7/12 17:26:57 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel11_Menu/DialogGraph.h"


DialogGraph *DialogGraph::self = nullptr;


DialogGraph::DialogGraph() :
    MenuDialog(_L("График"), 200,
        _L("Размер точки"), ID_BTN_MENU_GRAPH_SIZE_POINT, OnButtonSizePoint,
        _L("Стиль кривой"), ID_BTN_MENU_GRAPH_STYLE_CURVE, OnButtonStyleCurve,
        _L("Цвет"), ID_BTN_MENU_GRAPH_COLOR, OnButtonColor,
        _L("Маркеры"), ID_BTN_MENU_GRAPH_MARKERS, OnButtonMarkers,
        _L("Шкала"), ID_BTN_MENU_GRAPH_SCALE, OnButtonScale,
        _L("Сохранить график в архиве"), ID_BTN_MENU_GRAPH_SAVE, OnButtonSave,
        _L("Сброс графика Ref"), ID_BTN_MENU_GRAPH_RESET, OnButtonReset
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

}


void DialogGraph::OnButtonMarkers()
{

}


void DialogGraph::OnButtonScale()
{

}


void DialogGraph::OnButtonSave()
{

}


void DialogGraph::OnButtonReset()
{

}
