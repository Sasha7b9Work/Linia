// 2026/03/09 10:24:13 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include <wx/wx.h>
#include <wx/dcbuffer.h>
#include <cmath>

class KnobWidget : public wxControl
{
public:
    KnobWidget(wxWindow *parent, wxWindowID id = wxID_ANY,
        int minValue = 0, int maxValue = 100, int initialValue = 50,
        const wxPoint &pos = wxDefaultPosition,
        const wxSize &size = wxDefaultSize)
        : wxControl(parent, id, pos, size, wxBORDER_NONE)
        , m_minValue(minValue)
        , m_maxValue(maxValue)
        , m_value(initialValue)
        , m_dragging(false)
    {
        // Включаем обработку событий мыши
        Bind(wxEVT_LEFT_DOWN, &KnobWidget::OnMouseLeftDown, this);
        Bind(wxEVT_LEFT_UP, &KnobWidget::OnMouseLeftUp, this);
        Bind(wxEVT_MOTION, &KnobWidget::OnMouseMove, this);
        Bind(wxEVT_MOUSE_CAPTURE_LOST, &KnobWidget::OnMouseCaptureLost, this);
        Bind(wxEVT_PAINT, &KnobWidget::OnPaint, this);
        Bind(wxEVT_SET_FOCUS, &KnobWidget::OnSetFocus, this);
        Bind(wxEVT_KILL_FOCUS, &KnobWidget::OnKillFocus, this);

        // Устанавливаем размер по умолчанию, если не задан
        if (size == wxDefaultSize)
            SetMinSize(wxSize(50, 50));

        // Устанавливаем курсор "рука" при наведении
        SetCursor(wxCursor(wxCURSOR_HAND));

        // Включаем двойную буферизацию для устранения мерцания
        SetBackgroundStyle(wxBG_STYLE_PAINT);
    }

    int GetValue() const
    {
        return m_value;
    }

    void SetValue(int value)
    {
        if (value < m_minValue) value = m_minValue;
        if (value > m_maxValue) value = m_maxValue;

        if (m_value != value)
        {
            m_value = value;
            Refresh();

            // Отправляем событие об изменении значения
            wxCommandEvent event(wxEVT_SLIDER, GetId());
            event.SetInt(m_value);
            event.SetEventObject(this);
            GetEventHandler()->ProcessEvent(event);
        }
    }

private:
    int m_minValue;
    int m_maxValue;
    int m_value;
    bool m_dragging;
    wxPoint m_capturePoint; // Точка, где был захвачен виджет

    void OnMouseLeftDown(wxMouseEvent &event)
    {
        if (!m_dragging)
        {
            m_dragging = true;
            m_capturePoint = event.GetPosition(); // Запоминаем позицию захвата
            CaptureMouse(); // Захватываем мышь для получения событий вне виджета
        }
        event.Skip();
    }

    void OnMouseLeftUp(wxMouseEvent &event)
    {
        if (m_dragging)
        {
            m_dragging = false;
            if (HasCapture())
                ReleaseMouse();
        }
        event.Skip();
    }

    void OnMouseMove(wxMouseEvent &event)
    {
        if (m_dragging && event.LeftIsDown())
        {
            wxPoint currentPos = event.GetPosition();

            // Вычисляем дельту относительно точки захвата
            int deltaY = currentPos.y - m_capturePoint.y;

            if (deltaY != 0)
            {
                // Изменяем значение в зависимости от направления движения
                // Тянем вверх (отрицательный deltaY) - увеличиваем значение
                // Тянем вниз (положительный deltaY) - уменьшаем значение
                int newValue = m_value - deltaY; // Минус потому что Y увеличивается вниз

                // Ограничиваем значение
                if (newValue < m_minValue) newValue = m_minValue;
                if (newValue > m_maxValue) newValue = m_maxValue;

                SetValue(newValue);

                // ВАЖНО: Возвращаем курсор мыши в исходную позицию
                // Это создает эффект, что курсор остается на месте при вращении
                WarpPointer(m_capturePoint.x, m_capturePoint.y);
            }
        }
        event.Skip();
    }

    void OnMouseCaptureLost(wxMouseCaptureLostEvent &event)
    {
        m_dragging = false;
    }

    void OnSetFocus(wxFocusEvent &event)
    {
        Refresh();
        event.Skip();
    }

    void OnKillFocus(wxFocusEvent &event)
    {
        Refresh();
        event.Skip();
    }

    void OnPaint(wxPaintEvent &event)
    {
        wxAutoBufferedPaintDC dc(this);
        dc.Clear();

        wxSize size = GetClientSize();
        int width = size.GetWidth();
        int height = size.GetHeight();
        int diameter = std::min(width, height) - 4;
        int x = (width - diameter) / 2;
        int y = (height - diameter) / 2;

        // Рисуем фон
        dc.SetBrush(wxBrush(GetBackgroundColour()));
        dc.SetPen(*wxTRANSPARENT_PEN);
        dc.DrawRectangle(0, 0, width, height);

        // Рисуем корпус ручки
        dc.SetBrush(wxBrush(wxColour(80, 80, 80))); // Тёмно-серый
        dc.SetPen(wxPen(wxColour(120, 120, 120), 1));
        dc.DrawEllipse(x, y, diameter, diameter);

        // Рисуем блик
        dc.SetBrush(*wxTRANSPARENT_BRUSH);
        dc.SetPen(wxPen(wxColour(220, 220, 220), 1));
        dc.DrawEllipse(x + 2, y + 2, diameter - 4, diameter - 4);

        // Вычисляем угол поворота (от -210° до +90°, диапазон 300°)
        double angle = (double)(m_value - m_minValue) / (m_maxValue - m_minValue) * 300.0 - 210.0;
        angle = angle * M_PI / 180.0; // Конвертируем в радианы

        // Рисуем указатель
        int centerX = x + diameter / 2;
        int centerY = y + diameter / 2;
        int radius = diameter / 2 - 6;

        int indicatorX = centerX + (int)(radius * cos(angle));
        int indicatorY = centerY + (int)(radius * sin(angle));

        dc.SetPen(wxPen(wxColour(255, 200, 0), 3)); // Жёлтый указатель
        dc.DrawLine(centerX, centerY, indicatorX, indicatorY);

        // Рисуем центральную точку
        dc.SetBrush(wxBrush(wxColour(255, 200, 0)));
        dc.SetPen(*wxTRANSPARENT_PEN);
        dc.DrawCircle(centerX, centerY, 3);

        // Опционально: рисуем метки минимального и максимального значения
        dc.SetPen(wxPen(wxColour(200, 200, 200), 1));

        // Метка минимума (слева)
        int minX = x + 5;
        int minY = y + diameter - 8;
        dc.DrawText(wxString::Format("%d", m_minValue), minX, minY);

        // Метка максимума (справа)
        wxString maxStr = wxString::Format("%d", m_maxValue);
        int textWidth, textHeight;
        dc.GetTextExtent(maxStr, &textWidth, &textHeight);
        int maxX = x + diameter - textWidth - 5;
        int maxY = y + 5;
        dc.DrawText(maxStr, maxX, maxY);

        // Рисуем фокус, если есть
        if (HasFocus())
        {
            dc.SetBrush(*wxTRANSPARENT_BRUSH);
            dc.SetPen(wxPen(*wxBLACK, 1, wxPENSTYLE_DOT));
            dc.DrawRectangle(0, 0, width, height);
        }
    }
};

// Пример использования в главном окне
/*
class MyFrame : public wxFrame
{
public:
    MyFrame() : wxFrame(nullptr, wxID_ANY, "Knob Widget Example", wxDefaultPosition, wxSize(400, 300))
    {
        wxPanel *panel = new wxPanel(this);

        // Создаём вертикальный sizer для размещения элементов
        wxBoxSizer *vSizer = new wxBoxSizer(wxVERTICAL);

        // Создаём горизонтальный sizer для ручек
        wxBoxSizer *hSizer = new wxBoxSizer(wxHORIZONTAL);

        // Создаём первую ручку (громкость)
        wxStaticBoxSizer *volumeBox = new wxStaticBoxSizer(wxVERTICAL, panel, "Volume");
        KnobWidget *volumeKnob = new KnobWidget(volumeBox->GetStaticBox(), wxID_ANY, 0, 100, 75, wxDefaultPosition, wxSize(100, 100));
        wxStaticText *volumeValue = new wxStaticText(volumeBox->GetStaticBox(), wxID_ANY, "75");

        volumeKnob->Bind(wxEVT_SLIDER, [volumeValue](wxCommandEvent &event)
            {
                volumeValue->SetLabel(wxString::Format("%d", event.GetInt()));
            });

        volumeBox->Add(volumeKnob, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 10);
        volumeBox->Add(volumeValue, 0, wxALIGN_CENTER_HORIZONTAL | wxBOTTOM, 10);

        // Создаём вторую ручку (тембр)
        wxStaticBoxSizer *toneBox = new wxStaticBoxSizer(wxVERTICAL, panel, "Tone");
        KnobWidget *toneKnob = new KnobWidget(toneBox->GetStaticBox(), wxID_ANY, -50, 50, 0, wxDefaultPosition, wxSize(100, 100));
        wxStaticText *toneValue = new wxStaticText(toneBox->GetStaticBox(), wxID_ANY, "0");

        toneKnob->Bind(wxEVT_SLIDER, [toneValue](wxCommandEvent &event)
            {
                toneValue->SetLabel(wxString::Format("%d", event.GetInt()));
            });

        toneBox->Add(toneKnob, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 10);
        toneBox->Add(toneValue, 0, wxALIGN_CENTER_HORIZONTAL | wxBOTTOM, 10);

        hSizer->Add(volumeBox, 1, wxEXPAND | wxALL, 10);
        hSizer->Add(toneBox, 1, wxEXPAND | wxALL, 10);

        vSizer->AddStretchSpacer();
        vSizer->Add(hSizer, 0, wxEXPAND);
        vSizer->AddStretchSpacer();

        // Добавляем пояснительный текст
        wxStaticText *infoText = new wxStaticText(panel, wxID_ANY,
            "Press and drag UP to increase, DOWN to decrease.\nMouse cursor stays in place while dragging.");
        infoText->SetForegroundColour(wxColour(100, 100, 100));
        vSizer->Add(infoText, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 10);

        panel->SetSizer(vSizer);
    }
};
*/
