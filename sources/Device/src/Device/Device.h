// 2025/11/27 15:47:16 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace Device
{
    void Init();

    void Update();

    // По этой команде засылаем во все регистры и DACи нули
    void EmergencyStop();
}
