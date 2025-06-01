// 2023/11/25 13:27:48 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


#define SOUNDS_IN_MEMORY    10  // Столько звуков хранится в памяти считывателя
#define NUMBER_MELODY       3   // Столько мелодий нужно задавать

#define SIZE_FOR_SOUNDS   (872 * 1024 - SOUNDS_IN_MEMORY * 4)      // Столько байт могут занимать звуки

// Генерировать базу карт, если её нет
//#define GENERATE_CARDS_IF_EMPTY
