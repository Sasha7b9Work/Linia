// 2025/08/30 16:04:34 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "GUI/PageDebug/Notebook/CommandsCombo.h"


CommandsCombo::CommandsCombo(wxWindow *parent, const wxString &title, int width, const wxArrayString &labels, const wxArrayString &tooltips, const wxString &name) :
    ButtonsCombo(parent, title, width, labels, tooltips, 1, name, Type::Text)
{
    SetCurrentSelection(0, __FILE__, __LINE__);
}


void CommandsCombo::SetExtendedLabel(const wxString &, const wxString &, pchar file, int line)
{
    LOG_WRITE("from %s:%d", file, line);

    SetLabel(GetMyToolTip());
}


void CommandsCombo::SetInvalidChoice()
{
    DrawingButton::SetBackgroundColour(*wxRED);
    DrawingButton::SetForegroundColour(*wxRED);
    DrawingButton::Refresh();

    SetToolTip(L("Запрещено"));
}


void CommandsCombo::SetMyToolTip(const wxString &tool)
{
    ButtonsCombo::SetMyToolTip(tool);

    SetToolTip(title + " :\n" + GetMyToolTip());
}
