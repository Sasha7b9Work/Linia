// 2026/09/29 11:22:09 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once
#include "GUI/PageTests/Entities/OStT/OStT.h"


// Резистор


class RES : public OStT2
{
public:

    RES(Test *);

    virtual void Draw(wxAutoBufferedPaintDC &, const wxPoint &) override;

private:
};
