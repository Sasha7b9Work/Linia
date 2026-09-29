// 2026/09/29 11:15:33 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once
#include "GUI/PageTests/Entities/OStT/OStT.h"
#include "Settings/Tests/SettingsTests.h"


// Полевой транзистор


class FET : public OStT
{
public:

    FET(Test *);

    virtual void Draw(wxAutoBufferedPaintDC &, const wxPoint &) override;

    void DrawCommon(wxAutoBufferedPaintDC &, const wxPoint &);

private:

    TypeFET::E type = TypeFET::ChannelN;

    // Нарисовать затвор
    void DrawGate(wxAutoBufferedPaintDC &, const wxPoint &);

    // Нарисовать исток и сток
    void DrawSourceDrain(wxAutoBufferedPaintDC &, const wxPoint &);

    // Смещение линии затвора относительно центра по X
    int GateDX();

    // Смещение линии затвора относительно центра по Y
    int GateDY();

    int DrainDY();
};
