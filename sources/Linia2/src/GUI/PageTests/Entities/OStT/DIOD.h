// 2026/09/29 11:14:30 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once
#include "GUI/PageTests/Entities/OStT/OStT.h"
#include "Settings/Tests/SettingsTests.h"


// Диод


class DIOD : public OStT
{
public:

    DIOD(Test *);

    virtual void Draw(wxAutoBufferedPaintDC &, const wxPoint &) override;

protected:

    void DrawCommon(wxAutoBufferedPaintDC &, const wxPoint &center);

    // Рисуем анод, потому что он треугольник, катод рисовать уже не нужно
    void DrawAnode(wxAutoBufferedPaintDC &, const wxPoint &center);

    // Смещение угла треугольника относительно центра
    wxPoint Delta() const;

    TypeDIOD::E type = TypeDIOD::Common_Anode_P;
};
