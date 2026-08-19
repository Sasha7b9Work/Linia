// 2026/04/08 15:30:46 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once
#include "GUI/Controls/Panel.h"


class ToggleButton;
class Notebook;
class wxBoxSizer;


// Это панель - со своей кнопкой выбора, с возможностью открепляться от PanelBoard
class PageNotebook : public Panel
{
    friend class Notebook;

public:

    PageNotebook(Notebook *, const wxString &);

    const wxString &GetPanelName() const;

private:

    wxString name;
};


class Notebook : public Panel
{
public:

    Notebook(wxWindow *);

    void AddPanel(PageNotebook *);

    wxPanel *GetCenterContainer() const
    {
        return container;
    }

    int GetCurrentPanelIndex() const;

    // Переключиться на панель по указателю
    void SetCurrentPanel(PageNotebook *);

private:

    wxBoxSizer *sizer_main = nullptr;       // Вертикальный главный sizer
    wxBoxSizer *sizer_buttons = nullptr;    // Горизонтальный sizer для верхних кнопок
    wxBoxSizer *sizer_container = nullptr;  // Sizer для центрального контейнера

    wxPanel *container = nullptr;           // Здесь хранится содержимое (PageNotebook)
    PageNotebook *current_page = nullptr;   // Текущая PageNotebook

    std::vector<ToggleButton *> buttons;    // Кнопки, соотвествующие панелям

    // Добавить кнопку в верхнюю панель
    void AddTopButton(PageNotebook *);

    void OnEventButtonToggle(wxCommandEvent &);

    int GetPanelIndex(PageNotebook *) const;
};
