// 2023/07/04 17:46:11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Panels/Notebook/Pages.h"
#include "Controls/Painter.h"
#include "Controls/Buttons.h"
#include "Controls/TextControls.h"


class CntrlText4x2;
class CntrlMemBlock;


class PageMaster : public Page
{
public:
    PageMaster(wxNotebook *parent);

    ~PageMaster();

    static PageMaster *self;

    ButtonColor *buttonRED = nullptr;
    ButtonColor *buttonGREEN = nullptr;

    // Настройки изменились - нужно убрать признак того, что установлены заводские настройки
    void OnChangeSettings();

private:

    void OnEventButton(wxCommandEvent &);

    void OnEventToggleButton(wxCommandEvent &);

    void OnEventText(wxCommandEvent &);

    void OnEventCheckBox(wxCommandEvent &);

    void OnEventComboBox(wxCommandEvent &);

    // Вовзращает true, если текущие настройки соответствую заводским
    bool CurrentSettingsEqualFactory() const;

    void ResetToFactory(bool factory);
};
