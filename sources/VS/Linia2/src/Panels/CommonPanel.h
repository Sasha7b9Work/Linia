// 2023/07/04 18:00:56 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class CntrlText4x2;


struct StateCard
{
    enum E
    {
        NoneCard,           // нет карты
        NotRecognized,      // карта не распознана
        Master,             // мастер-карта
        Client              // карта №
    };
};


struct StateReader
{
    enum E
    {
        NotConnected,
        Connected
    };
};


class CommonPanel : public wxWindow
{
public:

    static const int WIDTH = 200 + BUTTON_WIDTH_BIG - BUTTON_WIDTH;
    static const int HEIGHT = 500;

    CommonPanel(wxFrame *);
    virtual ~CommonPanel();

    static CommonPanel *self;

    void CallbackOnReaderConnected(bool);

    void CallbackOnTimer();

private:

    static wxSize SIZE_BUTTON;

    void OnEventButton(wxCommandEvent &);
};
