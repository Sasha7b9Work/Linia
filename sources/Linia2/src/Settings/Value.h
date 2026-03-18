// 2024/02/03 12:50:14 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Utils/Configurator.h"
#include "Utils/GlobalFunctions.h"
#include "Controls/Buttons.h"


template<class T>
class Value
{
public:
    Value(const wxString &_key, const T &_default_value, wxWindow *_window = nullptr, int _id = 0, bool _serialiazed = true) : //-V730
        window(_window),
        id(_id),
        key(_key),
        default_value(_default_value),
        stored_value(_default_value),
        serialiazed(_serialiazed)
    {}

    T Get() const
    {
        return value;
    }

    T GetValueAndReset()
    {
        T result = value;

        Reset();

        return result;
    }

    virtual void Set(const T &_value)
    {
        value = _value;

        if (serialiazed)
        {
            Config::Write(key, value);
        }
    }

    virtual void Load()
    {
        value = Config::Read(key, default_value);
    }

    virtual void Save()
    {
        Set(value);
    }

    // Установить состояние контрола в соотвествии с состоянием переменной
    void LoadToControl(wxWindow *_window = nullptr, int _id = 0)
    {
        if (_window)
        {
            window = _window;
            id = _id;
        }

        Load();

        LoadToWindow();
    }

    virtual void ResetToFactory(bool factory)
    {
        if (factory)
        {
            stored_value = GetValueAndReset();
        }
        else
        {
            Set(stored_value);
        }

        LoadToControl();
    }

    // Установить значение переменной из контрола
    virtual void SetFromControl() { };

    bool ValueIsDefault() const
    {
        return Get() == GetDefault();
    }

    void Reset()
    {
        Set(default_value);
    }

    operator T() const
    {
        return Get();
    }

private:

    T GetDefault() const
    {
        return default_value;
    }

    wxString GetKey() const
    {
        return key;
    }

protected:
    wxWindow *window;           // Какому окну принадлежит элемент управления
    int      id;                // id элемента управления
    wxString key;
    T        default_value;
    T        stored_value;      // Сюда записываем значение перед тем как загрузить заводскую настроку,
                                // чтобы потом восстановить

    void Init(const wxString &_key, const T &_default_value, wxWindow *_window, int _id)
    {
        key = _key;
        default_value = _default_value;
        window = _window;
        id = _id;

        Load();
    }

    // Загрузить значение непосредственно в элемент управления
    virtual void LoadToWindow() { };

private:
    T value;
    const bool serialiazed;
};


struct ValueBool : public Value<bool>
{
    ValueBool(const wxString &_key, const bool &_def);
};

struct ValueCheckBox : public wxCheckBox
{
    ValueCheckBox(wxWindow *parent, const wxString &title, const wxPoint &position, const wxString &_key, const bool &_def);

    void SetNewValue(bool);

private:

    ValueBool *value = nullptr;

    void OnEventCheckBox(wxCommandEvent &);

    // Закрыли, чтобы случайно не вызвать
    virtual void SetValue(bool value) override;
};


struct ValueInt : public Value<int>
{
    ValueInt(const wxString &_key, const int &_def);
};


struct ValueUInt : public Value<uint>
{
    ValueUInt(const wxString &_key, const uint &_def);
};


struct ValuePoint : public Value<wxPoint>
{
    ValuePoint(const wxString &_key, const wxPoint &_def);
};
