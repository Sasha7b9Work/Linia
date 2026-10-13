// 2026/10/13 08:24:21 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once
#include "GUI/PageTests/Entities/OStT/DIOD.h"


// Стабилитрон


class ZENER : public DIOD
{
public:

    ZENER() : DIOD() { }

    virtual void Draw(AutoBufferedPaintDC &, const wxPoint &) override;

protected:
};
