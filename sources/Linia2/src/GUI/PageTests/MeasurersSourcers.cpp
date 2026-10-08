// 2026/10/08 11:47:33 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "GUI/PageTests/MeasurersSourcers.h"


void MeasurersSourcers::Create()
{
    measBase = new MeasurerVoltageCurrent(Chan::B, Dir::Down);

    srcVoltageCurrentBase = new SourceVoltageCurrent(Chan::B, Dir::Down);

    measSubstrate = new MeasurerVoltageCurrent(Chan::S, Dir::Down);

    srcVoltageCurrentSubstrate = new SourceVoltageCurrent(Chan::S, Dir::Down);

    ampCollector = new Ampermeter(Chan::C, Dir::Down);

    voltCollector = new Voltmeter(Chan::C, Dir::Down);

    srcVoltageCollector = new SourceVoltage(Chan::C, Dir::Down);
}


void MeasurersSourcers::Hide()
{
    measBase->Hide();
    srcVoltageCurrentBase->Hide();

    measSubstrate->Hide();
    srcVoltageCurrentSubstrate->Hide();

    ampCollector->Hide();
    voltCollector->Hide();
    srcVoltageCollector->Hide();
}


void MeasurersSourcers::Tune()
{
    measBase->ShowNeedParameters();
    srcVoltageCurrentBase->ShowNeedParameters();

    measSubstrate->ShowNeedParameters();
    srcVoltageCurrentSubstrate->ShowNeedParameters();

    ampCollector->ShowNeedParameters();
    voltCollector->ShowNeedParameters();
    srcVoltageCollector->ShowNeedParameters();
}


void MeasurersSourcers::Show(AutoBufferedPaintDC &dc, const Chan &ch, int x)
{
    if (ch.IsBase())
    {
        measBase->Show(dc, { x, PosY(1) });
        srcVoltageCurrentBase->Show(dc, { x, PosY(2) });
    }
    else if (ch.IsSubstrate())
    {
        measSubstrate->Show(dc, { x, PosY(1) });
        srcVoltageCurrentSubstrate->Show(dc, { x, PosY(2) });
    }
    else if (ch.IsCollector())
    {
        ampCollector->Show(dc, { x, PosY(0) });
        voltCollector->Show(dc, { x, PosY(1) });
        srcVoltageCollector->Show(dc, { x, PosY(2) });
    }
}


int MeasurersSourcers::PosY(int num)
{
    if (num == 0)
    {
        return 250;
    }
    else if (num == 1)
    {
        return 380;
    }
    else if (num == 2)
    {
        return 510;
    }

    return 100;
}
