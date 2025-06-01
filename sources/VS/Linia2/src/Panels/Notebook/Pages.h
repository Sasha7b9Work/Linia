// 2023/07/04 17:46:31 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Utils/GlobalFunctions.h"
#include "Utils/SystemDepend.h"


class Page : public wxPanel
{
public:

    // ser_name - имя для сериализации
    Page(wxNotebook* parent, const wxString &title);

protected:

    static const int FIRST_X = X_0;

    // Отключить защиту у карты
    void DisableProtection();

    // Координата Y первого контрола после действующего пароля
    int FirstY() const {  return 70; };

    // Нумерация начинается с 1
    int PositionButton(int i);

    void DrawDemarcationLine(int y);

    int ButtonY() const { return 300; }
};
