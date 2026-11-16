// 2025/10/17 08:25:48 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once
#include "GUI/Controls/Button.h"
#include "GUI/Controls/StaticText.h"
#pragma warning(push, 0)
    #include <wx/sizer.h>
    #include <wx/dialog.h>
#pragma warning(pop)


class AutoRebootDialog : public wxDialog
{
public:
    AutoRebootDialog(wxWindow *parent, const wxString &message, int countdownSeconds, void (*_func_on_finish)()) :
        wxDialog(parent, wxID_ANY, "Ошибка", wxDefaultPosition, wxSize(350, 170), wxCAPTION | wxCLOSE_BOX | wxRESIZE_BORDER),
        countdown(countdownSeconds),
        func_on_finish(_func_on_finish)
    {
        wxASSERT_MSG(func_on_finish, "AutoRebootDialog: callback is required");

        wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

        // Текст сообщения
        messageText = new StaticText(this, message);
        mainSizer->Add(messageText, 0, wxALL | wxALIGN_CENTER, 10);

        // Текст отсчета
        countdownText = new StaticText(this, wxString::Format(L("Перезагрузка через %d секунд"), countdown));
        mainSizer->Add(countdownText, 0, wxALL | wxALIGN_CENTER, 10);

        // Кнопки
        wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
        okButton = new Button(this, L("Перезагрузить"));
        cancelButton = new Button(this, L("Отмена"));

        buttonSizer->Add(okButton, 0, wxALL, 5);
        buttonSizer->Add(cancelButton, 0, wxALL, 5);

        mainSizer->Add(buttonSizer, 0, wxALIGN_CENTER | wxBOTTOM, 20);

        SetSizer(mainSizer);
        CentreOnParent();

        timer.Start(1000); // Таймер срабатывает каждую секунду

        // Привязка событий кнопок
        okButton->Bind(wxEVT_BUTTON, [this](wxCommandEvent &)
            {
                timer.Stop();
                Hide();
                EndModal(wxID_OK);
                CallAfter(func_on_finish);
            });

        cancelButton->Bind(wxEVT_BUTTON, [this](wxCommandEvent &)
            {
                timer.Stop();
                Hide();
                EndModal(wxID_CANCEL);
            });

        timer.Bind(wxEVT_TIMER, [this](wxTimerEvent &)
            {
                countdown--;

                if (countdown <= 0)
                {
                    timer.Stop();
                    // Выполняем действие по достижении нуля
                    OnCountdownFinished();
                }
                else
                {
                    // Обновляем текст отсчета
                    countdownText->SetLabel(wxString::Format(L("Перезагрузка через %d секунд"), countdown));
                    Layout();
                }
            });

        Bind(wxEVT_CLOSE_WINDOW, [this](wxCloseEvent &e)
            {
                timer.Stop();
                Hide();
                e.Skip();
            });

        // Закрытие по кнопке Esc
        Bind(wxEVT_CHAR_HOOK, [this](wxKeyEvent &e)
            {
                if (e.GetKeyCode() == WXK_ESCAPE)
                {
                    timer.Stop();
                    Hide();
                    EndModal(wxID_CANCEL);
                }
                else
                {
                    e.Skip();
                }
            });
    }

    ~AutoRebootDialog()
    {
        timer.Stop();
    }

private:

    void OnCountdownFinished()
    {
        Hide();
        EndModal(wxID_OK);
        CallAfter(func_on_finish);
    }

private:
    StaticText  *messageText;
    StaticText   *countdownText;
    Button       *okButton;
    Button       *cancelButton;
    wxTimer timer{ this, wxID_ANY };
    int           countdown;
    void (*func_on_finish)() = nullptr;
};
