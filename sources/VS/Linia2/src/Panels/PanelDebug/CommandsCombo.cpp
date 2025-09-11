// 2025/08/30 16:04:34 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PanelDebug/CommandsCombo.h"


CommandsCombo::CommandsCombo(wxWindow *parent, const wxString &title, const wxPoint &pos, int width, const wxArrayString &labels, const wxString &name) :
    ButtonsCombo(parent, title, pos, width, labels, labels, 1, name, Type::Text)
{

}
