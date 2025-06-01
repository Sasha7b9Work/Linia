// 2024/05/28 12:04:56 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


// Автоматически перемещается таким образом, что кнопка "Закрыть" находится под мышкой
class AutoMovedDialog : public wxDialog
{
public:

    AutoMovedDialog(const wxString &title) :
        wxDialog(nullptr, wxID_ANY, title)
    {
    }

    virtual int ShowModal() override
    {
        MoveToMousePosition();

        return wxDialog::ShowModal();
    }

protected:

    // Кнопка "Закрыть" в позиции мыши
    void MoveToMousePosition()
    {
        int x = 0;
        int y = 0;

        wxGetMousePosition(&x, &y);

        x -= GetSize().x - 20;
        y -= 15;

        SetPosition({ x, y });
    }

    void SetFixedSize(const wxSize &size)
    {
        SetMinClientSize(size);
        SetMaxClientSize(size);
        SetClientSize(size);
    }
};
