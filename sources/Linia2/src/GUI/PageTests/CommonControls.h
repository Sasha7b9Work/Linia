// 2026/10/08 12:25:06 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once
#include "GUI/PageTests/Entities/Commutator.h"
#include "GUI/Controls/ButtonCombo.h"


// Общие для всех типов элементов органы управления (тип развёртки, например)
struct CommonControls
{
    friend class GroupParameters;
    friend class Scanner;

    void Create(wxWindow *);
    void Show();
    void Hide();

    void Refresh();

    // Открыта крышка
    void OpenCover();

    // Закрыта крышка
    void CloseCover();

    // Если true - находимся в режиме редактирования теста
    bool InModeEdit() const;

    // Вызывать при изменении состояния коммутатора
    void OnEventChangeCommutator();

    Commutator *commutator = nullptr;                   // Управление коммутатором

private:

    StaticBox *boxCover = nullptr;                      // "Крышка"
    StaticText *txtCover = nullptr;                     // Индикатор состояния крышки
    Button *btnEditSave = nullptr;                      // Сохранить результат редактирования
    Button *btnEditExit = nullptr;                      // Выйти из режима редактирования
    bool cover_is_opened = false;

    void CreateButton(Button **, wxWindow *parent, const wxString &, const wxPoint &, const wxSize &, std::function<void(wxCommandEvent &)> onClick);
};
