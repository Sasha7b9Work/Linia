// 2026/09/29 10:56:39 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once
#include "GUI/PageTests/Entities/OStT/BJT.h"


// Биполярный транзистор с подложкой


class BJTS : public BJT
{
public:

    BJTS(Test *);

    virtual void Draw(wxAutoBufferedPaintDC &, const wxPoint &) override;

    virtual bool GetPoint_S(wxPoint &result) const override
    {
        result = point_S;

        return true;
    }

private:

    void DrawSubstrate(wxAutoBufferedPaintDC &, const wxPoint &center);
};
