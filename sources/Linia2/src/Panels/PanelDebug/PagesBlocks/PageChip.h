// 2025/6/3 14:02:10 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Panels/PanelDebug/PagesBlocks/Register.h"


// Это панель для засылки данных в регистры микросхем типа AD

class PageChip : public wxPanel
{
public:

    PageChip(wxNotebook *parent, const wxString &title);

    void AppendRegister(Register *);

    void Pack();
    void Unpack();

    // Возвращает указатель на кнопку закрытия
    wxButton *GetButtonReturn();

private:

    wxMenuItem *itemReturn = nullptr;

    wxButton *btnReturn = nullptr;

    std::vector<Register *> registers;

    void OnRightClick(wxMouseEvent &);

    void OnMenuEvent(wxCommandEvent &);

    void OnEventButton(wxCommandEvent &);

protected:

    // Заполнить регистры каналов B и S, которые одинаковые
    static void FillRegisterBS(RegFPGA *);
};
