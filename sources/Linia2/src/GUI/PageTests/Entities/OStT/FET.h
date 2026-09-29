// 2026/09/29 11:15:33 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once
#include "GUI/PageTests/Entities/OStT/OStT.h"


// Полевой транзистор


class FET : public OStT3
{
public:

    FET(Test *);

    virtual void Draw(wxAutoBufferedPaintDC &, const wxPoint &) override;

    static void DrawCommon(wxAutoBufferedPaintDC &, const wxPoint &, OStT &self);
};
