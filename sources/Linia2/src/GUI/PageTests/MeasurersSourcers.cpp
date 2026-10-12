// 2026/10/08 11:47:33 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "GUI/PageTests/MeasurersSourcers.h"
#include "GUI/PageTests/Entities/OStT/OStT.h"


void MeasurersSourcers::Create()
{
    measBase = new MeasurerVoltageCurrent(Chan::B, Dir::Down);

    srcVoltageCurrentBase = new SourceVoltageCurrent(Chan::B, Dir::Down);

    measSubstrate = new MeasurerVoltageCurrent(Chan::S, Dir::Down);

    srcVoltageCurrentSubstrate = new SourceVoltageCurrent(Chan::S, Dir::Down);

    ampCollector = new Ampermeter(Chan::C, Dir::Down);

    voltCollector = new Voltmeter(Chan::C, Dir::Down);

    srcVoltageCollector = new SourceVoltage(Chan::C, Dir::Down);

    scannerCollector = new Scanner(Chan::C, Dir::Down);
}


void MeasurersSourcers::HideAll()
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
    wxPoint point;

    if (OStT::current->GetPoint_B(point))
    {
        measBase->ShowNeedParameters();
        srcVoltageCurrentBase->ShowNeedParameters();
    }

    if (OStT::current->GetPoint_S(point))
    {
        measSubstrate->ShowNeedParameters();
        srcVoltageCurrentSubstrate->ShowNeedParameters();
    }

    ampCollector->ShowNeedParameters();
    voltCollector->ShowNeedParameters();
    srcVoltageCollector->ShowNeedParameters();
}


void MeasurersSourcers::Draw(AutoBufferedPaintDC &dc, const Chan &ch, int x)
{
    if (ch.IsBase())
    {
        measBase->Draw(dc, { x, PosY(1) });
        srcVoltageCurrentBase->Draw(dc, { x, PosY(2) });
    }
    else if (ch.IsSubstrate())
    {
        measSubstrate->Draw(dc, { x, PosY(1) });
        srcVoltageCurrentSubstrate->Draw(dc, { x, PosY(2) });
    }
    else if (ch.IsCollector())
    {
        ampCollector->Draw(dc, { x, PosY(0) });
        voltCollector->Draw(dc, { x, PosY(1) });
        srcVoltageCollector->Draw(dc, { x, PosY(2) });
        scannerCollector->Draw(dc, { x, PosY(-1) });
    }
}


int MeasurersSourcers::PosY(int num)
{
    if (num == -1)              // Развёртка
    {
        return 270;
    }
    else if (num == 0)          // Верхний измеритель
    {
        return 390;
    }
    else if (num == 1)          // Средний измеритель
    {
        return 480;
    }
    else if (num == 2)          // Нижний (источник)
    {
        return 570;
    }

    return 100;
}
