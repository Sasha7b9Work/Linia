// 2026/09/29 11:15:33 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once
#include "GUI/PageTests/Entities/OStT/OStT.h"
#include "Settings/Tests/SettingsTests.h"
#include "GUI/PageTests/PanelViewTest.h"


// Полевой транзистор


class FET : public OStT
{
public:

    FET();

    virtual void Draw(AutoBufferedPaintDC &, const wxPoint &) override;

    void DrawCommon(AutoBufferedPaintDC &, const wxPoint &);

    void SetType(TypeFET::E t)
    {
        type.Set(t);

        PanelViewTest::self->Refresh();
    }

private:

    TypeFET type{ TypeFET::ChannelP };

    // Нарисовать затвор
    void DrawGate(AutoBufferedPaintDC &, const wxPoint &);

    // Нарисовать исток и сток
    void DrawSourceDrain(AutoBufferedPaintDC &, const wxPoint &);

    // Смещение линии затвора относительно центра по X
    int GateDX();

    // Смещение линии затвора относительно центра по Y
    int GateDY();

    int DrainDY();

    virtual bool GetPoint_B(wxPoint &result) const override
    {
        result = point_B;

        return true;
    }
};
