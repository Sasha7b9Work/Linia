// 2025/7/26 23:57:11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Panels/Panel.h"


// Архив эталонных графиков


class PanelReferenceGraph : public Panel
{
public:

    PanelReferenceGraph(wxWindow *);

    static PanelReferenceGraph *self;

private:

    void OnEventButton(wxCommandEvent &);
};
