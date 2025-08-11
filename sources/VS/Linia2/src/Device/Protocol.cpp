// 2025/08/11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Device/Protocol.h"
#include <cmath>
#include <algorithm>


namespace Protocol
{
    // Константы для валидации параметров (из MCP4811.cpp)
    static const float MAX_VOLTAGE_1_12V = 180.0f;  // 150.0f * 1.2f
    static const float MAX_VOLTAGE_1_24V = 720.0f;  // 600.0f * 1.2f  
    static const float MAX_VOLTAGE_2A = 134.4f;     // 112.0f * 1.2f
    static const float MAX_VOLTAGE_3A = 360.0f;     // 300.0f * 1.2f
    static const float MAX_VOLTAGE_3B = 360.0f;     // 300.0f * 1.2f
    
    static const int MAX_TIME_MS = 60000;           // Максимально 60 секунд
    static const int MIN_TIME_MS = 1;               // Минимально 1 мс
}

// Value implementation
void Protocol::Value::SetVoltage(float volts)
{
    // Преобразуем в милливольты и устанавливаем тип Voltage
    int millivolts = static_cast<int>(volts * 1000.0f);
    raw = (millivolts & 0x1FFFFFFF) | (1 << 29); // Бит 29 = тип Voltage
    if (volts < 0) raw |= (1 << 31); // Бит 31 = знак
}

void Protocol::Value::SetTime(int milliseconds)
{
    // Устанавливаем тип Time
    raw = (milliseconds & 0x1FFFFFFF) | (1 << 30); // Бит 30 = тип Time
    if (milliseconds < 0) raw |= (1 << 31); // Бит 31 = знак
}

float Protocol::Value::ToVoltage() const
{
    if (!IsVoltage()) return 0.0f;
    
    int value = static_cast<int>(raw & 0x1FFFFFFF);
    if (raw & (1 << 31)) value = -value; // Учитываем знак
    
    return static_cast<float>(value) * 1e-3f; // Из милливольт в вольты
}

int Protocol::Value::ToTime() const
{
    if (!IsTime()) return 0;
    
    int value = static_cast<int>(raw & 0x1FFFFFFF);
    if (raw & (1 << 31)) value = -value; // Учитываем знак
    
    return value;
}

bool Protocol::Value::IsVoltage() const
{
    return (raw & (1 << 29)) != 0;
}

bool Protocol::Value::IsTime() const
{
    return (raw & (1 << 30)) != 0;
}

// Message base implementation
std::vector<uint8_t> Protocol::Message::Serialize() const
{
    std::vector<uint8_t> result;
    
    // Signature
    auto sig_bytes = Utils::SerializeUint32(Utils::MESSAGE_SIGNATURE);
    result.insert(result.end(), sig_bytes.begin(), sig_bytes.end());
    
    // Command
    auto cmd_bytes = Utils::SerializeUint32(static_cast<uint32_t>(command));
    result.insert(result.end(), cmd_bytes.begin(), cmd_bytes.end());
    
    // Parameters
    for (uint32_t param : parameters)
    {
        auto param_bytes = Utils::SerializeUint32(param);
        result.insert(result.end(), param_bytes.begin(), param_bytes.end());
    }
    
    // CRC
    uint32_t crc = Utils::CalculateCRC(result);
    auto crc_bytes = Utils::SerializeUint32(crc);
    result.insert(result.end(), crc_bytes.begin(), crc_bytes.end());
    
    return result;
}

// Specialized messages implementation
Protocol::StartMessage1_12V::StartMessage1_12V(const Value& voltage, const Value& time)
    : Message(Command::START_1_12V), voltage(voltage), time(time)
{
    AddValue(voltage);
    AddValue(time);
}

bool Protocol::StartMessage1_12V::IsValid() const
{
    if (!voltage.IsVoltage() || !time.IsTime()) return false;
    
    float v = voltage.ToVoltage();
    int t = time.ToTime();
    
    return (v >= 0 && v <= MAX_VOLTAGE_1_12V && 
            t >= MIN_TIME_MS && t <= MAX_TIME_MS);
}

Protocol::StartMessage1_24V::StartMessage1_24V(const Value& voltage, const Value& time)
    : Message(Command::START_1_24V), voltage(voltage), time(time)
{
    AddValue(voltage);
    AddValue(time);
}

bool Protocol::StartMessage1_24V::IsValid() const
{
    if (!voltage.IsVoltage() || !time.IsTime()) return false;
    
    float v = voltage.ToVoltage();
    int t = time.ToTime();
    
    return (v >= 0 && v <= MAX_VOLTAGE_1_24V && 
            t >= MIN_TIME_MS && t <= MAX_TIME_MS);
}

Protocol::StartMessage2A::StartMessage2A(const Value& voltage, const Value& time)
    : Message(Command::START_2A), voltage(voltage), time(time)
{
    AddValue(voltage);
    AddValue(time);
}

bool Protocol::StartMessage2A::IsValid() const
{
    if (!voltage.IsVoltage() || !time.IsTime()) return false;
    
    float v = voltage.ToVoltage();
    int t = time.ToTime();
    
    return (v >= 0 && v <= MAX_VOLTAGE_2A && 
            t >= MIN_TIME_MS && t <= MAX_TIME_MS);
}

Protocol::StartMessage3A::StartMessage3A(const Value& voltage)
    : Message(Command::START_3A), voltage(voltage)
{
    AddValue(voltage);
}

bool Protocol::StartMessage3A::IsValid() const
{
    if (!voltage.IsVoltage()) return false;
    
    float v = voltage.ToVoltage();
    return (v >= 0 && v <= MAX_VOLTAGE_3A);
}

Protocol::StartMessage3B::StartMessage3B(const Value& voltage)
    : Message(Command::START_3B), voltage(voltage)
{
    AddValue(voltage);
}

bool Protocol::StartMessage3B::IsValid() const
{
    if (!voltage.IsVoltage()) return false;
    
    float v = voltage.ToVoltage();
    return (v >= 0 && v <= MAX_VOLTAGE_3B);
}

// Response implementation
bool Protocol::Response::IsValid() const
{
    if (signature != Utils::MESSAGE_SIGNATURE) return false;
    
    // Проверяем CRC
    std::vector<uint8_t> data_for_crc;
    
    auto sig_bytes = Utils::SerializeUint32(signature);
    data_for_crc.insert(data_for_crc.end(), sig_bytes.begin(), sig_bytes.end());
    
    auto status_bytes = Utils::SerializeUint32(static_cast<uint32_t>(status));
    data_for_crc.insert(data_for_crc.end(), status_bytes.begin(), status_bytes.end());
    
    auto size_bytes = Utils::SerializeUint32(data_size);
    data_for_crc.insert(data_for_crc.end(), size_bytes.begin(), size_bytes.end());
    
    data_for_crc.insert(data_for_crc.end(), data.begin(), data.end());
    
    uint32_t calculated_crc = Utils::CalculateCRC(data_for_crc);
    
    return (calculated_crc == crc);
}

Protocol::Response Protocol::Response::Parse(const std::vector<uint8_t>& raw_data)
{
    Response response;
    
    if (raw_data.size() < 16) // Минимум: signature + status + size + crc
    {
        response.status = Status::ERROR_INVALID_PARAMS;
        return response;
    }
    
    const uint8_t* data_ptr = raw_data.data();
    
    response.signature = Utils::DeserializeUint32(data_ptr);
    data_ptr += 4;
    
    response.status = static_cast<Status>(Utils::DeserializeUint32(data_ptr));
    data_ptr += 4;
    
    response.data_size = Utils::DeserializeUint32(data_ptr);
    data_ptr += 4;
    
    if (response.data_size > 0 && raw_data.size() >= 16 + response.data_size)
    {
        response.data.assign(data_ptr, data_ptr + response.data_size);
        data_ptr += response.data_size;
    }
    
    if (raw_data.size() >= 16 + response.data_size + 4)
    {
        response.crc = Utils::DeserializeUint32(data_ptr);
    }
    
    return response;
}

// Utils implementation
uint32_t Protocol::Utils::CalculateCRC(const std::vector<uint8_t>& data)
{
    // Простой CRC32 алгоритм
    uint32_t crc = 0xFFFFFFFF;
    
    for (uint8_t byte : data)
    {
        crc ^= byte;
        for (int i = 0; i < 8; i++)
        {
            if (crc & 1)
                crc = (crc >> 1) ^ 0xEDB88320;
            else
                crc >>= 1;
        }
    }
    
    return ~crc;
}

std::vector<uint8_t> Protocol::Utils::SerializeUint32(uint32_t value)
{
    std::vector<uint8_t> result(4);
    result[0] = static_cast<uint8_t>(value & 0xFF);
    result[1] = static_cast<uint8_t>((value >> 8) & 0xFF);
    result[2] = static_cast<uint8_t>((value >> 16) & 0xFF);
    result[3] = static_cast<uint8_t>((value >> 24) & 0xFF);
    return result;
}

uint32_t Protocol::Utils::DeserializeUint32(const uint8_t* data)
{
    return static_cast<uint32_t>(data[0]) |
           (static_cast<uint32_t>(data[1]) << 8) |
           (static_cast<uint32_t>(data[2]) << 16) |
           (static_cast<uint32_t>(data[3]) << 24);
}
