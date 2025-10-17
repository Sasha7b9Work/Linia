// 2025/08/30 16:03:29 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls/ButtonsCombo.h"


class CommandsCombo : public ButtonsCombo
{
public:
    CommandsCombo(wxWindow *parent, const wxString &title, const wxPoint &pos, int width,
        const wxArrayString &labels, const wxArrayString &tooltips,
        const wxString &name);
private:
    virtual void SetExtendedLabel(const wxString &, const wxString &) override;
};
