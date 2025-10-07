// 2025/6/3 14:02:10 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Panels/PanelDebug/PagesBlocks/PanelRegister.h"


// Это панель для засылки данных в регистры микросхем типа AD

class PageChip : public wxPanel
{
public:

    PageChip(wxNotebook *parent, const wxString &title);

    void AppendRegister(PanelRegister *);

private:

    wxMenuItem *itemReturn = nullptr;

    wxButton *btnReturn = nullptr;

    std::vector<PanelRegister *> registers;

    void OnRightClick(wxMouseEvent &);

    void OnMenuEvent(wxCommandEvent &);

    void OnEventButton(wxCommandEvent &);
};
