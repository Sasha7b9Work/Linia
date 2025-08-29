// 2025/8/9 09:54:32 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls/ButtonsCombo.h"
#include "Panels/Panel03_Config/PanelScheme/PainterScheme.h"
#include "Controls/BmpButtonsCombo.h"
#include "Tests/Tests.h"


// Отображается на отрисованной схеме
class ComboJack : public ButtonsCombo
{
public:
    ComboJack(Channel::E, wxWindow *parent, const wxString &title, const wxPoint &pos, int width, const wxArrayString &labels);
    // Настроить текущее состояние в зависимости от текущих установок
    void TuneState();
private:
    Channel::E channel;
    // Установить видимость или невидимость в зависимости от текущих установок
    void SetVisibility();

    void SetChoices();

    void SetChoice(StateJack::E);
};


// На панели "Коммутация" объединяет изображение гнезда и комбобокса с вариантами
class FullJack : public wxPanel, public Serializer
{
public:
    FullJack(Channel::E, wxWindow *parent, const wxPoint &position, pchar file_jack_bmp, const wxArrayString * = nullptr);
    // Настроить текущее состояние в зависимости от текущих установок
    void TuneState();

    virtual void Pack() override;
    virtual void Unpack() override;
private:
    ButtonsCombo *combo = nullptr;
    PainterBMP *painterBMP = nullptr;
    Channel::E channel = Channel::Count;
    // Установить видимость или невидимость в зависимости от текущих установок
    void SetVisibility();

    void SetChoices();

    void SetChoice(StateJack::E);
};


class PanelScheme : public wxPanel
{
    friend struct Category;
    friend struct TypeCommutation;

public:

    PanelScheme(wxPanel *parent, const int x, int w, int h);

    static PanelScheme *self;

    void Pack();
    void Unpack();

private:

    wxButton *btnLoad = nullptr;

    ButtonsCombo *comboTest = nullptr,
        *comboCommutation = nullptr;        // IDC_COMBO_KOMMUTATOR         m_iKommutator           OnSelchangeComboKommutator

    ComboJack *combo[Channel::Count];       // Комбобоксы на схеме. Активны при внешней коммутации  IDC_COMBO_GNEZDO_C
    FullJack *jack[Channel::Count];         // Активны при внутренней коммутации                    IDC_COMBOCHECKKOL

    BmpButtonsCombo *comboCategory = nullptr;

    PainterScheme *painter = nullptr;                 // Здесь будет нарисована схема

    void OnEventButton(wxCommandEvent &);
    void OnEventComboBox(wxCommandEvent &);

    // Нарисовать схему в соответствии с установками
    void BuildPanel();
};
