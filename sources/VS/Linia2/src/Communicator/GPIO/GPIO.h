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
        OUT_1,
        OUT_2,
        OUT_3,
        OUT_4,
        OUT_5,
        OUT_6,
        OUT_7,
        OUT_8,
        OUT_9,
        Count
    };

    Pin(E v) : value(v) { }

    bool Get() const;
    void Set(bool);

    E GetValue() const
    {
        return value;
    }

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

    ChangeCallback onChange = nullptr;
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

extern PinOut pinOUT1;
extern PinOut pinOUT2;
extern PinOut pinOUT3;
extern PinOut pinOUT4;
extern PinOut pinOUT5;
extern PinOut pinOUT6;
extern PinOut pinOUT7;
extern PinOut pinOUT8;
extern PinOut pinOUT9;
