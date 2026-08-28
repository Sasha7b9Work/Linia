// 2023/08/09 19:11:58 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "Settings/Settings.h"
#include "Application.h"


template void SET::AppendValue<ValueBool>(ValueBool *);
template void SET::AppendValue<ValuePoint>(ValuePoint *);
template void SET::AppendValue<ValueInt>(ValueInt *);
template void SET::AppendValue<ValueUInt>(ValueUInt *);


namespace SET
{
    ValueBool *debug_mode = nullptr;
    ValueBool *emulate_mode = nullptr;

    namespace GUI
    {
        ValuePoint       *pos_console = nullptr;
        ValuePoint       *size_console = nullptr;
        ValueBool        *maximized_console = nullptr;
        ValueInt         *serial_port_num = nullptr;

        ValueUInt        *color_background = nullptr;
        ValueUInt        *color_grid = nullptr;
        ValueUInt        *color_font = nullptr;
        ValueUInt        *color_curve = nullptr;
        ValueUInt        *color_link = nullptr;
        ValueUInt        *color_secant = nullptr;

        ValueInt         *size_point = nullptr;
        ValueInt         *current_panel = nullptr;
    }

    namespace _DEBUG
    {
        ValueUInt *period_send = nullptr;
    }


    class IVector
    {
    public:

        virtual ~IVector() = default;

        virtual void Load() = 0;

        virtual void Save() = 0;
    };

    template<class type>
    class Vector : public IVector
    {
    public:

        Vector();

        void Push(type *value)
        {
            vec.push_back(value);
        }

        virtual void Load() override
        {
            for (auto elem : vec)
            {
                elem->Load();
            }
        }

        virtual void Save() override
        {
            for (auto elem : vec)
            {
                elem->Save();
            }
        }

    private:

        std::vector<type *> vec;
    };

    static Vector<ValueBool> vec_bool;
    static Vector<ValuePoint> vec_point;
    static Vector<ValueInt> vec_int;
    static Vector<ValueUInt> vec_uint;

    static std::vector<IVector *> all_vectors;

    // Этот метод нужен для того, чтобы порядок инициализации статических объектов всегда был правильным -
    // all_vector создавался раньше, чем в него будет добавлен первый объект
    static std::vector<IVector *> &GetAllVectors()
    {
        return all_vectors;
    }

    template<class type> Vector<type>::Vector()
    {
        GetAllVectors().push_back(this);
    }
}


void SET::Init()
{
    debug_mode = new ValueBool("debug_mode", false);
    emulate_mode = new ValueBool("emulate_mode", false);

    GUI::pos_console = new ValuePoint("gui_pos_console", { 10, 10 });
    GUI::size_console = new ValuePoint("gui_size_console", { 400, 400 });
    GUI::maximized_console = new ValueBool("gui_maximized_console", false);
    GUI::serial_port_num = new ValueInt("serial_port_num", 0);

    GUI::color_background = new ValueUInt("color_background", 0xFFFFFF);
    GUI::color_grid = new ValueUInt("color_grid", 0);
    GUI::color_font = new ValueUInt("color_font", 0);
    GUI::color_curve = new ValueUInt("color_curve", 0);
    GUI::color_link = new ValueUInt("color_link", 0);
    GUI::color_secant = new ValueUInt("color_secant", 0);

    GUI::size_point = new ValueInt("size_point", 2);
    GUI::current_panel = new ValueInt{ "current_panel", 0 };

    _DEBUG::period_send = new ValueUInt("period_send", 100);
}


template<typename T>
void SET::AppendValue(T *value)
{
    LOG_WRITE("append value");

    if constexpr (std::is_same_v<T, ValueBool>)
    {
        vec_bool.Push(value);
    }
    else if constexpr (std::is_same_v<T, ValuePoint>)
    {
        vec_point.Push(value);
    }
    else if constexpr (std::is_same_v<T, ValueInt>)
    {
        vec_int.Push(value);
    }
    else if constexpr (std::is_same_v<T, ValueUInt>)
    {
        vec_uint.Push(value);
    }
    else
    {
        LOG_ERROR("Can not find type");
    }
}


void SET::Load()
{
    LOG_WRITE("SET::Load()");

    for (auto *vec : all_vectors)
    {
        vec->Load();
    }
}


void SET::Save()
{
    for (auto *vec : all_vectors)
    {
        vec->Save();
    }
}
