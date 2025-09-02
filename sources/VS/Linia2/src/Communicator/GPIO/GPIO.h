#pragma once


struct Pin
{
    enum E
    {
        START,
        STOP,
        DAT_F0,
        DAT_F1,
        DAT_F2,
        DAT_F3,
        REQ_RD,
        FIFO_FULL,
        FIFO_EMPTY,
        Count
    };

    Pin(E v) : value(v) { }

    bool Get() const;
    void Set(bool);

protected:

    E value;
};


struct PinOut : public Pin
{
    PinOut(Pin::E v) : Pin(v) { }
};


struct PinIn : public Pin
{
    typedef void (*ChangeCallback)(bool);

    PinIn(Pin::E v) : Pin(v) { }

    void SetChangeCallback(ChangeCallback);

private:

    ChangeCallback onChange;
};


namespace GPIO
{
    void Init();
}


extern PinIn pinSTART;
extern PinIn pinSTOP;
extern PinIn pinDAT_F0;
extern PinIn pinDAT_F1;
extern PinIn pinDAT_F2;
extern PinIn pinDAT_F3;
extern PinIn pinREQ_RD;
extern PinIn pinFIFO_FULL;
extern PinIn pinFIFO_EMPTY;
