// 2025/7/12 17:26:57 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PanelMenu/DialogGraph.h"


DialogGraph *DialogGraph::self = nullptr;


const pchar DialogGraph::BTN_STYLE_CURVE = "Стиль кривой";
const pchar DialogGraph::BTN_MARKERS = "Маркеры";
const pchar DialogGraph::BTN_SCALE = "Шкала";
const pchar DialogGraph::BTN_SAVE = "Сохранить график в архиве";
const pchar DialogGraph::BTN_RESET = "Сброс графика Ref";

DialogGraph::DialogGraph() :
    MenuDialog("График", 200, { },
        BTN_STYLE_CURVE, []()
        {
        },
        BTN_MARKERS, []()
        {
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
