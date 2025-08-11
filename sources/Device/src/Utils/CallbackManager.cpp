// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "CallbackManager.h"

namespace Log
{
    void AddString(char *format, ...);
}c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "CallbackManager.h"
#include "Log_.h"

CallbackManager::CallbackEntry CallbackManager::callbacks[MAX_CALLBACKS];
uint8_t CallbackManager::callback_count = 0;

bool CallbackManager::RegisterCallback(DeviceEvent event, EventCallback callback)
{
    if (callback_count >= MAX_CALLBACKS)
    {
        Log::AddString("Cannot register callback - limit reached");
        return false;
    }
    
    callbacks[callback_count].event = event;
    callbacks[callback_count].callback = callback;
    callbacks[callback_count].active = true;
    callback_count++;
    
    Log::AddString("Callback registered for event %d", static_cast<int>(event));
    return true;
}

bool CallbackManager::UnregisterCallback(DeviceEvent event, EventCallback callback)
{
    for (uint8_t i = 0; i < callback_count; i++)
    {
        if (callbacks[i].event == event && callbacks[i].callback == callback)
        {
            callbacks[i].active = false;
            return true;
        }
    }
    return false;
}

void CallbackManager::TriggerEvent(DeviceEvent event, void* data)
{
    for (uint8_t i = 0; i < callback_count; i++)
    {
        if (callbacks[i].active && callbacks[i].event == event)
        {
            callbacks[i].callback(event, data);
        }
    }
}
