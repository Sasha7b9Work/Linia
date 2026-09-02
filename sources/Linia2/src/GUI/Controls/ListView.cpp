// 2026/09/02 13:38:54 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "GUI/Controls/ListView.h"


ListView::ListView(wxWindow *parent) :
    wxListView(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_LIST | wxLC_SINGLE_SEL)
{

}


void ListView::SetUserData(long index, void *_data)
{
    data[index] = _data;
}


void *ListView::GetUserData(long index)
{
    return data[index];
}
