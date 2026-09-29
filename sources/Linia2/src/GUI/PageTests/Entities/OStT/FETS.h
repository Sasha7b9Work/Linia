// 2026/09/29 11:16:47 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once
#include "GUI/PageTests/Entities/OStT/OStT.h"


// Полевой транзистор с подложкой


class FETS : public OStT4
{
public:

    FETS(Test *);

    virtual void Draw(wxAutoBufferedPaintDC &, const wxPoint &) override;

private:
};
