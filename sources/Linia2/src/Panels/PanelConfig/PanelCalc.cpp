// 2025/6/1 17:41:03 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PanelConfig/PanelCalc.h"
#include "Utils/GlobalFunctions.h"
#include "IPPP/Tests/Model.h"
#include "Panels/PanelConfig/PanelConfig.h"


PanelCalc *PanelCalc::self = nullptr;


PanelCalc::PanelCalc(wxWindow* parent, int /*_x*/, int _w, int /*_h*/) :
    wxPanel(parent)
{
    self = this;

    wxPanel::SetName("PanelCalc");
}


void PanelCalc::SetName(const wxString &_name)
{
    txtName->SetLabel(_name);
}


void PanelCalc::Update()
{
//    Panel::Update();

    if (Model::IsEmpty())
    {
        SetName("Файл модели");
    }
    else
    {
        SetName(Model::GetName());
    }
}
