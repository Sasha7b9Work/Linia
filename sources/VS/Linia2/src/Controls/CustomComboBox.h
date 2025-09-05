// 2025/8/9 10:57:48 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class CustomComboPopup : public wxComboPopup
{
public:
    CustomComboPopup() : m_panel(nullptr) {}

    // Обязательные методы wxComboPopup
    virtual bool Create(wxWindow *parent) override
    {
        // Создаем панель как дочернее окно
        m_panel = new wxScrolledWindow(parent, wxID_ANY);
        m_panel->SetScrollRate(0, 10); // Включаем вертикальную прокрутку

        // Инициализируем содержимое
        InitContent();
        return true;
    }

    virtual wxWindow *GetControl() override
    {
        return m_panel;
    }

    virtual void OnPopup() override
    {
        // Устанавливаем размер при открытии
        m_panel->SetSize(GetComboCtrl()->GetSize().x, 120);
        m_panel->Layout();
    }

    // Наша инициализация содержимого
    void InitContent()
    {
        wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);

        for (int i = 0; i < 20; ++i)
        {
            wxButton *btn = new wxButton(m_panel, wxID_ANY,
                wxString::Format("Item %d", i), wxDefaultPosition );
            btn->Bind(wxEVT_BUTTON, &CustomComboPopup::OnItemSelected, this);
            sizer->Add(btn, 0, wxEXPAND | wxALL, 2);
        }

        m_panel->SetSizer(sizer);
        m_panel->FitInside();
    }

    void OnItemSelected(wxCommandEvent &event)
    {
        wxButton *btn = dynamic_cast<wxButton *>(event.GetEventObject());
        if (btn)
        {
            wxComboCtrl *combo = static_cast<wxComboCtrl *>(GetComboCtrl());
            combo->SetText(btn->GetLabel());
            combo->Dismiss();
        }
    }

    virtual wxString GetStringValue() const override
    {
        return "1";
    }

private:
    wxScrolledWindow *m_panel;
};


class CustomComboBox : public wxComboCtrl
{
public:
    CustomComboBox(wxWindow *parent, wxWindowID id)
        : wxComboCtrl(parent, id, wxEmptyString, wxDefaultPosition, wxSize(150, -1), wxCB_READONLY)
    {
        wxComboCtrl::SetPopupControl(new CustomComboPopup());
    }
};
