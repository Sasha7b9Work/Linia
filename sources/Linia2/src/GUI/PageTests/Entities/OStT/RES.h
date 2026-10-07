// 2026/09/29 11:22:09 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once
#include "GUI/PageTests/Entities/OStT/OStT.h"


// Резистор


class RES : public OStT
{
public:

    RES();

    virtual void Draw(AutoBufferedPaintDC &, const wxPoint &) override;

private:
};
