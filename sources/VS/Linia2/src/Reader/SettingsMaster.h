// 2024/05/22 16:20:10 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Settings/Color.h"


struct BaudRate
{
    enum E
    {
        _9600,
        _19200,
        _38400,
        Count
    };

    explicit BaudRate(E v) : value(v) { }

    uint ToRAW() const;

    static E FromUInt(uint);

    operator E() { return value; }

private:

    E value;
};


struct TypeSound
{
    enum E
    {
        Beep,
        Green,
        Red,
        Count
    };
};


// Настройки мастер-карты.
// Они же - настройки считывателя
struct SettingsMaster
{
    BitSet32 s04;   // Для признака мастер-карты
    BitSet32 s05;   // Старый пароль. Должен совпадать с установленным сейчас
    BitSet32 s06;   // на считывателе. 0xFF, если карта меняет только пароль
    BitSet32 s07;   // Новый пароль. Будет установлен на считыватель
    BitSet32 s08;   // после отработки мастер-карты
    BitSet32 s09;   // WG[0], oсдп-режим, громкости[1], мелодии[2...3]
    BitSet32 s10;   // Байт на номер, первый блок номера
    BitSet32 s11;   // "Красный" цвет
    BitSet32 s12;   // "Зелёный" цвет
    BitSet32 s13;   // Время блокировки замка, время тревоги, адрес OSDP, baud rate OSDP
    BitSet32 s14;   // Датчик отрыва
    BitSet32 s15;
    BitSet32 s16;
    BitSet32 s17;
    BitSet32 s18;
    BitSet32 s19;
    BitSet32 s20;   // Контрольная сумма (всех предыдущих значений)

    // Подготавливает структуру для записи мастер-карты установки только пароля
    void     PrepareMasterOnlyPassword(uint64 new_password);

    // Возвращает размер в байтах
    int      Size() const { return sizeof(*this); }
    uint64   OldPassword() const;
    uint64   Password() const;
    void     SetPassword(uint64);
    void     SetOldPassword(uint64);
    Color    ColorRed() const { return Color::FromUint(s11.word); }
    void     SetColorRed(const Color &color) { s11.word = color.value; }
    Color    ColorGreen() const { return Color::FromUint(s12.word); }
    void     SetColorGreen(const Color &color) { s12.word = color.value; }
    void     EnableOSDP(bool);
    bool     IsEnabledOSDP() const { return (s09.bytes[0] & 0x80) != 0; }
    uint8    GetWiegandValue() const;
    void     SetWiegand(uint);                                  // Сюда пишем значение из команды, в котором и размерность, и информациия о контрольных битах
    uint     GetWiegand() const;
    void     SetOnlySL3(bool);
    bool     GetOnlySL3() const;
    uint8    Melody(TypeSound::E type) const
    {
        int shift = (int)type * 4;

        return (uint8)((s09.half_word[1] >> shift) & 0x0F);
    }
    void     SetMelody(TypeSound::E, uint8);
    uint8    Volume(TypeSound::E type) const
    {
        int shift = (int)type * 2;

        return (uint8)((s09.bytes[1] >> shift) & 0x03);
    }
    void     SetVolume(TypeSound::E, uint8);
    uint8    SizeNumber() const { return s10.bytes[0]; }
    void     SetSizeNumber(uint8 s) { s10.bytes[0] = s; }
    uint8    BeginNumber() const { return s10.bytes[1]; }
    void     SetBeginNumber(uint8 b) { s10.bytes[1] = b; }
    uint     TimeLock() const { return (uint)s13.bytes[0] * 1000; }
    void     SetTimeLock(uint8 t) { s13.bytes[0] = t; }
    uint     TimeAlarm() const { return (uint)s13.bytes[1] * 1000; }
    void     SetTimeAlarm(uint8 t) { s13.bytes[1] = t; }
    bool     IsSecurityModeEnabled() const { return (s10.bytes[2] & 2) != 0; }
    void     SetSecurityModeEnabled(bool en) { en ? (s10.bytes[2] |= 2) : (s10.bytes[2] &= ~2); }
    bool     IsOfflineModeAldowed() const { return (s10.bytes[2] & 1) != 0; }                    // Разрешено использование в автономном режиме (номера разрешённых
    void     SetOfflineModeAllowed(bool en) { en ? (s10.bytes[2] |= 1) : (s10.bytes[2] &= ~1); }   // карт хранятся в считывателе)
    uint8    AddressOSDP() const { return s13.bytes[2]; }
    void     SetAddressOSDP(uint8 address) { s13.bytes[2] = address; }
    BaudRate BaudRateOSDP() const;
    void     SetBaudRateOSDP(BaudRate::E b);
    bool     IsEnabledLPCD() const;
    void     SetEnabledLPCD(bool);

    // s14
    uint8    GetAntibreakSensRAW() const { return (uint8)(s14.bytes[0] & 0x0F); }
    uint     GetAntibreakNumber() const;

    // s15
    bool     IsWiegandControlBitsEnabled() const;           // Если false - контрольные биты не передаются
    bool     IsWiegandControlBitsParityStandard() const;    // Если true, то такое же, как для стандартного WG26, иначе - стартовый и конечный биты рассчитываются наоборот
    bool     IsWiegandFullGUID() const;                     // Если true, то всегда передаётся полный GUID (вернее, столько бит, сколько поместится)
    bool     IsWiegandReverseOrderBits() const;
    bool     IsWiegnadDiscard_NUID_LSB() const;

    bool IsEquals(const SettingsMaster &) const;

    static void Load();

    static void Save();

    // Сброс настроек на заводские
    static void ResetToFactory();

    struct PSWD
    {
        // Возвращает текущий пароль
        static uint64 Get();

        // Установка нового пароля с мастер-карты
        static void Set(uint64);

        static uint64 GetFactory();
    };

    bool CheckSumIsMatches();
    void CalculateAndWriteCheckSum();

private:
    uint  CalculateCRC32() const;        // Рассчитывает CRC32 (новая методология)
    uint &CRC32() { return s20.word; } //-V524
};
