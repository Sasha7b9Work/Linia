// 2023/07/04 17:52:08 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Panels/Notebook/Pages.h"
#include "Reader/Card.h"
#include "Panels/Notebook/PageUser/ListCards.h"


class CntrlMemBlock;


class PainterProgress : public Painter
{
public:
    PainterProgress(wxWindow *parent, wxPoint position, wxSize size);

    virtual void OnPaint(wxPaintEvent &) override;

    void SetValue(float);

private:

    wxBitmap *bitmap = nullptr;
};


class PageUser : public Page
{
public:

    PageUser(wxNotebook *parent);

    static PageUser *self;

    void SetNextCardNumber();

    void OnEventCheckBox(wxCommandEvent &);

    void OnEventEnterPage();

    struct Progress
    {
        void ResetAndShow();

        void Update(float);

        void Hide();

        PainterProgress *painter = nullptr;
    };

    Progress progress;

private:

    ListCards *list_cards = nullptr;

    void OnEventButton(wxCommandEvent &);

    void OnFocusEvent(wxFocusEvent &);

    void OnKeyEvent(wxKeyEvent &);

    void OnEventText(wxCommandEvent &);
};
