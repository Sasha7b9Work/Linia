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
    {
    }

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

    void Load()
    {
        value = Config::Read(key, default_value);
    }

    void Save()
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

private:

    T GetDefault() const
    {
        return default_value;
    }

    void Reset()
    {
        Set(default_value);
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



// Значение для выбора из ComboBox
class ValueComboBox : public Value<int>
{
public:
    ValueComboBox(pchar _key, int index) :
        Value<int>(_key, index, nullptr, 0)
    {
    }
    int GetIndex() const
    {
        return Value<int>::Get();
    }
    void SetIndex(int index)
    {
        Value<int>::Set(index);
    }
    // Установить значение из элемента управления
    virtual void SetFromControl() override
    {
        Value<int>::Set(GF::FindComboBox(window, id)->GetSelection());
    }
    int GetDefaultIndex() const
    {
        return default_value;
    }

protected:

    virtual void LoadToWindow() override
    {
        if (window)
        {
            GF::FindComboBox(window, id)->SetSelection(GetIndex());
        }
    }

private:
    int Get() const
    {
        return 0;
    }
};


class ValueCheckBox : public Value<bool>
{
public:
    ValueCheckBox(pchar _key, bool def) :
        Value<bool>(_key, def, nullptr, 0)
    {
    }
    void Load(wxWindow *parent, int _id = -1)
    {
        Value<bool>::Load();

        if (parent)
        {
            GF::FindCheckBox(parent, _id)->SetValue(Get());
        }
    }
    virtual void SetFromControl() override
    {
        Value<bool>::Set(GF::FindCheckBox(window, id)->GetValue());
    }
    void ApplyToGUI(wxWindow *parent, int _id)
    {
        GF::SendCommandEvent(parent, wxEVT_CHECKBOX, _id, Get());
    }
protected:
    virtual void LoadToWindow() override
    {
        if (window)
        {
            GF::FindCheckBox(window, id)->SetValue(Get());
        }
    }
private:
};


class ValueCheckButton : public Value<bool>
{
public:
    ValueCheckButton(pchar _key, bool value) : Value<bool>(_key, value, nullptr, 0)
    {
    }
    virtual void SetFromControl() override
    {
        Value<bool>::Set(GF::FindCheckButton(window, id)->GetValue());
    }
    // Посылает событие данного контрола на всё ГУИ
    void SendEventToGUI(wxWindow *parent = nullptr, int _id = 0)
    {
        if (parent == nullptr)
        {
            parent = window;
            _id = id;
        }

        GF::SendCommandEvent(parent, wxEVT_TOGGLEBUTTON, _id, Get());
    }
    void Set(const bool &val)
    {
        Value<bool>::Set(val);
    }
    virtual void ResetToFactory(bool factory) override
    {
        Value<bool>::ResetToFactory(factory);

        SendEventToGUI();
    }
protected:
    virtual void LoadToWindow() override
    {
        if (window)
        {
            GF::FindCheckButton(window, id)->SetValue(Get());
        }
    }
};


// Значение из TextCtrl
class ValueTextCtrl : public Value<int>
{
public:
    ValueTextCtrl(pchar _key, int def) : Value<int>(_key, def, nullptr, 0)
    {
    }
    wxString ToString() const
    {
        return wxString::Format("%d", Get());
    }
    virtual void SetFromControl() override
    {
        Set(wxAtoi(GF::FindTextCtrl(window, id)->GetValue()));
    }
protected:
    virtual void LoadToWindow() override
    {
        if (window)
        {
            GF::FindTextCtrl(window, id)->SetValue(ToString());
        }
    }
};


class ValueSpinCtrl : public Value<int>
{
public:
    ValueSpinCtrl(pchar _key, int value) : Value<int>(_key, value, nullptr, 0)
    {
    }
    wxString ToString() const
    {
        return wxString::Format("%d", Get());
    }
    int GetIntValue() const
    {
        return Value<int>::Get();
    }
    void SetIntValue(int val)
    {
        Value<int>::Set(val);
    }
    virtual void SetFromControl() override
    {
        Set(GF::FindSpinCtrl(window, id)->GetValue());
    }
    int GetDefaultValue() const
    {
        return default_value;
    }
protected:
    virtual void LoadToWindow() override
    {
        if (window)
        {
            GF::FindSpinCtrl(window, id)->SetValue(Get());
        }
    }
};


class ValueColor : public Value<Color>
{
public:
    ValueColor(const wxString &key, const Color &default_value) : Value<Color>(key, default_value) { }
    void Load(ButtonColor *button)
    {
        Value<Color>::Load();
        button->SetColor(Get());
    }
    virtual void SetFromControl() override { }
    void ResetToFactoryColor(bool factory, ButtonColor *button)
    {
        if (factory)
        {
            stored_value = GetValueAndReset();
            button->SetColor(Get());
        }
        else
        {
            Set(stored_value);
            button->SetColor(stored_value);
        }
    }
protected:
    virtual void LoadToWindow() override { }
};

