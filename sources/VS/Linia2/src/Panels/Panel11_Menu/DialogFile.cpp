// 2025/7/12 10:59:56 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel11_Menu/DialogFile.h"
#include "Tests/Model.h"
#include "Panels/Panel04_Model.h"


DialogFile *DialogFile::self = nullptr;


DialogFile::DialogFile() :
    MenuDialog("Файл", 125, { 2, 4 },
        "Новый", []()
        {
            Model::CreateNew("Untitled");
            PanelModel::self->Update();
            DialogFile::self->Close(true);
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
            if (Model::IsModified())
            {
                wxMessageDialog dialog(self, "Файл модели был изменён. Сохранить изменения?", "Подтверждение", wxYES_NO | wxICON_QUESTION);

                if (dialog.ShowModal() == wxID_YES)
                {
                    wxFileDialog dialog_save(self, "Сохранить файл модели измерения", wxEmptyString, wxEmptyString, "*.mod", wxFD_SAVE);

                    if (dialog_save.ShowModal() == wxID_OK)
                    {

                    }
                }
            }

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

    bool empty = Model::IsEmpty();

    FindButton("Новый")->Enable(empty);
    FindButton("Открыть")->Enable(empty);
    FindButton("Закрыть")->Enable(!empty);
    FindButton("Cохранить")->Enable(!empty);
    FindButton("Сохранить как...")->Enable(!empty);
}
