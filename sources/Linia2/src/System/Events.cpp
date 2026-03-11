// 2026/03/11 14:03:34 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "System/Events.h"
#include "Display/Display.h"


void Events::ChangingTheConnectionOfTheMeasuredElement()
{
    Display::self->OnEventCnangeMeasuredElement();
}
