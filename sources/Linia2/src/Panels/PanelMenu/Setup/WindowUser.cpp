// 2025/8/1 21:07:59 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PanelMenu/Setup/WindowUser.h"


WindowUser::WindowUser() :
    Dialog(_("Настройки пользователя"), wxDefaultPosition, { WIDTH, HEIGHT })
{
    new wxStaticText(this, wxID_ANY, _("Изделие"), { 20, 20 });
}
