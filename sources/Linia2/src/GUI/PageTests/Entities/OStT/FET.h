// 2026/09/29 11:15:33 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once
#include "GUI/PageTests/Entities/OStT/OStT.h"


// Полевой транзистор


class FET : public OStT3
{
public:

    FET();

    virtual void Draw(wxAutoBufferedPaintDC &) override;

private:
};
