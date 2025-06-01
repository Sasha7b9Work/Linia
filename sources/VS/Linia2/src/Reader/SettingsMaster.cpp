// 2024/05/22 16:20:18 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Reader/SettingsMaster.h"
#include "Utils/StringUtils.h"
#include <cstring>


void SettingsMaster::EnableOSDP(bool enable)
{
    if (enable)
    {
        s09.bytes[0] |= 0x80;
    }
    else
    {
        s09.bytes[0] &= ~0x80;
    }
}


void SettingsMaster::SetMelody(TypeSound::E type, uint8 num)
{
    uint16 half_word = s09.half_word[1];

    half_word &= ~(0x0F << (type * 4));

    half_word |= (num << (type * 4));

    s09.half_word[1] = half_word;
}


void SettingsMaster::SetVolume(TypeSound::E type, uint8 num)
{
    uint8 byte = s09.bytes[1];

    byte &= ~(0x03 << (type * 2));

    byte |= (num << (type * 2));

    s09.bytes[1] = byte;
}


BaudRate SettingsMaster::BaudRateOSDP() const
{
    uint8 baudrate = (uint8)(s13.bytes[3] & 0x7f);       // Потому что в старшем бите хранится информация о режиме ЭКО

    return BaudRate((BaudRate::E)baudrate);
}


bool SettingsMaster::IsEnabledLPCD() const
{
    return (s13.bytes[3] & 0x80) != 0;
}


void SettingsMaster::SetEnabledLPCD(bool enabled)
{
    if (enabled)
    {
        s13.bytes[3] |= 0x80;
    }
    else
    {
        s13.bytes[3] &= 0x7F;
    }
}


void SettingsMaster::SetBaudRateOSDP(BaudRate::E b)
{
    s13.bytes[3] &= 0x80;       // В старшем бите хранится режим ЭКО
    s13.bytes[3] |= (uint8)b;
}


#ifdef WIN32
#pragma warning(push)
#pragma warning(disable : 4310)
#endif


void SettingsMaster::SetWiegand(uint value)
{
    s09.bytes[0] &= 0x80;                           // Очищаем младшие семь бит

    s09.bytes[0] |= (uint8)(value & 0x7F);          // Записываем младшие семь бит

    //                         65432
    s15.bytes[0] &= BINARY_U8(10000011);            // Очишаем биты настроек Wiegand

    if (_GET_BIT(value, 9))  _SET_BIT(s15.bytes[0], 6);   // Если контрольные биты включены, то устанавливаем бит
    if (_GET_BIT(value, 10)) _SET_BIT(s15.bytes[0], 5);   // Если инверсный расчёт котрольных бит
    if (_GET_BIT(value, 8))  _SET_BIT(s15.bytes[0], 4);   // Если передём полный GUID ВСЕГДА (точнее, сколько поместится)
    if (_GET_BIT(value, 11)) _SET_BIT(s15.bytes[0], 3);   // Обратный порядок передачи бит - от младшего к старшему
    if (_GET_BIT(value, 12)) _SET_BIT(s15.bytes[0], 2);   // Отбрасывать старший байт от 4-байтного UID
}


uint SettingsMaster::GetWiegand() const
{
    uint value = (uint)(s09.bytes[0] & 0x7F);

    const uint8 byte = s15.bytes[0];

    if (_GET_BIT(byte, 6)) _SET_BIT(value, 9);
    if (_GET_BIT(byte, 5)) _SET_BIT(value, 10);
    if (_GET_BIT(byte, 4)) _SET_BIT(value, 8);
    if (_GET_BIT(byte, 3)) _SET_BIT(value, 11);
    if (_GET_BIT(byte, 2)) _SET_BIT(value, 12);

    return value;
}


#ifdef WIN32
#pragma warning(pop)
#endif


uint8 SettingsMaster::GetWiegandValue() const
{
    uint8 raw = (uint8)(s09.bytes[0] & 0x7F);

    if (raw < 7)                    // \todo Старая система - где тип вейганд выбирался из 5 возможных
    {                               // Удалить при глобальном обновлении
        struct Wiegand
        {
            enum E
            {
                _26,
                _33,
                _34,
                _37,
                _40,
                _42,
                Count
            };

            Wiegand(E v) : value(v)
            {
            }

            int ToRAW() const
            {
                static const int raws[Count] =
                {
                    26,
                    33,
                    34,
                    37,
                    40,
                    42
                };

                return raws[value];
            }

            E value;
        };

        Wiegand weig((Wiegand::E)raw);

        return (uint8)weig.ToRAW();
    }

    return raw;
}


bool SettingsMaster::IsWiegandControlBitsEnabled() const
{
    return (s15.bytes[0] & (1 << 6)) != 0;
}


bool SettingsMaster::IsWiegandControlBitsParityStandard() const
{
    return (s15.bytes[0] & (1 << 5)) == 0;
}


bool SettingsMaster::IsWiegandFullGUID() const
{
    return (s15.bytes[0] & (1 << 4)) != 0;
}


bool SettingsMaster::IsWiegandReverseOrderBits() const
{
    return (s15.bytes[0] & (1 << 3)) != 0;
}


bool SettingsMaster::IsWiegnadDiscard_NUID_LSB() const
{
    return (s15.bytes[0] & (1 << 2)) != 0;
}


uint64 SettingsMaster::OldPassword() const
{
    BitSet64 password;

    password.word[0] = s05.word;
    password.word[1] = s06.word;

    return password.long_word;
}


void SettingsMaster::SetOldPassword(uint64 password)
{
    BitSet64 bs(password);

    s05.word = bs.word[0];
    s06.word = bs.word[1];
}


void SettingsMaster::SetPassword(uint64 password)
{
    BitSet64 bs(password);

    s07.word = bs.word[0];
    s08.word = bs.word[1];
}


bool SettingsMaster::CheckSumIsMatches()
{
    uint crc1 = CRC32();
    uint crc2 = CalculateCRC32();

    return (crc1 == crc2);
}


uint SettingsMaster::CalculateCRC32() const
{
    const uint8 *begin = (const uint8 *)this;
    const uint8 *end = (const uint8 *)&s20;
    int size = end - begin;

    return SU::CalculateCRC32(begin, size);
}


uint64 SettingsMaster::Password() const
{
    BitSet64 password;

    password.word[0] = s07.word;
    password.word[1] = s08.word;

    return password.long_word;
}


uint SettingsMaster::GetAntibreakNumber() const
{
    uint number = 0;

    std::memcpy(&number, &s14.bytes[1], 3);

    return number;
}


void SettingsMaster::SetOnlySL3(bool only)
{
    if (only)
    {
        s15.bytes[0] |= 0x80;
    }
    else
    {
        s15.bytes[0] &= 0x7F;
    }
}


bool SettingsMaster::GetOnlySL3() const
{
    return (s15.bytes[0] & 0x80) != 0;
}


uint BaudRate::ToRAW() const
{
    static const uint raws[Count] =
    {
        9600,
        19200,
        38400
    };

    return raws[value];
}


bool SettingsMaster::IsEquals(const SettingsMaster &s) const
{
    const uint *pointer1 = &s04.word;
    const uint *pointer2 = &s.s04.word;

    for (int i = 0; i < 6; i++)
    {
        if (*pointer1 != *pointer2)
        {
            return false;
        }

        pointer1++;
        pointer2++;
    }

    return true;
}
