// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "defines.h"

enum class DeviceEvent
{
    MODE_CHANGED,
    ERROR_OCCURRED,
    DATA_RECEIVED,
    CONNECTION_LOST,
    GENERATOR_STARTED,
    GENERATOR_STOPPED
};

typedef void (*EventCallback)(DeviceEvent event, void* data);

class CallbackManager
{
public:
    static const uint8_t MAX_CALLBACKS = 10;
    
    static bool RegisterCallback(DeviceEvent event, EventCallback callback);
    static bool UnregisterCallback(DeviceEvent event, EventCallback callback);
    static void TriggerEvent(DeviceEvent event, void* data = nullptr);
    
private:
    struct CallbackEntry
    {
        DeviceEvent event;
        EventCallback callback;
        bool active;
    };
    
    static CallbackEntry callbacks[MAX_CALLBACKS];
    static uint8_t callback_count;
};
