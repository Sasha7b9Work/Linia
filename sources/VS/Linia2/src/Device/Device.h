#pragma once
#include "Tests/SettingsTests.h"
#include "Tests/Ranges.h"


namespace Device {
    class IDevice;
}

extern Device::IDevice* g_device;

namespace Device {

struct ButtonEvent {
    enum Type : uint8_t {
        START_PRESSED   = 1,  // K1 - нажата кнопка "Пуск"
        ENCODER_FIRST   = 2,  // K2 - первое вращение рукоятки  
        ENCODER_CHANGED = 3,  // K3 - новое состояние рукоятки
        STOP_PRESSED    = 4,  // K4 - нажата кнопка "Стоп"
        ENCODER_STEP    = 5   // K5 - изменение на 1 шаг в DC режиме
    };
    
    uint8_t type;
    uint8_t value;
};

struct StatusEvent {
    enum Type : uint8_t {
        READY    = 'G',  // готовность контроллера
        POSITION = 'P',  // положение переключателя
        FLAG     = 'F'   // сигнал о событии/ошибке
    };
    
    uint8_t type;
    uint8_t code;
};

struct ErrorFlags {
    static constexpr uint8_t POWER_OVERLOAD    = 0x01;  // D0 - превышен порог по мощности
    static constexpr uint8_t COVER_VIOLATION   = 0x04;  // D2 - блокировка крышки нарушена  
    static constexpr uint8_t CHANNEL_C_OVERLOAD = 0x08; // D3 - перегрузка по каналу С
    static constexpr uint8_t CHANNEL_B_OVERLOAD = 0x10; // D4 - перегрузка по каналу B
    static constexpr uint8_t CHANNEL_S_OVERLOAD = 0x20; // D5 - перегрузка по каналу S
    
    static constexpr uint8_t COVER_CLOSED = 0x3D;       // крышка закрыта
    static constexpr uint8_t COVER_OPEN   = 0x39;       // крышка открыта
};

struct SwitchPosition {
    static constexpr uint8_t PLUS_MINUS_2000V = 0;  // ±2000V
    static constexpr uint8_t PLUS_5V          = 1;  // +5V
    static constexpr uint8_t MINUS_5V         = 2;  // -5V
    static constexpr uint8_t NEUTRAL          = 3;  // нейтральное положение
    static constexpr uint8_t FAULT            = 4;  // переключатель неисправен
};

class IDevice {
public:
    virtual ~IDevice() = default;
    
    virtual bool Init() = 0;
    virtual void Shutdown() = 0;
    virtual bool IsConnected() const = 0;
    
//    virtual void SetDataCallback(DataCallback callback) = 0;

    virtual void SetCircuitConnection(StateJack::E channelC, StateJack::E channelB, StateJack::E channelS) = 0;
    
    virtual void SetSweepType(TypeScan::E, FirstQueue::E) = 0;
    
    virtual void SetPulseDuration(uint durationUS, GenerationStup::E) = 0;

    virtual void SetChannelC_SourceRange(RangeU::E) = 0;              
    virtual void SetChannelC_Meas_Range(RangeU::E) = 0;              
    virtual void SetChannelC_Meas_Range(RangeI::E) = 0;              
    virtual void SetChannelC_LimitMax(int percent) = 0;               
    virtual void SetChannelC_LimitMin(int percent) = 0;               
    
    virtual void SetChannelB_SourceMode(ModeSource::E) = 0;
    virtual void SetChannelB_AmplitudeRange(uint8_t range) = 0;
    virtual void SetChannelB_StepCount(uint8_t steps) = 0;     
    virtual void SetChannelB_AmplitudeValue(uint16_t value) = 0;
    virtual void SetChannelB_Bias(uint16_t value) = 0;          
    virtual void SetChannelB_MeasMode(ModeMeas::E) = 0;     
    virtual void SetChannelB_MeasRange(uint8_t range) = 0;      
    virtual void SetChannelB_LimitRange(uint8_t range) = 0;     
    virtual void SetChannelB_LimitThreshold(uint16_t value) = 0;
    virtual void SetChannelB_HighResolution(bool enable) = 0;   
    virtual void SetChannelB_SourceType(ModeSource::E) = 0;   
    
    virtual void SetChannelS_SourceMode(ModeSource::E) = 0;
    virtual void SetChannelS_AmplitudeRange(uint8_t range) = 0;
    virtual void SetChannelS_StepCount(uint8_t steps) = 0;     
    virtual void SetChannelS_AmplitudeValue(uint16_t value) = 0;
    virtual void SetChannelS_Bias(uint16_t value) = 0;          
    virtual void SetChannelS_MeasMode(ModeSource::E) = 0;     
    virtual void SetChannelS_MeasRange(uint8_t range) = 0;      
    virtual void SetChannelS_LimitRange(uint8_t range) = 0;     
    virtual void SetChannelS_LimitThreshold(uint16_t value) = 0;
    virtual void SetChannelS_SourceType(ModeSource::E) = 0;   
    
    virtual void SetAutoZeroOff() = 0;                                    
    virtual void SetAutoZeroOn() = 0;                                     
    virtual void SetPointCount(uint8_t points) = 0;                       
    virtual void SetStartType(bool external, bool multiple) = 0;          
    virtual void SetMeasurementMode(uint8_t mode) = 0;                    
    virtual void SetMaxDacCode(uint8_t code) = 0;                         
    virtual void SetBitQ18(uint8_t state) = 0;                            
    virtual void SetElementType(bool isMultipole) = 0;                    
    virtual void SendLimitSignal(uint8_t limitCode) = 0;                  
    virtual void SetConfigData(uint8_t modification, uint8_t nominal) = 0;
    virtual void EmergencyStop() = 0;                                     
    virtual void StartMeasurement() = 0;                                  
    
    virtual void StopMeasurement() = 0;
    virtual void ResetToDefaults() = 0;
};
}
