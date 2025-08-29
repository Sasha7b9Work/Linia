// 2025/7/12 10:59:56 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel11_Menu/DialogFile.h"
#include "Tests/Model.h"


DialogFile *DialogFile::self = nullptr;


DialogFile::DialogFile() :
    MenuDialog("Файл", 125, { 2, 4 },
        "Новый", []()
        {
            wxFileDialog dialog(self, "Новый файл модели измерения", wxEmptyString, wxEmptyString, "*.mod", wxFD_SAVE);

            if (dialog.ShowModal() == wxID_OK)
            {

            }
            else
            {

            }
        },
        "Открыть", []()
        {
            wxFileDialog dialog(self, "Открыть файл модели измерения", wxEmptyString, wxEmptyString, "*.mod", wxFD_OPEN);

            if (dialog.ShowModal() == wxID_OK)
            {

            }
            else
            {

            }
        },
        "Закрыть", []()
        {
            DialogFile::self->Close(true);
        },
        "Cохранить", []()
        {
            DialogFile::self->Close(true);
        },
        "Сохранить как...", []()
        {
            wxFileDialog dialog(self, "Сохранить файл модели измерения", wxEmptyString, wxEmptyString, "*.mod", wxFD_SAVE);

            if (dialog.ShowModal() == wxID_OK)
            {

            }
            else
            {

            }
        },
        "Удалить", []()
        {

        }
    )
{
    self = this;

    bool loaded = Model::IsLoaded();

    FindButton("Новый")->Enable(!loaded);
    FindButton("Открыть")->Enable(!loaded);
    FindButton("Закрыть")->Enable(loaded);
    FindButton("Cохранить")->Enable(loaded);
    FindButton("Сохранить как...")->Enable(loaded);
}
