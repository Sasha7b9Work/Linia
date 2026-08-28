// 2026/08/28 15:06:42 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once
#include "Settings/Value.h"
#include <map>
#include <string>


#define SETTING(name) SET::GetValue(name)

#define REGISTER(type, name, def) \
    auto* name##_ptr = new Value<type>(#name, def); \
    RegisterValue(#name, new ValueWrapper<type>(name##_ptr));


#define SET_DEBUG_MODE            SETTING("debug_mode")
#define SET_DEBUG_PERIOD_SEND     SETTING("period_send")
#define SET_DEBUG_EMULATE_MODE    SETTING("debug_emulate_mode")

#define SET_GUI_CURRENT_PANEL     SETTING("current_panel")
#define SET_GUI_POS_CONSOLE       SETTING("gui_pos_console")
#define SET_GUI_SIZE_CONSOLE      SETTING("gui_size_console")
#define SET_GUI_MAXIMIZED_CONSOLE SETTING("gui_maximized_console")
#define SET_GUI_COLOR_BACKGROUND  SETTING("gui_color_background")
#define SET_GUI_COLOR_GRID        SETTING("gui_color_grid")
#define SET_GUI_COLOR_FONT        SETTING("gui_color_font")
#define SET_GUI_COLOR_CURVE       SETTING("gui_color_curve")
#define SET_GUI_COLOR_LINK        SETTING("gui_color_link")
#define SET_GUI_COLOR_SECANT      SETTING("gui_color_secant")
#define SET_GUI_SIZE_POINT        SETTING("gui_size_point")
#define SET_GUI_SERIAL_PORT_NUM   SETTING("gui_serial_port_num")


// Настройки прибора
namespace SET
{
    // Базовый класс для всех значений
    class IValue
    {
    public:
        virtual ~IValue() = default;
        virtual void Load() = 0;
        virtual void Save() = 0;
        virtual void Set(bool val) = 0;
        virtual void Set(int val) = 0;
        virtual void Set(unsigned int val) = 0;
        virtual void Set(const wxPoint &val) = 0;
        virtual bool GetBool() const = 0;
        virtual int GetInt() const = 0;
        virtual unsigned int GetUInt() const = 0;
        virtual wxPoint GetPoint() const = 0;
        virtual void *GetRaw() const = 0;
        virtual bool IsBool() const = 0;
        virtual bool IsInt() const = 0;
        virtual bool IsUInt() const = 0;
        virtual bool IsPoint() const = 0;

        virtual ValueUInt *GetValueUInt() = 0;
    };

    // Шаблонная обёртка
    template<typename T>
    class ValueWrapper : public IValue
    {
    public:
        ValueWrapper(Value<T> *val) : value(val)
        {}

        virtual void Load() override
        {
            value->Load();
        }
        virtual void Save() override
        {
            value->Save();
        }

        virtual void Set(bool val) override
        {
            if constexpr (std::is_same_v<T, bool>) value->Set(val);
        }
        virtual void Set(int val) override
        {
            if constexpr (std::is_same_v<T, int>) value->Set(val);
        }
        virtual void Set(unsigned int val) override
        {
            if constexpr (std::is_same_v<T, unsigned int>) value->Set(val);
        }
        virtual void Set(const wxPoint &val) override
        {
            if constexpr (std::is_same_v<T, wxPoint>) value->Set(val);
        }

        virtual bool GetBool() const override
        {
            if constexpr (std::is_same_v<T, bool>)
            {
                return value->Get();
            }
            else
            {
                return false;
            }
        }
        virtual int GetInt() const override
        {
            if constexpr (std::is_same_v<T, int>)
            {
                return value->Get();
            }
            else
            {
                return 0;
            }
        }
        virtual unsigned int GetUInt() const override
        {
            if constexpr (std::is_same_v<T, unsigned int>)
            {
                return value->Get();
            }
            else
            {
                return 0;
            }
        }
        virtual wxPoint GetPoint() const override
        {
            if constexpr (std::is_same_v<T, wxPoint>)
            {
                return value->Get();
            }
            else
            {
                return wxPoint(0, 0);
            }
        }
        virtual bool IsBool() const override
        {
            return std::is_same_v<T, bool>;
        }
        virtual bool IsInt() const override
        {
            return std::is_same_v<T, int>;
        }
        virtual bool IsUInt() const override
        {
            return std::is_same_v<T, unsigned int>;
        }
        virtual bool IsPoint() const override
        {
            return std::is_same_v<T, wxPoint>;
        }
        virtual void *GetRaw() const override
        {
            return (void *)&value->Get();
        }
        Value<T> *Get()
        {
            return value;
        }

        virtual ValueUInt *GetValueUInt() override
        {
            if constexpr (std::is_same_v<T, uint>)
            {
                return (ValueUInt *)value;
            }
            else
            {
                return nullptr;
            }
        }

    private:
        Value<T> *value;
    };

    // Получить значение по имени
    IValue *GetValue(const std::string &name);

    // Установить значение по имени (универсальный)
    template<typename T>
    void SetValue(const std::string &name, const T &value)
    {
        auto *val = GetValue(name);
        if (val)
        {
            val->Set(value);
        }
    }

    void Init();
    void Load();
    void Save();
    void RegisterValue(const std::string &name, IValue *value);
}

#define SETTING(name) SET::GetValue(name)

#define REGISTER(type, name, def) \
    auto* name##_ptr = new Value<type>(#name, def); \
    RegisterValue(#name, new ValueWrapper<type>(name##_ptr));
