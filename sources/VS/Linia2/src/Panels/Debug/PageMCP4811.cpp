// 2025/6/3 22:32:57 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Debug/PageMCP4811.h"


PageMCP4811 *PageMCP4811::self = nullptr;


PageMCP4811::PageMCP4811(wxNotebook *parent) : PageChip(parent, "MCP4811")
{
    self = this;
}
