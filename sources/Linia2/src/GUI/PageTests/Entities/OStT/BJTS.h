// 2026/09/29 10:56:39 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once
#include "GUI/PageTests/Entities/OStT/BJT.h"


// Биполярный транзистор с подложкой


class BJTS : public OStT4
{
public:

    BJTS(Test *);

    virtual void Draw(wxAutoBufferedPaintDC &, const wxPoint &) override;

private:

    TypeBJT::E type = TypeBJT::PNP;
};
