// 2026/09/29 11:14:30 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once
#include "GUI/PageTests/Entities/OStT/OStT.h"


// Диод


class DIOD : public OStT
{
public:

    DIOD(Test *);

    virtual void Draw(wxAutoBufferedPaintDC &, const wxPoint &) override;

protected:

    void DrawCommon(wxAutoBufferedPaintDC &, const wxPoint &center);

    // Смещение угла треугольника относительно центра
    wxPoint Delta() const;
};
