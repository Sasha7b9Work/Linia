// 2026/09/29 11:09:15 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once
#include "GUI/PageTests/Entities/OStT/OStT.h"


// Конденсатор


class CAP : public OStT
{
public:

    CAP(Test *);

    virtual void Draw(AutoBufferedPaintDC &, const wxPoint &) override;

private:
};

