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

public:

    static const int WIDTH = MainWindow::WIDTH2;
    static const int HEIGHT = 520;

    PanelGraph(wxWindow *parent);

    static PanelGraph *self;

    void Draw();

    void DrawLine(int x1, int y1, int x2, int y2);
    void DrawLine(int x1, int y1, int x2, int y2, const wxColor &);

private:

    Grid grid;                              // Координатная сетка

    std::vector<GraphEntity *> entities;    // Сущности для отрисовки

    static wxBitmap bitmap;
    wxGraphicsContext *gc = nullptr;        // Используется для рисования
    wxMemoryDC dc;                          // А здесь хранится то, что нарисовано, пока не будет вызвано событие wxEVT_PAINT
    wxColor color;

    // Координаты мыши при нажатии кнопки
    wxPoint pos_mouse_down;
    bool mouse_is_pressed = false;

    void BeginPaint();

    void EndPaint();

    void OnPaint(wxPaintEvent &);

    void OnMouseDown(wxMouseEvent &);

    void OnMouseMove(wxMouseEvent &);

    void OnMouseUp(wxMouseEvent &);

    void OnMouseWheel(wxMouseEvent &);

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
