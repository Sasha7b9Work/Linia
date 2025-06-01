// 2023/07/04 17:46:14 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Dialogs/PasswordDialog.h"
#include "Settings/Settings.h"
#include "Panels/Notebook/PageMaster/ColorDialog.h"
#include "Utils/GlobalFunctions.h"
#include "Controls/Buttons.h"
#include "Controls/TextControls.h"
#include "Utils/Configurator.h"
#include "Controls/Painter.h"
#include "Reader/Reader.h"
#include "Settings/Settings.h"
#include "DataBase/DataBaseCards.h"
#include "Utils/String.h"
#include "Utils/StringUtils.h"
#include "Reader/Indicator.h"
#include "Panels/Notebook/PageMaster/PageMaster.h"
#include "Panels/Notebook/PageMaster/OSDPModeDialog.h"
#include "Panels/Notebook/PageMaster/AntiBreakDialog.h"
#include "Panels/Notebook/PageMaster/OfflineModeDialog.h"
#include "Panels/Notebook/PageMaster/ExtendedSettingsDialog.h"
#include "Panels/Notebook/PageMaster/ModeEKODialog.h"
#include "Panels/Notebook/PageMaster/ChooseSoundsDialog.h"
#include "Panels/Notebook/PageMaster/WiegandDialog.h"


PageMaster *PageMaster::self = nullptr;


PageMaster::PageMaster(wxNotebook *parent) :
    Page(parent, _L("Мастер карта"))
{
    OSDPModeDialog::Create();

    AntiBreakDialog::Create();

    ChooseSoundsDialog::Create();

    WiegandDialog::Create();

    OfflineModeDialog::Create();

    ExtendedSettingsDialog::Create();

    ModeEKODialog::Create();

    self = this;

    int X_BOX = 8;

    const int Y_TOP = 15;

    wxColor sys_color = GetBackgroundColour();

    Page::SetBackgroundColour(sys_color);

    {
        wxStaticBox *boxFactorySettings = new wxStaticBox(this, wxID_ANY, _L("Заводские настройки"), { X_BOX, Y_TOP }, { WIDTH_BOX, 65 });

        boxFactorySettings->SetBackgroundColour(parent->GetBackgroundColour());

        new CheckButton(boxFactorySettings, ID_CHECKBUTTON_FACTORY_SETTINGS, _L("Вкл-Откл"), { X_0, Y_0 }, BUTTON_SIZE);
    }

    {
        wxStaticBox *boxConfig = new wxStaticBox(this, wxID_ANY, _L("Конфигурация"), { X_BOX, 90 }, { WIDTH_BOX, 180 });

        boxConfig->SetBackgroundColour(sys_color);

        wxSize size = BUTTON_SIZE;

        size.x -= size.y + 3;

        int sp = size.y - 1;

        Painter *painterRED = new Painter(boxConfig, { X_0 + size.x + 5, Y_0 + 1 }, { sp, sp }, TypePainter::Red);

        buttonRED = new ButtonColor(boxConfig, ID_BUTTON_RED, _L("Цвет Red"), { X_0, Y_0 }, size, painterRED);

        Painter *painterGREEN = new Painter(boxConfig, { X_0 + size.x + 5, Y_1 + 1}, { sp, sp }, TypePainter::Green);

        buttonGREEN = new ButtonColor(boxConfig, ID_BUTTON_GREEN, _L("Цвет Green"), { X_0, Y_1 }, size, painterGREEN);

        ColorDialog::Create(buttonRED, buttonGREEN);

        new wxButton(boxConfig, ID_BUTTON_MELODY, _L("Звуки"), { X_0, Y_3 - 5 }, BUTTON_SIZE);
    }

    {
        wxPanel *panel = new wxPanel(this, wxID_ANY, { X_BOX - 5, 285 }, { WIDTH_BOX + 10, 155 });

        panel->SetBackgroundColour(*wxRED);

        wxStaticBox *boxWriteMaster = new wxStaticBox(panel, wxID_ANY, _L("Запись на мастер карту"), { 5, 5 }, { WIDTH_BOX, 145 });

        boxWriteMaster->SetBackgroundColour(sys_color);

        (new wxCheckBox(boxWriteMaster, ID_CHECKBOX_WRITE_MASTER_PASSWORD, _L("Действующего пароля"), { X_0, Y_0 + 10 }))->SetValue(true);

        new wxCheckBox(boxWriteMaster, ID_CHECKBOX_WRITE_MASTER_CONFIG, _L("Конфигураций"), { X_0, Y_1 });

        new wxButton(boxWriteMaster, ID_BUTTON_WRITE_MASTER_CARD, _L("Записать на карту"), { X_0, Y_2 - 5 }, BUTTON_SIZE);
    }

    {
        wxStaticBox *boxExtra = new wxStaticBox(this, wxID_ANY, _L("Дополнительно"), { 200, Y_TOP }, { WIDTH_BOX, 230 });

        boxExtra->SetBackgroundColour(sys_color);

        new wxButton(boxExtra, ID_BUTTON_OFFLINE_MODE, _L("Автономный режим"), { X_0, Y_0 }, BUTTON_SIZE);

        new wxButton(boxExtra, ID_BUTTON_OSDP, _L("Режим OSDP"), { X_0, Y_1 }, BUTTON_SIZE);

        new wxButton(boxExtra, ID_BUTTON_ANTIBREAK, _L("Датчик отрыва"), { X_0, Y_2 }, BUTTON_SIZE);

        new wxButton(boxExtra, ID_BUTTON_EKO, _L("Режим ЭКО"), { X_0, Y_3 }, BUTTON_SIZE);
        FindWindow(ID_BUTTON_EKO)->SetToolTip(_L("Hint mode eko"));

        new wxButton(boxExtra, ID_BUTTON_EXTRA, _L("Расширенные настройки"), { X_0, Y_4 }, BUTTON_SIZE);
    }

    {
        new wxButton(this, ID_BUTTON_DECODE_MP3, "Decode MP3", { 210, 300 }, BUTTON_SIZE);
    }

    Bind(wxEVT_BUTTON, &PageMaster::OnEventButton, this);
    Bind(wxEVT_CHECKBOX, &PageMaster::OnEventCheckBox, this);
    Bind(wxEVT_COMBOBOX, &PageMaster::OnEventComboBox, this);
    Bind(wxEVT_TOGGLEBUTTON, &PageMaster::OnEventToggleButton, this);
    Bind(wxEVT_TEXT, &PageMaster::OnEventText, this);
}


PageMaster::~PageMaster()
{
    AntiBreakDialog::Delete();

    OSDPModeDialog::Delete();

    ChooseSoundsDialog::Delete();

    WiegandDialog::Delete();

    ColorDialog::Delete();

    OfflineModeDialog::Delete();

    ExtendedSettingsDialog::Delete();

    ModeEKODialog::Delete();
}


void PageMaster::OnEventButton(wxCommandEvent &event)
{
    int id = event.GetId();

    if (id == ID_BUTTON_WRITE_MASTER_CARD)
    {
        if (!Card::IsInserted())
        {
            return;
        }

        Indicator::Task();

        FindWindow(ID_BUTTON_WRITE_MASTER_CARD)->Enable(false);

        if (FIND_CHECKBOX(ID_CHECKBOX_WRITE_MASTER_CONFIG)->GetValue())        // Запись карты новой конфигурации
        {
            String message(
                "#MAKE MASTER\n"
                "FPWD=%llu\n"
                "NPWD=%llu\n"
                "WG=%08X\n"
                "CRED=%08X\n"
                "CGREEN=%08X\n"
                "M1=%d\n"
                "M2=%d\n"
                "M3=%d\n"
                "V1=%d\n"
                "V2=%d\n"
                "V3=%d\n"
                "NSIZE=%d\n"
                "NBEGIN=%d\n"
                "TLOCK=%d\n"
                "TRUN=%d\n"
                "AMODE=%d\n"
                "OMODE=%d\n"
                "OSDPA=%d\n"
                "OSDPBR=%d\n"
                "OSDPEN=%d\n"
                "ECO=%d\n"
                "SENS=%d\n"
                "NUMBERSENS=%d\n"
                "ONLY_SL3=%d\n"
                "%s\n"
                "CRC32=",
                gset.password_master.value,                 // 3 старый
                gset.password_current.value,                // 4 новый
                SET::WIEGAND::GetFullValue(),               // 5
                SET::MASTER::color_red.Get().value,         // 6
                SET::MASTER::color_green.Get().value,       // 7
                SET::MASTER::melody[0].GetIndex(),          // 8
                SET::MASTER::melody[1].GetIndex(),          // 9
                SET::MASTER::melody[2].GetIndex(),          // 10
                SET::MASTER::volume[0].Get(),               // 11
                SET::MASTER::volume[1].Get(),               // 12
                SET::MASTER::volume[2].Get(),               // 13
                3,                                          // 14
                4,                                          // 15
                SET::OFFLINE::time_lock.Get(),              // 16 Время замка
                SET::OFFLINE::time_alarm.Get(),             // 17 Время тревоги
                SET::OFFLINE::security_mode.Get() ? 1 : 0,  // 18 Режим охраны
                SET::OFFLINE::enabled.Get() ? 1 : 0,        // 19 Автономный режим
                SET::OSDP::address.Get(),                   // 20 Адрес OSDP
                BaudRateOSDP::FromIndexCombobox(SET::OSDP::baudrate.GetIndex()),    // 21 Скорость OSDP
                SET::OSDP::enabled.Get() ? 1 : 0,           // 22
                SET::mode_eco.Get() ? 1 : 0,                // 23
                SET::ANTIBREAK::GetRAW(),                   // 24 Чувствительность датчика отрыва
                SET::ANTIBREAK::number.Get(),               // 25 Номер датчика отрыва
                SET::MISC::disable_less_SL3.Get(),          // 26 Отключены ли карты SL3
                CardsDB::GetBits(600).c_str().AsChar()      // 27 Массив разрешённых карт
            );

            message.Append(String("%08X\n#END", SU::CalculateCRC32(message.c_str(), message.Size())).c_str());

            Reader::Send(message.c_str());
        }
        else                                                                        // Запись карты нового пароля
        {
            String message("#MAKE MASTER FPWD=%llu NPWD=%llu CRC32=", gset.password_master.value, gset.password_current.value);

            message.Append(String("%08X", SU::CalculateCRC32(message.c_str(), message.Size())).c_str());

            Reader::Send(message.c_str());
        }
    }
    else if (id == ID_BUTTON_RED)
    {
        ColorDialog::red->ShowModal();

        OnChangeSettings();
    }
    else if (id == ID_BUTTON_GREEN)
    {
        ColorDialog::green->ShowModal();

        OnChangeSettings();
    }
    else if (id == ID_BUTTON_MELODY)
    {
        ChooseSoundsDialog::self->ShowModal();
    }
    else if (id == ID_BUTTON_OFFLINE_MODE)
    {
        OfflineModeDialog::self->ShowModal();

        GF::FindAnyButton(PageMaster::self, ID_BUTTON_OSDP)->Enable(!SET::OFFLINE::enabled.Get());
    }
    else if (id == ID_BUTTON_OSDP)
    {
        OSDPModeDialog::self->ShowModal();
    }
    else if (id == ID_BUTTON_ANTIBREAK)
    {
        AntiBreakDialog::self->ShowModal();
    }
    else if (id == ID_BUTTON_EXTRA)
    {
        ExtendedSettingsDialog::self->ShowModal();
    }
    else if (id == ID_BUTTON_EKO)
    {
        ModeEKODialog::self->ShowModal();
    }
    else if (id == ID_BUTTON_DECODE_MP3)
    {
        wxFileDialog dialog(this, "Выбрать файл mp3", wxEmptyString, wxEmptyString,
            wxString("Файлы mp3") + "(*.mp3) | *.mp3; *.MP3",
            wxFD_OPEN);

        if (dialog.ShowModal() == wxID_OK)
        {
            wxString file_name = dialog.GetPath();

            std::vector<int> samples;

        }
    }

    OnChangeSettings();
}


void PageMaster::OnEventCheckBox(wxCommandEvent &event)
{
    int id = event.GetId();

    bool checked = event.IsChecked();

    if (id == ID_CHECKBOX_WRITE_MASTER_PASSWORD)
    {
        if (!checked)
        {
            FIND_CHECKBOX(ID_CHECKBOX_WRITE_MASTER_PASSWORD)->SetValue(true);
        }
    }
    else if (id == ID_CHECKBOX_WRITE_MASTER_CONFIG)
    {
        SET::MASTER::write_config.SetFromControl();
    }
    else if (id == ID_CHECKBOX_EXT_PARITY)
    {
        SET::EXT::parity.SetFromControl();
    }
    else if (id == ID_CHECKBOX_EXT_INVERSION_CODE)
    {
        SET::EXT::inverse_code.SetFromControl();
    }
    else if (id == ID_CHECKBOX_EXT_CONTROL_BIT)
    {
        SET::EXT::control_bit.SetFromControl();
    }

    OnChangeSettings();
}


void PageMaster::OnEventComboBox(wxCommandEvent &event)
{
    int id = event.GetId();

    if (id == ID_COMBOBOX_EXT_MODE_READ_CARD)
    {
        bool enable = event.GetSelection() == 0;

        FindWindow(ID_STATICTEXT_PERIOD_AUTOREPEAT)->Enable(enable);
        FindWindow(ID_TEXTCNTRL_EXT_PERIOD_AUTOREPEAT)->Enable(enable);

        SET::EXT::mode_read_card.SetFromControl();
    }

    OnChangeSettings();
}


void PageMaster::OnChangeSettings()
{
    FIND_CHECKBUTTON(ID_CHECKBUTTON_FACTORY_SETTINGS)->SetValue(CurrentSettingsEqualFactory());
}


void PageMaster::OnEventText(wxCommandEvent &event)
{
    event.Skip();

    int id = event.GetId();

    if (id == ID_TEXTCNTRL_EXT_PERIOD_AUTOREPEAT)
    {
        SET::EXT::period_autorepeat.SetFromControl();
    }
}


void PageMaster::OnEventToggleButton(wxCommandEvent &event)
{
    int id = event.GetId();
    bool checked = event.IsChecked();

    if (id == ID_CHECKBUTTON_EXT_SET_ENABLE)
    {
        FindWindow(ID_CHECKBOX_EXT_PARITY)->Enable(checked);
        FindWindow(ID_COMBOBOX_EXT_MODE_READ_CARD)->Enable(checked);
        FindWindow(ID_STATICTEXT_MODE_READ_CARD)->Enable(checked);
        FindWindow(ID_STATICTEXT_PERIOD_AUTOREPEAT)->Enable(checked);
        FindWindow(ID_TEXTCNTRL_EXT_PERIOD_AUTOREPEAT)->Enable(checked);
        FindWindow(ID_CHECKBOX_EXT_INVERSION_CODE)->Enable(checked);
        FindWindow(ID_CHECKBOX_EXT_CONTROL_BIT)->Enable(checked);

        SET::EXT::enabled.SetFromControl();

        OnChangeSettings();
    }
    else if (id == ID_CHECKBUTTON_FACTORY_SETTINGS)
    {
        ResetToFactory(checked);

        if (!checked && CurrentSettingsEqualFactory())
        {
            FIND_CHECKBUTTON(ID_CHECKBUTTON_FACTORY_SETTINGS)->SetValue(true);
        }

        GF::FindCheckButton(this, ID_CHECKBUTTON_FACTORY_SETTINGS)->SetToolTip(FIND_CHECKBUTTON(ID_CHECKBUTTON_FACTORY_SETTINGS)->GetValue() ?
            _L("Hint factory settings on") :
            _L("Hint factory settings off"));
    }
}


bool PageMaster::CurrentSettingsEqualFactory() const
{
    bool color_green = SET::MASTER::color_green.ValueIsDefault();
    bool color_red = SET::MASTER::color_red.ValueIsDefault();
    bool sound = ChooseSoundsDialog::self->CurrentSettingsEqualFactory();
    bool write_config = SET::MASTER::write_config.ValueIsDefault();
    bool offline = OfflineModeDialog::self->CurrentSettingsEqualFactory();
    bool wiegand = WiegandDialog::self->CurrentSettingsEqualFactory();
    bool osdp = OSDPModeDialog::self->CurrentSettingsEqualFactory();
    bool antibreak = AntiBreakDialog::self->CurrentSettingsEqualFactory();
    bool ext_enabled = SET::EXT::enabled.ValueIsDefault();
    bool ext_parity = SET::EXT::parity.ValueIsDefault();
    bool ext_inverse = SET::EXT::inverse_code.ValueIsDefault();
    bool ext_control_bit = SET::EXT::control_bit.ValueIsDefault();
    bool ext_period = SET::EXT::period_autorepeat.ValueIsDefault();
    bool ext_mode_read = SET::EXT::mode_read_card.ValueIsDefault();

    bool result = 
        color_green &&
        color_red &&
        sound &&
        write_config &&
        offline &&
        wiegand &&
        osdp &&
        antibreak &&
        ext_enabled &&
        ext_parity &&
        ext_inverse &&
        ext_control_bit &&
        ext_period &&
        ext_mode_read;

    static bool is_first = true;
    static bool prev = true;

    if (is_first || (result != prev))
    {
        is_first = false;

        GF::FindCheckButton(this, ID_CHECKBUTTON_FACTORY_SETTINGS)->SetToolTip(result ?
            _L("Hint factory settings on") :
            _L("Hint factory settings off"));
    }

    prev = result;

    return result;
}


void PageMaster::ResetToFactory(bool factory)
{
    SET::MASTER::color_red.ResetToFactoryColor(factory, ColorDialog::red->button);
    SET::MASTER::color_green.ResetToFactoryColor(factory, ColorDialog::green->button);
    ChooseSoundsDialog::self->ResetToFactory(factory);
    SET::MASTER::write_config.ResetToFactory(factory);
    OfflineModeDialog::self->ResetToFactory(factory);
    WiegandDialog::self->ResetToFactory(factory);
    OSDPModeDialog::self->ResetToFactory(factory);
    AntiBreakDialog::self->ResetToFactory(factory);
    SET::EXT::enabled.ResetToFactory(factory);
    SET::EXT::parity.ResetToFactory(factory);
    SET::EXT::inverse_code.ResetToFactory(factory);
    SET::EXT::control_bit.ResetToFactory(factory);
    SET::EXT::period_autorepeat.ResetToFactory(factory);
    SET::EXT::mode_read_card.ResetToFactory(factory);
    SET::MISC::disable_less_SL3.ResetToFactory(factory);
}
