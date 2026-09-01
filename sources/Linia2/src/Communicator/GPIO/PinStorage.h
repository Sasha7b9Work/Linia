// 2026/07/23 16:19:29 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once


/*
          Стало     Было          Стало    Было
     1                       21 SPI_MISO   STOP
     2                       22 F_CON2     DAT_F2
     3 T13                   23 SPI_CLK    SPI1_CLK
     4                       24 SPICS      DAT_F3
     5 T14                   25 GND
     6 GND                   26 F_CON1     SCICS
     7 T15                   27 ENB_PC     ENB_STM
     8 TX                    28 T18
     9 GND        -          29 T16
    10 RX                    30 GND
    11 KA                    31 EN_DDA1
    12 T17                   32 ST_EXT    REQ_RD
    13 KB                    33 K_STOP    -
    14 GND                   34 GND
    15 K_START               35 EN_DDA2
    16 DT_DDAC  DAT_F0       36 FULL
    17                       37 FIT       -
    18 CLK_DDAC DAT_F1       38
    19          SPI1_MOSI    39 GND
    20 GND      GNDK         40          RDY


       Замены

       Было       Стало
    16 DAT_F0     DT_DDAC
    18 DAT_F1     CLK_DDAC
    22 DAT_F2     F_CON2
    26 SCICS      F_CON1
    27 ENB_STM    ENB_PC
    32 REQ_RD     ST_EXT
    33 -          KN_STOP
    37 -          FIT
    40 RDY        -

        Сделано
    19 SPI1_MOSI  -
    21 STOP       SPI_MISO
    23 SPI1_CLK   SPI_CLK
    24 DAT_F3     SPICS

    SPI0_M2:
    21 MISO
    23 CLK
    24 CS0
*/


// Физические пины на разъёме
class Pin
{
public:

    // Номер пина на разъёме
    enum Num
    {
        _00_none,
        _01_33V,
        _02_5V,
        T13_03__________________FREE,
        _04_5V,
        T14_05__________________FREE,
        _06_GND,
        T15_07__________________FREE,
        _08_UART6_TX_M1,
        _09_GND,
        _10_UART6_RX_M1,
        ENC_A_11,
        T17_12__________________FREE,
        ENC_B_13,
        _14_GND,
        KN_START_15,
        DDAC_MOSI_16_out,
        _17_33V,
        DDAC_CLK_18_out,
        _19_____________________FREE,
        _20_GND,
        _21_SPI0_MISO_M2,
        F_CON2_22,
        _23_SPI0_CLK_M2,
        _24_SPI0_CS0_M2,
        _25_GND,
        F_CON1_26,
        ENB_PC_27,
        _28_____________________FREE,
        _29_____________________FREE,
        _30_GND,
        DDAC_CS0_31_out,
        ST_EXT_32,
        KN_STOP_33,
        _34_GND,
        DDAC_CS1_35_out,
        FULL_36,
        FIT_37,
        _38_____________________FREE,
        _39_GND,
        _40_____________________FREE,
        Count
    };

    explicit Pin(Num _num) : num(_num) {}

    bool GetState() const;

protected:

    Num num;
};


// Структуры для аппаратной информации GPIO (нужны для Linux)
struct gpiod_chip;
struct gpiod_line;


struct HardwarePinInfo
{
    int pin_logical = -666;             // Номер GPIO пина чипа
    const char *chip_name = nullptr;    // Имя GPIO чипа
    gpiod_chip *chip = nullptr;         // Дескриптор чипа
    gpiod_line *line = nullptr;         // Дескриптор линии
};


struct PinInfo
{
    PinInfo(Pin::Num _num, bool _is_input) : pin_num(_num), is_input(_is_input) { }
    HardwarePinInfo hw;
    Pin::Num pin_num = Pin::Count;      // Номер пина на разъёме
    bool is_input = true;               // true, если это входной пин
    bool last_state = false;            // Последнее состояние
};


namespace PinStorage
{
    PinInfo GetPinInfo(Pin::Num, bool is_input);
}
