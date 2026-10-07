// 2026/09/29 11:16:47 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once
#include "GUI/PageTests/Entities/OStT/FET.h"


// Полевой транзистор с подложкой


class FETS : public FET
{
public:

    FETS();

    virtual void Draw(AutoBufferedPaintDC &, const wxPoint &center) override;

    virtual bool GetPoint_S(wxPoint &result) const override
    {
        result = point_S;

        return true;
    }

private:

    void DrawSubstrate(AutoBufferedPaintDC &, const wxPoint &center);
};
