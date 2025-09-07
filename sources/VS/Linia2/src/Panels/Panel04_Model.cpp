// 2025/6/1 17:41:03 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel04_Model.h"
#include "Utils/GlobalFunctions.h"
#include "IPPP/Tests/Model.h"


PanelModel *PanelModel::self = nullptr;


PanelModel::PanelModel(wxWindow* parent) :
    Panel(parent, 0, MainWindow::HEIGHT1, WIDTH, HEIGHT)
{
    self = this;

    const int height_name = 20;

    wxPanel *panel_name = new wxPanel(this, wxID_ANY, { 0, 0 }, { WIDTH, height_name }, wxTAB_TRAVERSAL | wxSUNKEN_BORDER);

    txtName = new wxStaticText(panel_name, wxID_ANY, "", { 0, 0 }, { WIDTH, height_name }, wxALIGN_CENTER);

    Panel::SetName("Файл модели");

    panel_graph = new wxPanel(this, wxID_ANY, { 0, height_name }, { WIDTH, HEIGHT - height_name }, wxTAB_TRAVERSAL | wxSUNKEN_BORDER);

    panel_graph->SetBackgroundColour(*wxWHITE);

    Panel::Update();
}


void PanelModel::SetName(const wxString &_name)
{
    txtName->SetLabel(_name);
}


void PanelModel::Update()
{
    Panel::Update();

    if (Model::IsEmpty())
    {
        SetName("Файл модели");
    }
    else
    {
        SetName(Model::GetName());
    }
}
