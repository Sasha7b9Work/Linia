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
    wxPoint m_lastMousePos;

    void OnMouseLeftDown(wxMouseEvent &event)
    {
        if (!m_dragging)
        {
            m_dragging = true;
            m_lastMousePos = event.GetPosition();
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
            int deltaY = currentPos.y - m_lastMousePos.y;

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

                // Сохраняем текущую позицию для следующего шага
                m_lastMousePos = currentPos;
            }
        }
        event.Skip();
    }

    void OnMouseCaptureLost(wxMouseCaptureLostEvent &)
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

    void OnPaint(wxPaintEvent &)
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

        // Вычисляем угол поворота
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
    MyFrame() : wxFrame(nullptr, wxID_ANY, "Knob Widget Example", wxDefaultPosition, wxSize(300, 200))
    {
        wxPanel *panel = new wxPanel(this);

        // Создаём вертикальный sizer для размещения элементов
        wxBoxSizer *vSizer = new wxBoxSizer(wxVERTICAL);

        // Создаём ручку
        KnobWidget *knob = new KnobWidget(panel, wxID_ANY, 0, 100, 50, wxDefaultPosition, wxSize(100, 100));

        // Создаём текстовое поле для отображения значения
        wxStaticText *valueText = new wxStaticText(panel, wxID_ANY, "Value: 50");

        // Обработчик изменения значения ручки
        knob->Bind(wxEVT_SLIDER, [valueText](wxCommandEvent &event)
            {
                valueText->SetLabel(wxString::Format("Value: %d", event.GetInt()));
            });

        vSizer->AddStretchSpacer();
        vSizer->Add(knob, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 10);
        vSizer->Add(valueText, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
        vSizer->AddStretchSpacer();

        panel->SetSizer(vSizer);
    }
};
*/
