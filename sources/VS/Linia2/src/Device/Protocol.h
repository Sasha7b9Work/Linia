// 2025/08/11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "defines.h"
#include <vector>
#include <cstdint>


// Протокол связи с STM32 через RS-232
namespace Protocol
{
    // Команды для STM32 (совпадают с Device/src/Messages_.h)
    enum Command : uint32_t
    {
        START_1_12V = 0,
        START_1_24V,
        START_2A,
        START_3A,
        START_3B,
        STOP,
        PAUSE,
        RESUME,
        Count
    };

    // Статусы ответов от STM32
    enum Status : uint32_t
    {
        OK = 0,
        ERROR_INVALID_COMMAND,
        ERROR_INVALID_PARAMS,
        ERROR_HARDWARE_FAULT,
        ERROR_BUSY,
        ERROR_TIMEOUT
    };

    // Структура Value (совпадает с common/Connector/Device/Value_.h)
    struct Value
    {
        uint32_t raw;

        Value() : raw(0) {}
        Value(uint32_t r) : raw(r) {}
        Value(float voltage) { SetVoltage(voltage); }
        Value(int time_ms) { SetTime(time_ms); }

        void SetVoltage(float volts);
        void SetTime(int milliseconds);
        float ToVoltage() const;
        int ToTime() const;
        bool IsVoltage() const;
        bool IsTime() const;
    };

    // Базовый класс для сообщений
    class Message
    {
    public:
        Message(Command cmd) : command(cmd) {}
        virtual ~Message() = default;

        virtual std::vector<uint8_t> Serialize() const;
        virtual bool IsValid() const { return true; }

        Command GetCommand() const { return command; }

    protected:
        Command command;
        std::vector<uint32_t> parameters;

        void AddParameter(uint32_t param) { parameters.push_back(param); }
        void AddValue(const Value& value) { parameters.push_back(value.raw); }
    };

    // Специализированные сообщения
    class StartMessage1_12V : public Message
    {
    public:
        StartMessage1_12V(const Value& voltage, const Value& time);
        bool IsValid() const override;
    private:
        Value voltage, time;
    };

    class StartMessage1_24V : public Message
    {
    public:
        StartMessage1_24V(const Value& voltage, const Value& time);
        bool IsValid() const override;
    private:
        Value voltage, time;
    };

    class StartMessage2A : public Message
    {
    public:
        StartMessage2A(const Value& voltage, const Value& time);
        bool IsValid() const override;
    private:
        Value voltage, time;
    };

    class StartMessage3A : public Message
    {
    public:
        StartMessage3A(const Value& voltage);
        bool IsValid() const override;
    private:
        Value voltage;
    };

    class StartMessage3B : public Message
    {
    public:
        StartMessage3B(const Value& voltage);
        bool IsValid() const override;
    private:
        Value voltage;
    };

    class StopMessage : public Message
    {
    public:
        StopMessage() : Message(Command::STOP) {}
    };

    class PauseMessage : public Message
    {
    public:
        PauseMessage() : Message(Command::PAUSE) {}
    };

    class ResumeMessage : public Message
    {
    public:
        ResumeMessage() : Message(Command::RESUME) {}
    };

    // Ответное сообщение от STM32
    struct Response
    {
        uint32_t signature;
        Status status;
        uint32_t data_size;
        std::vector<uint8_t> data;
        uint32_t crc;

        bool IsValid() const;
        static Response Parse(const std::vector<uint8_t>& raw_data);
    };

    // Утилиты для CRC и сериализации
    namespace Utils
    {
        uint32_t CalculateCRC(const std::vector<uint8_t>& data);
        std::vector<uint8_t> SerializeUint32(uint32_t value);
        uint32_t DeserializeUint32(const uint8_t* data);
        const uint32_t MESSAGE_SIGNATURE = 0x12345678;
    }
}
