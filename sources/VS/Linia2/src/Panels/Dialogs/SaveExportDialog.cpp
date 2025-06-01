// 2024/03/07 08:45:30 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Dialogs/SaveExportDialog.h"
#include "Settings/Settings.h"
#include "DataBase/DataBaseCards.h"
#include "Utils/GlobalFunctions.h"


SaveExportDialog::SaveExportDialog() :
    AutoMovedDialog(_L("Сохранить"))
{
    int x0 = 20;
    int dY = 20;

    int y = dY;

    new wxButton(this, ID_BUTTON_SAVE_CONFIG, _L("Конфигурация"), { x0, y }, BUTTON_SIZE);

    y += BUTTON_SIZE.GetHeight() + dY;

    new wxButton(this, ID_BUTTON_SAVE_USER_CARDS, _L("Список карт"), { x0, y }, BUTTON_SIZE);

    y += BUTTON_SIZE.GetHeight() + dY;

//    new wxButton(this, ID_BUTTON_SAVE_PASSWORDS, _L("Пароли"), { x0, y }, BUTTON_SIZE);

    y += BUTTON_SIZE.GetHeight() + dY + dY / 2;

    int x = (BUTTON_SIZE.GetWidth() + x0 * 2) / 2 - BUTTON_SIZE_SMALL.GetWidth() / 2;

    new wxButton(this, wxID_CANCEL, _L_CLOSE, { x, y }, BUTTON_SIZE_SMALL);

    Bind(wxEVT_BUTTON, &SaveExportDialog::OnEventButton, this);

    y += BUTTON_SIZE.GetHeight() + dY;

    wxSize client_size(BUTTON_SIZE.GetWidth() + x0 * 2, y);

    AutoMovedDialog::SetClientSize(client_size);
}


void SaveExportDialog::OnEventButton(wxCommandEvent &event)
{
    int id = event.GetId();

    if (id != wxID_CANCEL)
    {
        wxString title = _L("Сохранить конфигурацию");
        wxString type = _L("Файлы конфигураций");
        wxString ext = "(*.conf) | *.conf";

        switch (id)
        {
        case ID_BUTTON_SAVE_CONFIG:
            break;

        case ID_BUTTON_SAVE_USER_CARDS:
            title = _L("Сохранить список карт");
            type = _L("Файлы списков карт");
            ext = SET::OFFLINE::enabled.Get() ? "(*.auto) | *.auto" : "(*.cards) | *.cards";
            break;

        case ID_BUTTON_SAVE_PASSWORDS:
            title = _L("Сохранить список паролей");
            type = _L("Файлы списков паролей");
            ext = "(*.pass) | *.pass";
            break;
        }

        wxFileDialog dialog(this, title, wxEmptyString, wxEmptyString, type + " " + ext, wxFD_SAVE);

        if (dialog.ShowModal() == wxID_OK)
        {
            switch (id)
            {
            case ID_BUTTON_SAVE_CONFIG:

                SET::Save(dialog.GetPath());

                Close();

                GF::ShowModalInfo(this, _L("Конфигурация сохранена"));

                break;

            case ID_BUTTON_SAVE_USER_CARDS:
                {
                    CardsDB::SaveDataBase();
                    wxString file_name = dialog.GetFilename();
                    if (SET::OFFLINE::enabled.Get())
                    {
                        SET::OFFLINE::file_cards.Set(file_name);
                    }
                    else
                    {
                        SET::USER::file_cards.Set(file_name);
                    }

                    if (!wxFileExists(dialog.GetPath()))
                    {
                        wxTextFile file(dialog.GetPath());

                        file.Create();
                        file.Close();
                    }

                    CardsDB::SaveDataBaseToFile(file_name);

                    GF::ShowModalInfo(this, _L("Список карт сохранён"));

                    Close();
                }
                break;

            case ID_BUTTON_SAVE_PASSWORDS:
                break;
            }
        }
    }

    event.Skip();
}
