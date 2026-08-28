// 2026/03/23 11:42:54 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "IPPP/DeviceFactory.h"
#include "Settings/Settings.h"
#include "IPPP/I_IPPP.h"
#include "IPPP/Emulator/EmulatorIPPP.h"
#include "IPPP/Real/RealIPPP.h"


std::map<DeviceFactory::Type, DeviceFactory::Creator> DeviceFactory::creators;


struct DeviceRegistrator
{
    DeviceRegistrator()
    {
        DeviceFactory::RegisterType<RealIPPP>(DeviceFactory::Type::Real);
        DeviceFactory::RegisterType<EmulatorIPPP>(DeviceFactory::Type::Emulator);
    }
};


static DeviceRegistrator registrator;


std::unique_ptr<I_IPPP> DeviceFactory::Create(Type type)
{
    auto it = creators.find(type);

    if (it != creators.end())
    {
        return it->second();
    }

    return nullptr;
}


std::unique_ptr<I_IPPP> DeviceFactory::CreateFromConfig()
{
    if (SET_DEBUG_EMULATE_MODE->GetBool())
    {
        return std::make_unique<EmulatorIPPP>();
    }

    return std::make_unique<RealIPPP>();
}
