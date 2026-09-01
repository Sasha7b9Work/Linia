// 2025/6/3 13:44:53 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once
#include "GUI/Controls/Notebook.h"
#pragma warning(push, 0)
    #include <wx/stattext.h>
#pragma warning(pop)


class PageDebug : public PageNotebook
{
public:

    PageDebug(Notebook *, PageDebug *&);

    virtual bool Show(bool) override;

    wxStaticText *labelButtonStart = nullptr;
    wxStaticText *labelButtonStop = nullptr;
    wxStaticText *labelEncoder = nullptr;
};
