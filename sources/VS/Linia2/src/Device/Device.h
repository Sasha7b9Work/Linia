#pragma once

namespace Device {
    class IDevice;
}

extern Device::IDevice* g_device;

namespace Device {

enum class ChannelMode : uint8_t {
    OPEN         = 0,  // обрыв
    CHANNEL      = 1,  // канал
    COMMON       = 2,  // общий  
    NEUTRAL      = 3,  // нейтральное положение
    COMMON_1KOHM = 4   // общий, 1 kOm
};

enum class SweepType : uint8_t {
    PLUS_IMP     = 0,  // + IMP
    PLUS_DC      = 1,  // + DC
    PLUS_SYN     = 2,  // + SYN
    PLUS_MINUS_AC = 3, // ± AC
    MINUS_SYN    = 4,  // - SYN
    MINUS_DC     = 5,  // - DC
    MINUS_IMP    = 6   // - IMP
};

enum class SourceType : uint8_t {
    VOLTAGE = 0,  // источник напряжения
    CURRENT = 1   // источник тока
};

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
    
    virtual void SetButtonCallback(ButtonCallback callback) = 0;
    virtual void SetStatusCallback(StatusCallback callback) = 0;
    virtual void SetDataCallback(DataCallback callback) = 0;

    virtual void SetCircuitConnection(ChannelMode channelC, ChannelMode channelB, ChannelMode channelS) = 0;
    
    virtual void SetSweepType(SweepType sweep, uint8_t queue) = 0;
    
    virtual void SetPulseDuration(uint8_t duration, uint8_t mode) = 0;

    virtual void SetChannelC_SourceRange(uint8_t range) = 0;              
    virtual void SetChannelC_MeasU_Range(uint8_t range) = 0;              
    virtual void SetChannelC_MeasI_Range(uint8_t range) = 0;              
    virtual void SetChannelC_LimitMax(uint8_t percent) = 0;               
    virtual void SetChannelC_LimitMin(uint8_t percent) = 0;               
    
    virtual void SetChannelB_SourceMode(SourceType type) = 0;
    virtual void SetChannelB_AmplitudeRange(uint8_t range) = 0;
    virtual void SetChannelB_StepCount(uint8_t steps) = 0;     
    virtual void SetChannelB_AmplitudeValue(uint16_t value) = 0;
    virtual void SetChannelB_Bias(uint16_t value) = 0;          
    virtual void SetChannelB_MeasMode(SourceType type) = 0;     
    virtual void SetChannelB_MeasRange(uint8_t range) = 0;      
    virtual void SetChannelB_LimitRange(uint8_t range) = 0;     
    virtual void SetChannelB_LimitThreshold(uint16_t value) = 0;
    virtual void SetChannelB_HighResolution(bool enable) = 0;   
    virtual void SetChannelB_SourceType(SourceType type) = 0;   
    
    virtual void SetChannelS_SourceMode(SourceType type) = 0;
    virtual void SetChannelS_AmplitudeRange(uint8_t range) = 0;
    virtual void SetChannelS_StepCount(uint8_t steps) = 0;     
    virtual void SetChannelS_AmplitudeValue(uint16_t value) = 0;
    virtual void SetChannelS_Bias(uint16_t value) = 0;          
    virtual void SetChannelS_MeasMode(SourceType type) = 0;     
    virtual void SetChannelS_MeasRange(uint8_t range) = 0;      
    virtual void SetChannelS_LimitRange(uint8_t range) = 0;     
    virtual void SetChannelS_LimitThreshold(uint16_t value) = 0;
    virtual void SetChannelS_SourceType(SourceType type) = 0;   
    
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
