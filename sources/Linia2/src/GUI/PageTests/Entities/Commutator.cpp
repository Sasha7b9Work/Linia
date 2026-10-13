// 2026/09/11 22:14:22 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "GUI/PageTests/Entities/Commutator.h"
#include "GUI/PageTests/PanelViewTest.h"


void Commutator::OnEventButton(wxCommandEvent &event)
{
    int id = event.GetId();
    if (id == btnModePlus50V->GetId())
    {
        SetValue(Value::_Plus50V);

        warning_label->Enable();

        PanelViewTest::self->OnEventChangeCommutator();
    }
    else if (id == btnModeMinus50V->GetId())
    {
        SetValue(Value::_Minus50V);

        warning_label->Enable();

        PanelViewTest::self->OnEventChangeCommutator();
    }
    else if (id == btnMode3kV->GetId())
    {
        SetValue(Value::_3kV);

        warning_label->Disable();

        PanelViewTest::self->OnEventChangeCommutator();
    }

    Refresh();
}
