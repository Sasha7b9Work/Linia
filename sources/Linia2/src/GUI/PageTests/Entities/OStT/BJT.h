// 2026/09/29 10:57:37 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once
#include "GUI/PageTests/Entities/OStT/OStT.h"
#include "Settings/Tests/SettingsTests.h"


// Биполярный транзистор


class BJT : public OStT
{
public:

    BJT(Test *);

    virtual void Draw(wxAutoBufferedPaintDC &, const wxPoint &) override;

    // Нарисовать общую часть для BJT и BJTS
    // x_vert - заканчивается линия базы внутри окружности
    void DrawCommon(wxAutoBufferedPaintDC &, const wxPoint &);

private:

    TypeBJT::E type = TypeBJT::PNP;

    void DrawBase(wxAutoBufferedPaintDC &, const wxPoint &center);

    void DrawCollectorEmitter(wxAutoBufferedPaintDC &dc, const wxPoint &c);

    virtual bool GetPoint_B(wxPoint &result) const override
    {
        result = point_B;

        return true;
    }

    // x - смещение вертикальной линии базы относительно центра по горизонтали
    // y - верхняя точка вертикальной линии базы относительно центра
    wxPoint DeltaBase() const
    {
        return wxPoint{ (int)(RADIUS * 0.5), (int)(RADIUS * 0.5) };
    }
};
