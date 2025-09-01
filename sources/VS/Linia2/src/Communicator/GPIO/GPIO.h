#pragma once


struct Pin
{
    typedef void (*ChangeCallback)(bool);

    enum E
    {
        START,
        STOP,
        EN_DDA1,
        EN_DDA2,
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

    void SetChangeCallback(ChangeCallback);

private:

    E value;

    ChangeCallback onChange;
};


namespace GPIO
{
    void Init();
}


extern Pin pinSTART;
extern Pin pinSTOP;
extern Pin pinEN_DDA1;
extern Pin pinEN_DDA2;
extern Pin pinDAT_F0;
extern Pin pinDAT_F1;
extern Pin pinDAT_F2;
extern Pin pinDAT_F3;
extern Pin pinREQ_RD;
extern Pin pinFIFO_FULL;
extern Pin pinFIFO_EMPTY;
