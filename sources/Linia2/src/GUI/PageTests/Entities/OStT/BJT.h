// 2026/09/29 10:57:37 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once
#include "GUI/PageTests/Entities/OStT/OStT.h"


// Биполярный транзистор


class BJT : public OStT3
{
public:

    BJT(Test *);

    virtual void Draw(wxAutoBufferedPaintDC &, const wxPoint &) override;

private:
};
