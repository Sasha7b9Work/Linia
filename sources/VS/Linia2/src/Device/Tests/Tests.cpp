// 2025/08/19 14:10:57 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Device/Tests/Tests.h"
#include "Panels/Panel03_Config/PanelScheme/PanelScheme.h"


Category::E Category::Current()
{
    return (E)PanelScheme::self->comboCategory->GetCurrentChoice();
}


void Test::Load(pchar /*file_name*/)
{
    // CLineDlg::ExtractParamFromTst()
    // CLineDlg::SaveParamToTst(int where) 
}