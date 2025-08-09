// 2025/8/9 11:41:23 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls/Buttons.h"

// Кнопка с рисунком, по нажатию на которую открывается окно с другими выборами


class BmpButtonsCombo : public ButtonBitmap
{
public:

    BmpButtonsCombo(wxWindow *parent, const wxString &title, const wxPoint &pos, const wxSize &,
        const wxArrayString &files,             // Эти изображения будут на кнопках
        const wxArrayString &tooltips,
        int num_file,                           // Эта изображение будет на главной кнопке
        int buttons_in_row);                    // В каждом ряду будет расположено столько кнопок

private:

    class ButtonPopup : public wxPopupTransientWindow
    {
    public:
        ButtonPopup(wxWindow *parent, const wxString &title, const wxArrayString &files, const wxArrayString &tooltips, int buttons_in_row) :
            wxPopupTransientWindow(parent, wxBORDER_SUNKEN)
        {
            SetWindowStyle(wxBORDER_SIMPLE | wxPU_CONTAINS_CONTROLS);

            Bind(wxEVT_SHOW, &ButtonPopup::OnShow, this);

            // Основной контейнер с отступами по краям
            wxBoxSizer *outerSizer = new wxBoxSizer(wxVERTICAL);
            wxPanel *mainPanel = new wxPanel(this, wxID_ANY);
            mainPanel->SetBackgroundColour(*wxWHITE);

            int num_rows = (int)(files.size() / buttons_in_row);

            if (num_rows * buttons_in_row < (int)files.size())
            {
                num_rows++;
            }

            int num_cols = (int)(files.size() / num_rows);

            if (num_rows * num_cols < (int)files.size())
            {
                num_cols++;
            }

            wxGridSizer *gridSizer = new wxGridSizer(num_rows, num_cols, 5, 5); // 5px промежутки

            for (uint i = 0; i < files.size(); ++i)
            {
                ButtonBitmap *btn = new ButtonBitmap(mainPanel, wxDefaultPosition, wxDefaultSize, files[i]);
                btn->SetToolTip(tooltips[i]);
                btn->Bind(wxEVT_BUTTON, &ButtonPopup::OnButtonClick, this);
                gridSizer->Add(btn, 0, wxEXPAND | wxALL, 2); // 2px отступы у кнопок
            }

            // Добавляем рамку вокруг сетки кнопок
            wxStaticBoxSizer *boxSizer = new wxStaticBoxSizer(wxVERTICAL, mainPanel, title);
            boxSizer->Add(gridSizer, 1, wxEXPAND | wxALL, 0); // 10px отступ внутри рамки

            // Основная панель
            mainPanel->SetSizer(boxSizer);

            // Внешние отступы 15px
            outerSizer->Add(mainPanel, 1, wxEXPAND | wxALL, 5);
            SetSizer(outerSizer);

            Fit(); // Автоподбор размера
        }

        // Новая функция для показа
        void ShowPopup(const wxPoint &pos)
        {
            if (IsShown())
            {
                Hide();
                // Явно сбрасываем обработчики
                GetEventHandler()->SetNextHandler(nullptr);
            }

            Position(pos, wxSize(0, 0));
            Popup(); // Активируем режим popup
        }

    private:

        void OnShow(wxShowEvent &event)
        {
            if (event.IsShown())
            {
                // Сохраняем предыдущий фокус при показе
                m_prevFocus = FindFocus();
                SetFocus(); // Устанавливаем фокус на popup
            }
            else
            {
                // Восстанавливаем фокус при скрытии
                if (m_prevFocus)
                {
                    m_prevFocus->SetFocus();
                    m_prevFocus = nullptr;
                }
            }
            event.Skip();
        }
        virtual void OnDismiss() override
        {
            Hide();

            if (GetEventHandler())
            {
                // Важно: отвязываем все дополнительные обработчики
//                GetEventHandler()->SetNextHandler(nullptr);
            }
        }

        void OnButtonClick(wxCommandEvent &)
        {
            Hide();
        }

        wxWindow *m_prevFocus = nullptr;

        // Отключаем копирование
        ButtonPopup(const ButtonPopup &) = delete;
        ButtonPopup &operator=(const ButtonPopup &) = delete;
    };

    ButtonPopup *popup = nullptr;

    void OnButtonClicked(wxCommandEvent &);
};
