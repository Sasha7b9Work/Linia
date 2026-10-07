// 2026/09/29 11:14:30 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once
#include "GUI/PageTests/Entities/OStT/OStT.h"
#include "Settings/Tests/SettingsTests.h"
#include "Utils/AutoBufferedPaintDC.h"
#include "GUI/PageTests/PanelViewTest.h"


// Диод


class DIOD : public OStT
{
public:

    DIOD();

    virtual void Draw(AutoBufferedPaintDC &, const wxPoint &) override;

    void SetCommonElectrode(CommonElectrode::E e)
    {
        commonElectrode.Set(e);

        PanelViewTest::self->Refresh();
    }

protected:

    void DrawCommon(AutoBufferedPaintDC &, const wxPoint &center);

    // Рисуем анод, потому что он треугольник, катод рисовать уже не нужно
    void DrawAnode(AutoBufferedPaintDC &, const wxPoint &center);

    // Смещение угла треугольника относительно центра
    wxPoint Delta() const;

    CommonElectrode commonElectrode{ CommonElectrode::Anode_P };
};
