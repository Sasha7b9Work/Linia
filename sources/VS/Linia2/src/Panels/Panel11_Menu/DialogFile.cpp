// 2025/7/12 10:59:56 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel11_Menu/DialogFile.h"


DialogFile *DialogFile::self = nullptr;


DialogFile::DialogFile() :
    MenuDialog(_L("Файл"), 125,
        _L("Новый"), []()
        {
            wxFileDialog dialog(self, _L("Новый файл модели измерения"), wxEmptyString, wxEmptyString, "*.mod", wxFD_SAVE);

            if (dialog.ShowModal() == wxID_OK)
            {

            }
            else
            {

            }
        },
        _L("Открыть"), []()
        {
            wxFileDialog dialog(self, _L("Открыть файл модели измерения"), wxEmptyString, wxEmptyString, "*.mod", wxFD_OPEN);

            if (dialog.ShowModal() == wxID_OK)
            {

            }
            else
            {

            }
        },
        _L("Закрыть"), []()
        {
            DialogFile::self->Close(true);
        },
        _L("Cохранить"), []()
        {
            DialogFile::self->Close(true);
        },
        _L("Сохранить как..."), []()
        {
            wxFileDialog dialog(self, _L("Сохранить файл модели измерения"), wxEmptyString, wxEmptyString, "*.mod", wxFD_SAVE);

            if (dialog.ShowModal() == wxID_OK)
            {

            }
            else
            {

            }
        },
        _L("Удалить"), []()
        {

        }
    )
{
    self = this;
}
