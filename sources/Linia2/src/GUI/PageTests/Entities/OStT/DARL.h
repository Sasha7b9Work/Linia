// 2026/09/29 11:10:48 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once
#include "GUI/PageTests/Entities/OStT/OStT.h"


// Транзистор Дарлингтона


class DARL : public OStT3
{
public:

    DARL(Test *);

    virtual void Draw(wxAutoBufferedPaintDC &, const wxPoint &) override;

private:
};
