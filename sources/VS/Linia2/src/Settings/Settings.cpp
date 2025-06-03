// 2023/08/09 19:11:58 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Settings/Settings.h"
#include "Utils/Configurator.h"


namespace SET
{
    namespace GUI
    {
        Value<wxPoint>   position("gui_position", { 100, 100 });
        Value<wxPoint>   pos_console("gui_pos_console", { 10, 10 });
        Value<wxPoint>   size_console("gui_size_console", { 400, 400 });
        Value<wxPoint>   pos_list("gui_pos_list", { 10, 10 });
        Value<wxPoint>   size_list("gui_size_list", { 400, 400 });
        Value<bool>      maximized_list("gui_maximized_list", false);
        Value<bool>      maximized_console("gui_maximized_console", false);
        Value<int>       current_page_notebook("gui_current_page_notebook", 0);
        ValueCheckButton serial_port_auto_find("serial_port_auto_find", true);
        Value<int>       serial_port_num("serial_port_num", 0);
        Value<bool>      debug_mode("debug_mode", false);
    }

    void Init()
    {
        Load("");
    }

    void Load(const wxString &file_path)
    {
        Config::SetFile(file_path);

        Config::SetFile("");
    }


    void Save(const wxString &file_path)
    {
        Config::SetFile(file_path);

        Config::SetFile("");
    }
}


void SET::GUI::Load()
{
    position.Load();
    pos_console.Load();
    size_console.Load();
    pos_list.Load();
    size_list.Load();
    maximized_list.Load();
    debug_mode.Load();
    maximized_console.Load();
    current_page_notebook.Load();
    serial_port_num.Load();
}


void SET::GUI::Save()
{
    debug_mode.Save();
}
