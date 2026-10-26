// 2026/10/08 11:47:33 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "GUI/PageTests/MeasurersSourcers.h"
#include "GUI/PageTests/Entities/OStT/OStT.h"


void MeasurersSourcers::Create()
{
    measBase = new MeasurerVoltageCurrent(Chan::B, Dir::Right);

    srcVoltageCurrentBase = new SourceVoltageCurrent(Chan::B, Dir::Right);

    measSubstrate = new MeasurerVoltageCurrent(Chan::S, Dir::Left);

    srcVoltageCurrentSubstrate = new SourceVoltageCurrent(Chan::S, Dir::Left);

    ampCollector = new Ampermeter(Chan::C, Dir::Right);

    voltCollector = new Voltmeter(Chan::C, Dir::Right);

    srcVoltageCollector = new SourceVoltage(Chan::C, Dir::Right);

    scannerCollector = new Scanner(Chan::C, Dir::Right);
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
        return 220;
    }
    else if (num == 0)          // Верхний измеритель
    {
        return 390;
    }
    else if (num == 1)          // Средний измеритель
    {
        return 500;
    }
    else if (num == 2)          // Нижний (источник)
    {
        return 630;
    }

    return 100;
}


void MeasurersSourcers::OnEventChangeCommutator()
{
    ampCollector->FillAllControls();
    srcVoltageCollector->FillAllControls();
    scannerCollector->FillAllControls();
}


void MeasurersSourcers::OnEventChangeScanMode()
{
    ampCollector->FillAllControls();
}
