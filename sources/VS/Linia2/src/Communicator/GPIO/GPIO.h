#pragma once


// Структуры для аппаратной информации GPIO (нужны для Linux)
struct gpiod_chip;
struct gpiod_line;


struct HardwarePinInfo
{
    int pin_number;          // Номер GPIO пина  
    const char *chip_name;   // Имя GPIO чипа
    gpiod_chip *chip;        // Дескриптор чипа
    gpiod_line *line;        // Дескриптор линии
};


struct InputPinInfo
{
    HardwarePinInfo hw;
    bool last_state;         // Последнее состояние
    std::function<void(bool)> callback; // Callback функция
};


struct OutputPinInfo
{
    HardwarePinInfo hw;
};


class Pin
{
public:
    enum E
    {
        In_START,       // Кнопка СТАРТ
        In_STOP,        // Кнопка СТОП
        In_DAT_F0,      //
        In_DAT_F1,      //
        In_DAT_F2,      //
        In_DAT_F3,      //
        In_FIFO_FULL,   //
        Out_REQ_RD,     //
        In_KA,          // 11 GPIO1_A4
        In_KB,          // 13 GPIO1_A7
        Out_SPI_CS,     // 26 GPIO1_B5
        Count
    };
    explicit Pin(E type) : type_(type) { }

    E type() const
    {
        return type_;
    }

    bool Get() const;

protected:
    E type_;
};


class PinIn : public Pin
{
public:

    using Pin::Pin;

    using ChangeCallback = std::function<void(bool)>;

    void SetChangeCallback(ChangeCallback);

    static bool GetHardware(gpiod_line *);
    static float TimeGetAverage();

private:

    ChangeCallback callback_;
};


class PinOut : public Pin
{
public:

    using Pin::Pin;

    void Set(bool state);

    void ToLow();

    void ToHi();

    static void Set(gpiod_line *, int);
    static float TimeSetAverage();
};

namespace GPIO
{
    void Init();
    void DeInit();

    InputPinInfo *GetInputPinInfo(Pin::E);
    OutputPinInfo *GetOutputPinInfo(Pin::E);
}


extern PinIn pinDAT_F0;
extern PinIn pinDAT_F1;
extern PinIn pinDAT_F2;
extern PinIn pinDAT_F3;
extern PinIn pinFIFO_FULL;

extern PinIn pinSTART;
extern PinIn pinSTOP;
extern PinIn pinKA;
extern PinIn pinKB;

extern PinOut pinREQ_RD;
extern PinOut pinSPI_CS;
