// 2026/09/02 13:37:58 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once
#include <map>
#pragma warning(push, 0)
    #include <wx/listctrl.h>
#pragma warning(pop)


class ListView : public wxListView
{
public:

    ListView(wxWindow *);

    void SetUserData(long, void *);

    void *GetUserData(long);

private:

    std::map<long, void *> data;
};
