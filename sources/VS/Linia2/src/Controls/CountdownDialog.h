// 2025/10/17 08:25:48 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class CountdownDialog : public wxDialog
{
public:
    CountdownDialog(wxWindow *parent, const wxString &message, int countdownSeconds)
        : wxDialog(parent, wxID_ANY, "Сообщение", wxDefaultPosition, wxSize(300, 150)),
        m_countdown(countdownSeconds), m_initialCountdown(countdownSeconds)
    {
        wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

        // Текст сообщения
        m_messageText = new wxStaticText(this, wxID_ANY, message);
        mainSizer->Add(m_messageText, 0, wxALL | wxALIGN_CENTER, 10);

        // Текст отсчета
        m_countdownText = new wxStaticText(this, wxID_ANY,
            wxString::Format("Автоматическое закрытие через: %d секунд", m_countdown));
        mainSizer->Add(m_countdownText, 0, wxALL | wxALIGN_CENTER, 10);

        // Кнопки
        wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
        m_okButton = new wxButton(this, wxID_OK, "OK");
        m_cancelButton = new wxButton(this, wxID_CANCEL, "Отмена");

        buttonSizer->Add(m_okButton, 0, wxALL, 5);
        buttonSizer->Add(m_cancelButton, 0, wxALL, 5);

        mainSizer->Add(buttonSizer, 0, wxALIGN_CENTER | wxBOTTOM, 10);

        SetSizer(mainSizer);
        Centre();

        // Настройка таймера
        m_timer = new wxTimer(this, wxID_ANY);
        Bind(wxEVT_TIMER, &CountdownDialog::OnTimer, this);
        m_timer->Start(1000); // Таймер срабатывает каждую секунду

        // Привязка событий кнопок
        m_okButton->Bind(wxEVT_BUTTON, &CountdownDialog::OnOk, this);
        m_cancelButton->Bind(wxEVT_BUTTON, &CountdownDialog::OnCancel, this);
    }

    ~CountdownDialog()
    {
        if (m_timer && m_timer->IsRunning())
        {
            m_timer->Stop();
        }
    }

private:
    void OnTimer(wxTimerEvent &)
    {
        m_countdown--;

        if (m_countdown <= 0)
        {
            m_timer->Stop();
            // Выполняем действие по достижении нуля
            OnCountdownFinished();
        }
        else
        {
            // Обновляем текст отсчета
            m_countdownText->SetLabel(
                wxString::Format("Автоматическое закрытие через: %d секунд", m_countdown));
            Layout();
        }
    }

    void OnCountdownFinished()
    {
        // Действие при достижении нуля
        wxMessageBox("Обратный отсчет завершен! Выполняется действие...", "Информация");

        // Закрываем диалог с кодом OK
        EndModal(wxID_OK);
    }

    void OnOk(wxCommandEvent &)
    {
        m_timer->Stop();
        EndModal(wxID_OK);
    }

    void OnCancel(wxCommandEvent &)
    {
        m_timer->Stop();
        EndModal(wxID_CANCEL);
    }

private:
    wxStaticText *m_messageText;
    wxStaticText *m_countdownText;
    wxButton *m_okButton;
    wxButton *m_cancelButton;
    wxTimer *m_timer;
    int m_countdown;
    int m_initialCountdown;
};
