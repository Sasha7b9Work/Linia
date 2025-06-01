// 2023/08/11 11:28:48 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Reader/Reader.h"
#include "Settings/Color.h"
#include "Reader/SettingsMaster.h"
#include "Controls/Painter.h"
#include "Panels/Dialogs/AutoMovedDialog.h"


struct PanelCard : public wxPanel
{
    PanelCard(wxWindow *window) : wxPanel(window) { }

protected:

    const int d = 20;

    std::vector<wxStaticText *> values;

    wxStaticText *AddLine(bool append, wxBoxSizer *, const wxString &, const wxString &hint);
};


struct PanelUnknownCard : public PanelCard
{
    PanelUnknownCard(wxWindow *);

    void Fill();

    static const wxSize size;

private:

    wxBoxSizer *CreateColumn(
        bool append,
        const wxString & = "",
        const wxString & = ""
    );

    wxStaticText *valueTypeCard = nullptr;
    wxStaticText *valueID = nullptr;
    wxButton     *buttonKeys = nullptr;
};


struct PanelUserCard : public PanelCard
{
    PanelUserCard(wxWindow *);

    void Fill();

    static const wxSize size;

private:
    wxBoxSizer *CreateColumn(
        bool append,
        const wxString & = "",
        const wxString & = "",
        const wxString & = "",
        const wxString & = "",
        const wxString & = "",
        const wxString & = ""
    );

    wxStaticText *valueFunction = nullptr;      // Назначение карты
    wxStaticText *valueTypeCard = nullptr;
    wxStaticText *valueID = nullptr;
    wxStaticText *valuePassword = nullptr;
    wxStaticText *valueNumber = nullptr;
    wxStaticText *valueDate = nullptr;
};


struct PanelMasterCard : public PanelCard
{
    PanelMasterCard(wxWindow *);

    void Fill();

    static const wxSize size;

private:

    // Вторая колонка - подсказка
    wxBoxSizer *CreateColumn(
        bool append,
        const wxString & = "", const wxString & = "",     // Назначение
        const wxString & = "", const wxString & = "",     // Тип карты
        const wxString & = "", const wxString & = "",     // ID
        const wxString & = "", const wxString & = "",     // Пароль
        const wxString & = "", const wxString & = "",     // Пароль системы
        const wxString & = "", const wxString & = "",     // Цвет RED
        const wxString & = "", const wxString & = "",     // Цвет GREEN
        const wxString & = "", const wxString & = "",     // Режим ЭКО
        const wxString & = "", const wxString & = "",     // Режим OSDP
        const wxString & = "", const wxString & = "",     // Адрес OSDP
        const wxString & = "", const wxString & = "",     // Скорость OSDP
        const wxString & = "", const wxString & = "",     // Мелодия Beep
        const wxString & = "", const wxString & = "",     // Мелодия Green
        const wxString & = "", const wxString & = "",     // Мелодия Red
        const wxString & = "", const wxString & = "",     // Автоном
        const wxString & = "", const wxString & = "",     // WIEGAND
        const wxString & = "", const wxString & = "",     // Only SL3
        const wxString & = "", const wxString & = "",     // Датчик чувств
        const wxString & = "", const wxString & = ""     // Датчик номер
    );

    wxStaticText *valueFunction = nullptr;      // Назначение карты
    wxStaticText *valueTypeCard = nullptr;
    wxStaticText *valueID = nullptr;
    wxStaticText *valuePassword = nullptr;
    wxStaticText *valueNewPassword = nullptr;
    wxStaticText *valueColorRed = nullptr;
    wxStaticText *valueColorGreen = nullptr;
    wxStaticText *valueModeECO = nullptr;
    wxStaticText *valueEnabledOSDP = nullptr;
    wxStaticText *valueAddressOSDP = nullptr;
    wxStaticText *valueBaudRateOSDP = nullptr;
    wxStaticText *valueSoundBeep = nullptr;
    wxStaticText *valueSoundGreen = nullptr;
    wxStaticText *valueSoundRed = nullptr;
    wxStaticText *valueOfflineMode = nullptr;
    wxStaticText *valueWiegand = nullptr;
    wxStaticText *valueOnlySL3 = nullptr;
    wxStaticText *valueAntibreakSens = nullptr;
    wxStaticText *valueAntibreakNumber = nullptr;

    Painter *painterRED = nullptr;
    Painter *painterGREEN = nullptr;

    wxString ColorValue(const Color &);

    void WriteSound(wxStaticText *, const SettingsMaster &, TypeSound::E);
};


class CardInfoDialog : public AutoMovedDialog
{
public:

    CardInfoDialog();
    virtual ~CardInfoDialog();

    static CardInfoDialog *self;

    // Заполнить значениями в соотвествии с текущей картой
    static void Fill();

private:

    void OnEventButton(wxCommandEvent &);

    wxBoxSizer       *boxUnknownCard = nullptr;
    PanelUnknownCard *panelUnknownCard = nullptr;

    wxBoxSizer       *boxUserCard = nullptr;
    PanelUserCard    *panelUserCard = nullptr;

    wxBoxSizer       *boxMasterCard = nullptr;
    PanelMasterCard *panelMasterCard = nullptr;
};
