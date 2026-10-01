// 2026/09/29 11:23:57 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once
#include "GUI/PageTests/Entities/OStT/DIOD.h"


// Тиристор


class THYR : public DIOD
{
public:

    THYR(Test *);

    virtual void Draw(AutoBufferedPaintDC &, const wxPoint &center) override;

private:

    ControlElectrode controlElectrode{ ControlElectrode::Anode_P };

    virtual bool GetPoint_B(wxPoint &result) const override
    {
        result = point_B;

        return true;
    }

    void DrawControlElectrode(AutoBufferedPaintDC &, const wxPoint &center);
};
