// 2022/10/28 23:17:02 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Panels/Panel.h"
#include "MainWindow.h"
#include "Panels/Panel05_Graph/Grid.h"
#include "Panels/Panel05_Graph/GraphEntity.h"


class PanelGraph : public Panel
{
    friend class Point;
    friend class Text;
    friend class Spline;

public:

    static const int WIDTH = MainWindow::WIDTH2;
    static const int HEIGHT = 520;

    PanelGraph(wxWindow *parent);

    static PanelGraph *self;

    void Draw();

    void DrawLine(int x1, int y1, int x2, int y2);
    void DrawLine(int x1, int y1, int x2, int y2, const wxColor &);

    void SetColor(const wxColor &_color)
    {
        color = _color;
    }

    bool track_x = false;           // Отслеживать координату x
    bool track_y = false;           // Отслеживать координату y
    bool track_none = true;         // Не отслеживать координаты
    bool mouse_is_pressed = false;

private:

    Grid grid;                              // Координатная сетка

    std::vector<GraphEntity *> entities;    // Сущности для отрисовки

    static wxBitmap bitmap;
    wxGraphicsContext *gc = nullptr;        // Используется для рисования
    wxMemoryDC dc;                          // А здесь хранится то, что нарисовано, пока не будет вызвано событие wxEVT_PAINT
    wxColor color;

    // Координаты мыши при нажатии кнопки
    wxPoint pos_mouse_down;

    void BeginPaint();

    void EndPaint();

    void OnRightClick(wxMouseEvent &);

    void OnPaint(wxPaintEvent &);

    void OnMouseDown(wxMouseEvent &);

    void OnMouseMove(wxMouseEvent &);

    void OnMouseUp(wxMouseEvent &);

    void OnMouseWheel(wxMouseEvent &);

    void OnMenuReset(wxCommandEvent &);
    void OnMenuTrackX(wxCommandEvent &);
    void OnMenuTrackY(wxCommandEvent &);
    void OnMenuTrackNone(wxCommandEvent &);

    void FillRectangle(int x, int y, int width, int height, const wxColor &);

    void CreateEntities();
};


class Point
{
public:

    void Draw(int x, int y) const;
};


class Text
{
public:

    Text(const wxString &);

    static void SetFont();

    void Draw(int x, int y) const;

    // Рисует слева от точки по центру
    void DrawAboutCenterLeft(int x, int y) const;

    void DrawAboutCenterRigth(int x, int y, bool fillBackground = false, const wxColor & = *wxWHITE) const;

    // Рисует снизу по центру
    void DrawAboutCenterDown(int x, int y, bool fillBackground = false, const wxColor & = *wxWHITE) const;

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

    std::vector<wxPoint2DDouble> points;
};
