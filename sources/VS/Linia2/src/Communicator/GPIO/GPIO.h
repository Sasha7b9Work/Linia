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

class Pin {
public:
    enum Type {
        START,
        STOP,
        DAT_F0,
        DAT_F1,
        DAT_F2,
        DAT_F3,
        FIFO_FULL,
        REQ_RD,
        Count
    };
    explicit Pin(Type type) : type_(type)
    {
    }
    Type type() const
    {
        return type_;
    }

    bool Get() const;

protected:
    Type type_;
};

class PinIn : public Pin {
public:
    using Pin::Pin;
    using ChangeCallback = std::function<void(bool)>;
    void SetChangeCallback(ChangeCallback cb);
private:
    ChangeCallback callback_;
};

class PinOut : public Pin {
public:
    using Pin::Pin;

    void Set(bool state);
};

namespace GPIO {
    void Init();
    void DeInit();

    // Вспомогательные функции
    InputPinInfo *GetInputPinInfo(Pin::Type type);
    OutputPinInfo *GetOutputPinInfo(Pin::Type type);
}



// Категория: входные пины
extern PinIn pinSTART;
extern PinIn pinSTOP;
extern PinIn pinDAT_F0;
extern PinIn pinDAT_F1;
extern PinIn pinDAT_F2;
extern PinIn pinDAT_F3;
extern PinIn pinFIFO_FULL;

// Категория: выходные пины
extern PinOut pinREQ_RD;
