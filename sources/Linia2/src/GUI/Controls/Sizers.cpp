// 2026/04/23 11:53:43 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "GUI/Controls/Sizers.h"


BoxSizerVert::BoxSizerVert() : wxBoxSizer(wxVERTICAL)
{

}


void BoxSizerVert::AddSizer(wxBoxSizer *sizer)
{
    AddSpacer(spacer);

    wxBoxSizer::Add(sizer, 0, wxEXPAND | wxLEFT | wxRIGHT);
}


void BoxSizerVert::AddWidgetsInSizer(wxWindow *widget1, wxWindow *widget2)
{
    BoxSizerHor *sizer = new BoxSizerHor();

    sizer->AddWidget(widget1);

    if (widget2)
    {
        sizer->AddWidget(widget2);
    }

    sizer->Finish();

    AddSizer(sizer);
}


BoxSizerHor::BoxSizerHor() : wxBoxSizer(wxHORIZONTAL)
{
    AddSpacer(spacer);
}


void BoxSizerHor::AddWidget(wxWindow *widget)
{
    wxBoxSizer::Add(widget, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);

    AddSpacer(spacer);
}


void BoxSizerHor::Finish()
{
    AddStretchSpacer();
}
