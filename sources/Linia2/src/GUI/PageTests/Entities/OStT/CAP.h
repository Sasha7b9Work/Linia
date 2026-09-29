// 2026/09/29 11:09:15 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once
#include "GUI/PageTests/Entities/OStT/OStT.h"


// Конденсатор


class CAP : public OStT2
{
public:

    CAP(Test *);

    virtual void Draw(wxAutoBufferedPaintDC &) override;

private:
};

