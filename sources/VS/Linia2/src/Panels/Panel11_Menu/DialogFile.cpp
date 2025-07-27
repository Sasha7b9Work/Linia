// 2025/7/12 10:59:56 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel11_Menu/DialogFile.h"


enum
{
    ID_BTN_MENU_FILE_NEW = wxID_HIGHEST + 1,
    ID_BTN_MENU_FILE_OPEN,
    ID_BTN_MENU_FILE_CLOSE,
    ID_BTN_MENU_FILE_SAVE,
    ID_BTN_MENU_FILE_SAVE_AS,
    ID_BTN_MENU_FILE_DELETE
};


DialogFile *DialogFile::self = nullptr;


DialogFile::DialogFile() :
    MenuDialog(_L("Файл"), 125,
        _L("Новый"), ID_BTN_MENU_FILE_NEW, []()
        {
            wxFileDialog dialog(self, _L("Новый файл модели измерения"), wxEmptyString, wxEmptyString, "*.mod", wxFD_SAVE);

            if (dialog.ShowModal() == wxID_OK)
            {

            }
            else
            {

            }
        },
        _L("Открыть"), ID_BTN_MENU_FILE_OPEN, []()
        {
            wxFileDialog dialog(self, _L("Открыть файл модели измерения"), wxEmptyString, wxEmptyString, "*.mod", wxFD_OPEN);

            if (dialog.ShowModal() == wxID_OK)
            {

            }
            else
            {

            }
        },
        _L("Закрыть"), ID_BTN_MENU_FILE_CLOSE, []()
        {
            DialogFile::self->Close(true);
        },
        _L("Cохранить"), ID_BTN_MENU_FILE_SAVE, []()
        {
            DialogFile::self->Close(true);
        },
        _L("Сохранить как..."), ID_BTN_MENU_FILE_SAVE_AS, []()
        {
            wxFileDialog dialog(self, _L("Сохранить файл модели измерения"), wxEmptyString, wxEmptyString, "*.mod", wxFD_SAVE);

            if (dialog.ShowModal() == wxID_OK)
            {

            }
            else
            {

            }
        },
        _L("Удалить"), ID_BTN_MENU_FILE_DELETE, []()
        {

        }
    )
{
    self = this;
}
