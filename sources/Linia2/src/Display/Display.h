// 2022/10/28 23:17:02 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Panels/Panel.h"
#include "Display/Grid/Grid.h"
#include "Display/PanelErrors.h"
#include "Display/Graphics/GraphEntity.h"


// Здесь отрисовывается график ну и кнопки разные


class Display : public Panel
{
    friend class Point;
    friend class Text;
    friend class Spline;
    friend class Line;
    friend class Rect;
    friend class MenuDisplay;
    friend class GraphMeasures;

public:

    Display(wxWindow *parent);
    ~Display();

    static Display *self;

    void SetColorBrush(const wxColor &);
    void SetColorPen(const wxColor &);
    void LoadColors();

    bool track_x = false;           // Отслеживать координату x
    bool track_y = false;           // Отслеживать координату y
    bool track_none = true;         // Не отслеживать координаты
    bool mouse_is_pressed = false;
    bool full_screen = false;       // График рисуется на полный экран

    PanelErrors *panel_errors = nullptr;

    void FullScreen(bool);

    void FillRectangle(int x, int y, int width, int height, const wxColor &);

    void OnEventCnangeMeasuredElement();

    // Возвращает размер области для отрисовки
    wxSize GetDrawingSize() const;

    // Возврщает полнлный размер виджета
    wxSize GetFullSize() const;

    std::vector<GraphEntity *> entities;    // Сущности для отрисовки

    void ReInit();

private:

    wxButton *btnHelp = nullptr,
        *btnLessX = nullptr,
        *btnMoreX = nullptr,
        *btnLessY = nullptr,
        *btnMoreY = nullptr;

    std::vector<wxButton *> buttons;        // Здесь все кнопки, у которых нужно менять цвета

    wxBitmap *bitmap = nullptr;
    wxGraphicsContext *gc = nullptr;        // Используется для рисования
    wxMemoryDC dc;                          // А здесь хранится то, что нарисовано, пока не будет вызвано событие wxEVT_PAINT
    wxColor color_pen;
    wxColor color_brush;

    // Координаты мыши при нажатии кнопки
    wxPoint pos_mouse_down;

    void Init();

    void BeginPaint();

    void EndPaint();

    void OnEventRightClick(wxMouseEvent &);
    void OnEventPaint(wxPaintEvent &);
    void OnEventMouseDown(wxMouseEvent &);
    void OnEventMouseUp(wxMouseEvent &);
    void OnEventMouseMove(wxMouseEvent &);
    void OnEventMouseWheel(wxMouseEvent &);
    void OnLeaveWindow(wxMouseEvent &);
    void OnEventButton(wxCommandEvent &);

    wxSize GetSize() const;
    wxSize GetClientSize() const;
};


class Point
{
public:

    void Draw(int x, int y) const;
};


class Line
{
public:

    Line(int _x1, int _y1, int _x2, int _y2) :
        x1(_x1), y1(_y1), x2(_x2), y2(_y2) { }

    void Draw() const;
    void Draw(const wxColor &) const;

private:
    int x1, y1, x2, y2;
};


class Rect
{
public:

    Rect(int _w, int _h) : width(_w), height(_h) { }

    void Fill(int x, int y, const wxColor &) const;
    void Draw(int x, int y, const wxColor &) const;

private:
    int width, height;
};


class Text
{
public:

    Text(const wxString &);

    static void SetFont();

    void Draw(int x, int y) const;

    // Рисует слева от точки по центру
    void DrawAboutCenterLeft(int x, int y, bool fillBackground) const;

    void DrawAboutCenterRigth(int x, int y, bool fillBackground) const;

    // Рисует снизу по центру
    void DrawAboutCenterDown(int x, int y, bool fillBackground) const;

    void DrawAboutCenterUp(int x, int y, bool fillBackground) const;
    void DrawAboutRightUp(int x, int y, bool fillBackground) const;

private:

    wxString text;
};


class Spline
{
public:

    void AppendPoint(const wxPoint2DDouble &);

    // smooth - сглаживание
    void Draw(bool smooth, bool draw_points) const;

private:

    std::vector<wxPoint2DDouble> rel_points;    // Здесь хранятся значения, приведённые к координатам экрана
};
