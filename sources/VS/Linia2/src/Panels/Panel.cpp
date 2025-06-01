// 2025/6/1 16:29:22 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel.h"


Panel::Panel(wxWindow* parent, int w, int h) :
    wxPanel(parent, wxID_ANY, wxDefaultPosition, { w, h }, wxTAB_TRAVERSAL | wxSUNKEN_BORDER)
{

}
