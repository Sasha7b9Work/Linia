// 2025/09/04 16:37:21 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "IPPP/Tests/SettingsTests.h"


bool Category::IsBCE()
{
    E v = Current();

    return (v == BCE_N || v == BCE_P || v == BCSE_N || v == BCSE_P);
}


bool Category::IsGDS()
{
    E v = Current();

    return (v == GDS_N || v == GDS_P || v == GDBS_N || v == GDBS_P);
}


wxString Chan::Name(E chan)
{
    switch (chan)
    {
    case _C: return "C";
    case _B: return "B";
    case _S: return "S";
    case _E: return "E";
    case Count:
    default: return "";
    }
}

wxString StateJack::Name(E state)
{
    switch (state)
    {
    case _C: return "C";
    case _B: return "B";
    case _S: return "S";
    case _E: return "E";
    case General: return "GENERAL";
    case General_1k: return "GENERAL_1K";
    case Break: return "BREAK";
    case Count:
    default: return "BREAK";
    }
}

wxString ModeSource::Name(E mode)
{
    return mode == U ? "U" : "I";
}

wxString ModeMeas::Name(E mode)
{
    return mode == U ? "U" : "I";
}

wxString TypeScan::Name(E type)
{
    switch (type)
    {
    case ImpulsePos: return "IMP_P";
    case ImpulseNeg: return "IMP_N";
    case DCPos: return "DC_P";
    case DCNeg: return "DC_N";
    case SYNPos: return "SYN_P";
    case SYNNeg: return "SYN_N";
    case AC: return "AC";
    case Count:
    default: return "";
    }
}

wxString FirstQueue::Name(E fq)
{
    return fq == ChannelC ? "C" : "B";
}

wxString GenerationStup::Name(E gs)
{
    return gs == Enable ? "1" : "0";
}

wxString PointCount::Name(E points)
{
    return points == Points_20 ? "20" : "50";
}


wxString StartTrigger::Name(E trigger)
{
    return trigger == External ? "EXT" : "INT";
}


wxString StartMode::Name(E mode)
{
    return mode == Multiple ? "MULT" : "SINGLE";
}


wxString MeasMode::Name(E mode)
{
    switch (mode)
    {
    case Manual: return "MANUAL";
    case Auto_1: return "AUTO_1";
    case Auto_2: return "AUTO_2";
    case Auto_3: return "AUTO_3";
    case Auto_4: return "AUTO_4";
    case Auto_5: return "AUTO_5";
    case Count:
    default: return "MANUAL";
    }
}


wxString DacCode::Name(E code)
{
    switch (code)
    {
    case Code_100: return "100";
    case Code_110: return "110";
    case Code_120: return "120";
    case Code_130: return "130";
    case Code_140: return "140";
    case Code_150: return "150";
    case Count:
    default: return "130";
    }
}

uint8_t DacCode::Value(E code)
{
    switch (code)
    {
    case Code_100: return 100;
    case Code_110: return 110;
    case Code_120: return 120;
    case Code_130: return 130;
    case Code_140: return 140;
    case Code_150: return 150;
    case Count:
    default: return 130;
    }
}


wxString PulseState::Name(E state)
{
    return state == Enabled ? "ON" : "OFF";
}


wxString ElementType::Name(E type)
{
    return type == Multipole ? "MULTI" : "TWO";
}


wxString LimitResult::Name(E result)
{
    switch (result)
    {
    case Pass: return "PASS";
    case Fail: return "FAIL";
    case NotReached: return "NOT_REACHED";
    case Protection: return "PROTECTION";
    case Count:
    default: return "PASS";
    }
}


wxString PowerNominal::Name(E nominal)
{
    return nominal == Nominal_110V ? "110V" : "220V";
}


wxString PulseDuration::Name(E duration)
{
    switch (duration)
    {
    case Duration_50us: return "50us";
    case Duration_100us: return "100us";
    case Duration_200us: return "200us";
    case Duration_300us: return "300us";
    case Duration_400us: return "400us";
    case Duration_500us: return "500us";
    case Duration_1000us: return "1000us";
    case Count:
    default: return "200us";
    }
}

uint PulseDuration::Value(E duration)
{
    switch (duration)
    {
    case Duration_50us: return 50;
    case Duration_100us: return 100;
    case Duration_200us: return 200;
    case Duration_300us: return 300;
    case Duration_400us: return 400;
    case Duration_500us: return 500;
    case Duration_1000us: return 1000;
    case Count:
    default: return 200;
    }
}


wxString LimitPercent::Name(E percent)
{
    switch (percent)
    {
    case Percent_0: return "0%";
    case Percent_5: return "5%";
    case Percent_10: return "10%";
    case Percent_15: return "15%";
    case Percent_20: return "20%";
    case Percent_25: return "25%";
    case Percent_30: return "30%";
    case Percent_35: return "35%";
    case Percent_40: return "40%";
    case Percent_45: return "45%";
    case Percent_50: return "50%";
    case Percent_55: return "55%";
    case Percent_60: return "60%";
    case Percent_65: return "65%";
    case Percent_70: return "70%";
    case Percent_75: return "75%";
    case Percent_80: return "80%";
    case Percent_85: return "85%";
    case Percent_90: return "90%";
    case Percent_95: return "95%";
    case Percent_100: return "100%";
    case Count:
    default: return "0%";
    }
}

int LimitPercent::Value(E percent)
{
    return (int)percent * 5;
}


wxString AmplitudeRange::Name(E range)
{
    switch (range)
    {
    case Range_200mV: return "200mV";
    case Range_2V: return "2V";
    case Range_20V: return "20V";
    case Range_200V: return "200V";
    case Count:
    default: return "200mV";
    }
}

uint8_t AmplitudeRange::Value(E range)
{
    return (uint8_t)range;
}


wxString StepCount::Name(E count)
{
    switch (count)
    {
    case Steps_1: return "1";
    case Steps_10: return "10";
    case Steps_20: return "20";
    case Steps_50: return "50";
    case Steps_100: return "100";
    case Steps_200: return "200";
    case Steps_500: return "500";
    case Steps_1000: return "1000";
    case Count:
    default: return "1";
    }
}

int StepCount::Value(E count)
{
    switch (count)
    {
    case Steps_1: return 1;
    case Steps_10: return 10;
    case Steps_20: return 20;
    case Steps_50: return 50;
    case Steps_100: return 100;
    case Steps_200: return 200;
    case Steps_500: return 500;
    case Steps_1000: return 1000;
    case Count:
    default: return 1;
    }
}


wxString AmplitudeValue::Name(E value)
{
    switch (value)
    {
    case Value_N32767: return "-32767";
    case Value_N30000: return "-30000";
    case Value_N20000: return "-20000";
    case Value_N10000: return "-10000";
    case Value_N5000: return "-5000";
    case Value_N1000: return "-1000";
    case Value_N500: return "-500";
    case Value_N100: return "-100";
    case Value_N50: return "-50";
    case Value_N10: return "-10";
    case Value_N1: return "-1";
    case Value_0: return "0";
    case Value_1: return "1";
    case Value_10: return "10";
    case Value_50: return "50";
    case Value_100: return "100";
    case Value_500: return "500";
    case Value_1000: return "1000";
    case Value_5000: return "5000";
    case Value_10000: return "10000";
    case Value_20000: return "20000";
    case Value_30000: return "30000";
    case Value_32767: return "32767";
    case Count:
    default: return "0";
    }
}

int16_t AmplitudeValue::Value(E value)
{
    switch (value)
    {
    case Value_N32767: return -32767;
    case Value_N30000: return -30000;
    case Value_N20000: return -20000;
    case Value_N10000: return -10000;
    case Value_N5000: return -5000;
    case Value_N1000: return -1000;
    case Value_N500: return -500;
    case Value_N100: return -100;
    case Value_N50: return -50;
    case Value_N10: return -10;
    case Value_N1: return -1;
    case Value_0: return 0;
    case Value_1: return 1;
    case Value_10: return 10;
    case Value_50: return 50;
    case Value_100: return 100;
    case Value_500: return 500;
    case Value_1000: return 1000;
    case Value_5000: return 5000;
    case Value_10000: return 10000;
    case Value_20000: return 20000;
    case Value_30000: return 30000;
    case Value_32767: return 32767;
    case Count:
    default: return 0;
    }
}


wxString LimitThreshold::Name(E threshold)
{
    return AmplitudeValue::Name((AmplitudeValue::E)threshold);
}

int16_t LimitThreshold::Value(E threshold)
{
    return AmplitudeValue::Value((AmplitudeValue::E)threshold);
}
