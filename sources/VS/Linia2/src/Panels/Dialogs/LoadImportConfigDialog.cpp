// 2024/03/06 15:40:40 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Dialogs/LoadImportConfigDialog.h"
#include "Settings/Settings.h"
#include "DataBase/DataBaseCards.h"
#include "Utils/GlobalFunctions.h"


LoadImportConfigDialog::LoadImportConfigDialog() : 
    AutoMovedDialog(_L("Загрузить"))
{
    int x0 = 20;
    int dY = 20;

    int y = dY;

    new wxButton(this, ID_BUTTON_LOAD_CONFIG, _L("Конфигурация"), { x0, y }, BUTTON_SIZE);

    y += BUTTON_SIZE.GetHeight() + dY;

    new wxButton(this, ID_BUTTON_LOAD_USER_CARDS, _L("Список карт"), { x0, y }, BUTTON_SIZE);

    y += BUTTON_SIZE.GetHeight() + dY;

//    new wxButton(this, ID_BUTTON_LOAD_PASSWORDS, _L("Пароли"), { x0, y }, BUTTON_SIZE);

    y += BUTTON_SIZE.GetHeight() + dY + dY / 2;

    int x = (BUTTON_SIZE.GetWidth() + x0 * 2) / 2 - BUTTON_SIZE_SMALL.GetWidth() / 2;

    new wxButton(this, wxID_CANCEL, _L_CLOSE, { x, y }, BUTTON_SIZE_SMALL);

    Bind(wxEVT_BUTTON, &LoadImportConfigDialog::OnEventButton, this);

    y += BUTTON_SIZE.GetHeight() + dY;

    wxSize client_size(BUTTON_SIZE.GetWidth() + x0 * 2, y);

    AutoMovedDialog::SetClientSize(client_size);
}


void LoadImportConfigDialog::OnEventButton(wxCommandEvent &event)
{
    int id = event.GetId();

    if (id != wxID_CANCEL)
    {
        wxString title = _L("Загрузить конфигурацию");
        wxString type = _L("Файлы конфигураций");
        wxString ext = "(*.conf) | *.conf";

        switch (id)
        {
        case ID_BUTTON_LOAD_CONFIG:
            break;

        case ID_BUTTON_LOAD_USER_CARDS:
            title = _L("Прочитать список карт");
            type = _L("Файлы списков карт");
            ext = SET::OFFLINE::enabled.Get() ? "(*.auto) | *.auto" : "(*.cards) | *.cards";
            break;

        case ID_BUTTON_LOAD_PASSWORDS:
            title = _L("Прочитать список паролей");
            type = _L("Файлы списков паролей");
            ext = "(*.pass) | *.pass";
            break;
        }

        wxFileDialog dialog(this, title, wxEmptyString, wxEmptyString, type + " " + ext, wxFD_OPEN);

        if (dialog.ShowModal() == wxID_OK)
        {
            switch (id)
            {
            case ID_BUTTON_LOAD_CONFIG:
                {
                    SET::Load(dialog.GetPath());
                    SET::Save("");
                    Close();
                    GF::ShowModalInfo(this, _L("Конфигурация загружена"));
                }
                break;

            case ID_BUTTON_LOAD_USER_CARDS:
                if (SET::OFFLINE::enabled.Get())
                {
                    SET::OFFLINE::file_cards.Set(dialog.GetFilename());
                }
                else
                {
                    SET::USER::file_cards.Set(dialog.GetFilename());
                }
                CardsDB::LoadDataBase();
                Close();
                break;

            case ID_BUTTON_LOAD_PASSWORDS:
                break;
            }
        }
    }

    event.Skip();
}
