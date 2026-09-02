// 2026/04/29 16:01:42 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once
#include "GUI/Controls/Panel.h"
#include "Settings/Tests/Library/Library.h"


class PanelViewTest : public Panel
{
public:

    PanelViewTest(wxWindow *, PanelViewTest *&);

    void SetTest(Test *);

private:

    Test *test = nullptr;

    void OnEventPaint(wxPaintEvent &);
};
