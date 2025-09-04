// 2025/09/04 16:37:21 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Tests/SettingsTests.h"


bool Category::IsBCE()
{
    E v = Current();

    return (v == BCE_N || v == BCE_P || v == BCSE_N || v == BCSE_P);
}


bool Category::IsGDS()
{
    E v = Current();

    return (v == GDS_N || v == GDS_P || v == GDBS_N || v == GDBS_P);
}
