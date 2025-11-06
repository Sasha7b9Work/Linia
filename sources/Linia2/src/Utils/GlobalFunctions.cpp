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


wxRadioButton *GF::FindRadioButton(const wxWindow *w, int id)
{
    return (wxRadioButton *)w->FindWindow(id);
}


wxCheckBox *GF::FindCheckBox(wxWindow *window, int id)
{
    return (wxCheckBox *)window->FindWindow(id);
}


ButtonsCombo *GF::FindComboBox(const wxWindow *window, int id)
{
    return (ButtonsCombo *)window->FindWindow(id);
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


void GF::SendCommandEvent(wxWindow *window, int id, int type_event, int int_value)
{
    wxCommandEvent evt(type_event, id);
    evt.SetInt(int_value);
    wxPostEvent(window->GetEventHandler(), evt);
}


void GF::SendCommandEvent(wxWindow *object, int type_event, int int_value)
{
    wxCommandEvent event(type_event, object->GetId());
    event.SetEventObject(object);
    event.SetInt(int_value);
    wxPostEvent(object->GetEventHandler(), event);
}


bool GF::IsBoardPCM()
{
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


wxString GF::DirForModFiles()
{
    wxString path = wxGetCwd() + wxT("/mod");

    wxString normalizedPath = wxFileName::DirName(path).GetFullPath();

    if (!wxDirExists(path))
    {
        wxFileName::Mkdir(normalizedPath, wxS_DIR_DEFAULT, wxPATH_MKDIR_FULL);
    }

    return normalizedPath;
}


bool GF::ApproxEqual(double a, double b)
{
    if (std::signbit(a) != std::signbit(b))
    {
        return false;
    }

    a = std::abs(a);
    b = std::abs(b);

    if (std::abs(a - b) < std::numeric_limits<double>::epsilon())
    {
        return true;
    }

    double epsilon = 1.00002;

    if (a > b)
    {
        return a / b < epsilon;
    }
    else
    {
        return b / a < epsilon;
    }
}


wxString GF::GetSelfIP()
{
#ifdef WIN32

    return "";

#else

    struct ifaddrs *ifaddr, *ifa;
    int family, s;
    char host[NI_MAXHOST] = { '\0' };

    if (getifaddrs(&ifaddr) == -1)
    {
        return "ERROR getifaddrs()";
    }

    wxString result;

    // Проходим по всем интерфейсам
    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
    {
        if (ifa->ifa_addr == NULL)
            continue;

        family = ifa->ifa_addr->sa_family;

        // Отображаем IPv4 адреса
        if (family == AF_INET)
        {
            s = getnameinfo(ifa->ifa_addr, sizeof(struct sockaddr_in),
                host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);

            if (s != 0)
            {
                LOG_ERROR("getnameinfo() failed: %s", gai_strerror(s));

                continue;
            }

            if (wxString("lo") != ifa->ifa_name)
            {
                result += host;
                result += " ";
            }
        }
    }

    freeifaddrs(ifaddr);

    return result;

#endif
}
