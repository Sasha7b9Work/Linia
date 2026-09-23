// 2026/08/19 10:59:39 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once
#include "GUI/PageDebug/Notebook/PageChip.h"
#include <thread>

/*
    Страница платы контроллера stm32
*/


class PageSTM32 : public PageChip
{
public:

    PageSTM32(wxNotebook *);

    static PageSTM32 *self;

    // :UPGRADE:START
    void OnUpgradeStart();

    // :UPGRADE:BLOCK <num_block> <size> <crc32>
    void OnConfirmUpgradeBlock(int num_block, int size, uint crc32);

    // :UPGRADE:END <size> <crc32>
    void OnUpgradeEnd(int size, uint crc32);

private:

    void StartUpgrade(pchar);

    void StopUpgrade();

    std::vector<uint8> data;                // Здесь файл для загрузки
    std::thread thread;
    std::atomic<bool> is_running;
    std::atomic<int> current_block;         // Этот блок сейчас загружается
    wxString file_name;

    enum State
    {
        IDLE,
        START_UPGRADE,      // Подтверждён старт обновления
        PROCESS_UPGRADE,    // Находимся в процессе обновления
        END_UPGRADE,        // Подтверждено завершение обновления
        Count
    };

    std::atomic<State> state = IDLE;

    // Если false, то нет данных - буфер слишком мал
    bool CalculateParametersBlock(int num_block, int &offset, int &size, uint &crc32);

    // Перезапустить процесс обновления прошивки
    void ResetUpgrade();

    void SendNextBlock();
};
