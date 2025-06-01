// 2024/01/21 12:46:11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Device.h"
#include "Panels/CommonPanel.h"
#include "Communicator/ComPort.h"
#include "Utils/Timer.h"
#include "DataBase/DataBaseCards.h"


void Device::Update()
{
    if (CommonPanel::self)
    {
        CommonPanel::self->CallbackOnTimer();
    }

    CardsDB::Update();
}
