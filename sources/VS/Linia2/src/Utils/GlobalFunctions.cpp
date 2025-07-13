// 2023/08/10 19:21:00 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Utils/GlobalFunctions.h"
#include "MainWindow.h"


void GF::HorizontalAlignCenter(wxStaticText *label , wxWindow *border)
{
    wxBitmap bitmap("BITMAP_CARD");

    wxMemoryDC dc(bitmap);

    dc.SetFont(label->GetFont());

    wxSize size = dc.GetTextExtent(label->GetLabelText());

    size.x += 4;

    label->SetClientSize(size);

    wxPoint position = label->GetPosition();

    position.x = border->GetSize().x / 2 - size.x / 2;

    label->SetPosition(position);
}


wxStaticText *GF::FindStaticText(wxWindow *window, int id)
{
    return (wxStaticText *)window->FindWindow(id);
}


wxAnyButton *GF::FindAnyButton(wxWindow *window, int id)
{
    return (wxAnyButton *)window->FindWindow(id);
}


wxToggleButton *GF::FindToggleButton(wxWindow *w, int id)
{
    return (wxToggleButton *)w->FindWindow(id);
}


wxRadioButton *GF::FindRadioButton(wxWindow *w, int id)
{
    return (wxRadioButton *)w->FindWindow(id);
}


wxCheckBox *GF::FindCheckBox(wxWindow *window, int id)
{
    return (wxCheckBox *)window->FindWindow(id);
}


CheckButton *GF::FindCheckButton(const wxWindow *window, int id)
{
    return (CheckButton *)window->FindWindow(id);
}


wxComboBox *GF::FindComboBox(const wxWindow *window, int id)
{
    return (wxComboBox *)window->FindWindow(id);
}


wxTextCtrl *GF::FindTextCtrl(wxWindow *window, int id)
{
    return (wxTextCtrl *)window->FindWindow(id);
}


wxSpinCtrl *GF::FindSpinCtrl(wxWindow *window, int id)
{
    return (wxSpinCtrl *)window->FindWindow(id);
}


int GF::IntFromTextCtrl(wxWindow *window, int id)
{
    wxTextCtrl *control = FindTextCtrl(window, id);

    wxString value = control->GetValue();

    int result = 0;

    value.ToInt(&result);

    return result;
}


uint64 GF::UInt64FromTextCtrl(wxWindow *window, int id)
{
    wxTextCtrl *control = FindTextCtrl(window, id);

    wxString value = control->GetValue();

    uint64 result = 0;

    value.ToULongLong(&result);

    return result;
}


void GF::IntToTextCtrl(wxWindow *window, int id, int value)
{
    FindTextCtrl(window, id)->SetValue(wxString::Format("%d", value));
}


void GF::ShowModalInfo(wxWindow *windows, const wxString &text)
{
    wxMessageDialog(windows, text, "", wxCENTRE | wxICON_INFORMATION | wxOK).ShowModal();
}


void GF::SendCommandEvent(wxWindow *window, int type_event, int id, int int_value)
{
    wxCommandEvent evt(type_event, id);
    evt.SetInt(int_value);
    window->ProcessWindowEvent(evt);
}


bool GF::IsBoardPCM()
{
    const wxSize size(MainWindow::WIDTH, MainWindow::HEIGHT);

    wxSize screenSize = wxGetDisplaySize(); // Получить общее разрешение экрана
    int width = screenSize.GetWidth();
    int height = screenSize.GetHeight();

    return (width == MainWindow::WIDTH) && (height == MainWindow::HEIGHT);
}


wxPoint GF::GetCoordCenter(const wxSize &size)
{
    wxPoint pos_win = { (MainWindow::WIDTH - size.x) / 2, (MainWindow::HEIGHT - size.y) / 2 };

    if (!IsBoardPCM())
    {
        pos_win += MainWindow::self->GetPosition();
    }

    return pos_win;
}
