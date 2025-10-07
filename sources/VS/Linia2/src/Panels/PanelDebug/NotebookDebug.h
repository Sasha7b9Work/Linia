// 2025/6/3 14:06:45 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class NotebookDebug : public wxNotebook
{
public:

    NotebookDebug(wxWindow *parent);

    static NotebookDebug *self;

    void Init();

    void Update();

    void DeInit();

private:

    void AppendNewPage(wxPanel *);
};
