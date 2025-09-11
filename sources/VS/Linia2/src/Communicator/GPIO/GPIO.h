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
        START,          // Кнопка СТАРТ
        STOP,           // Кнопка СТОП
        DAT_F0,
        DAT_F1,
        DAT_F2,
        DAT_F3,
        FIFO_FULL,
        REQ_RD,
        KA,             // 11  Энкодер А
        KB,             // 13  Энкодер B
//        FIFO_EMPTY,   Конт. 26
//        ENB_PC,       27
//        RDY,          40
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
private:
    ChangeCallback callback_;
};


class PinOut : public Pin
{
public:

    using Pin::Pin;

    void Set(bool state);
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
