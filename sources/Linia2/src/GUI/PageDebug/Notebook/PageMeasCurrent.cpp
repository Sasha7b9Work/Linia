// 2025/10/07 12:25:36 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "GUI/PageDebug/Notebook/PageMeasCurrent.h"
#include "GUI/PageDebug/Notebook/Register.h"

/*
    ADC     AD7691      16 разрядов
*/

PageMeasCurrent *PageMeasCurrent::self = nullptr;


PageMeasCurrent::PageMeasCurrent(wxNotebook *notebook) :
    PageChip(notebook, "Измеритель тока")
{
    self = this;

    RegFPGA *reg5 = new RegFPGA(this, regs[REG::_5_MeasCurrent]);

    std::vector<StructDescription> desc5;

    std::vector<StructDescription::CommandStruct> commandKoff;
    commandKoff.emplace_back(StructDescription::CommandStruct{ 0b00, "x1" });
    commandKoff.emplace_back(StructDescription::CommandStruct{ 0b01, "x2" });
    commandKoff.emplace_back(StructDescription::CommandStruct{ 0b10, "x4" });
    commandKoff.emplace_back(StructDescription::CommandStruct{ 0b11, "x8" });
    desc5.emplace_back(StructDescription{ 0, 2 , "k", "коэфффициент", { true, commandKoff } });

    std::vector<StructDescription::CommandStruct> commandRange;
    commandRange.emplace_back(StructDescription::CommandStruct{ 0b000, "200 нА" });
    commandRange.emplace_back(StructDescription::CommandStruct{ 0b001, "20 мкА" });
    commandRange.emplace_back(StructDescription::CommandStruct{ 0b010, "2 мА" });
    commandRange.emplace_back(StructDescription::CommandStruct{ 0b011, "200 мА" });
    commandRange.emplace_back(StructDescription::CommandStruct{ 0b100, "20 А" });
    commandRange.emplace_back(StructDescription::CommandStruct{ 0b101, "200 А" });
    desc5.emplace_back(StructDescription{ 2, 3, "п", "пределы", { true, commandRange} });

    std::vector<StructDescription::CommandStruct> commandBlock;
    commandBlock.emplace_back(StructDescription::CommandStruct{ 0b0, "блокировка сигнала OLVI,1 "
        "о превышении порогового уровня тока в "
        "режимах \"Коллектор в обрыве\" и \"Стоп\"."
        "(В шине - сигнал \"unOLVI,1\")" });
    commandBlock.emplace_back(StructDescription::CommandStruct{ 0b1, "для всех остальных режимов" });
    desc5.emplace_back(StructDescription{ 5, 1 , "б", "блокировка", { true, commandBlock } });

    std::vector<StructDescription::CommandStruct> commandCapacity;
    commandCapacity.emplace_back(StructDescription::CommandStruct{ 0b0, "для всех режимов кроме DC" });
    commandCapacity.emplace_back(StructDescription::CommandStruct{ 0b1, "дополнительная ёмкость в режиме DC" });
    desc5.emplace_back(StructDescription{ 6, 1, "ё", "дополнительная ёмкость", {true, commandCapacity } });

    std::vector<StructDescription::CommandStruct> commandK;
    commandK.emplace_back(StructDescription::CommandStruct{ 0b0, "x1" });
    commandK.emplace_back(StructDescription::CommandStruct{ 0b1, "x10" });
    desc5.emplace_back(StructDescription{ 7, 1, "к", "коэффициент", { true, commandK } });

    reg5->SetDescriptionBits(0, desc5);

    AppendRegister(reg5);
}

