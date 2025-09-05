// 2025/7/12 10:59:56 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel11_Menu/DialogFile.h"
#include "Tests/Model.h"
#include "Panels/Panel04_Model.h"
#include "Controls/RestrictedFileDialog.h"


DialogFile *DialogFile::self = nullptr;


const pchar DialogFile::BTN_NEW = "Новый";
const pchar DialogFile::BTN_OPEN = "Открыть";
const pchar DialogFile::BTN_CLOSE = "Закрыть";
const pchar DialogFile::BTN_SAVE = "Cохранить";
const pchar DialogFile::BTN_SAVE_AS = "Сохранить как...";
const pchar DialogFile::BTN_DELETE = "Удалить";


DialogFile::DialogFile() :
    MenuDialog("Файл", 125, { 2, 4 },
        BTN_NEW, []()
        {
            Model::CreateNew("Untitled");
            PanelModel::self->Update();
            DialogFile::self->Close(true);
        },
        BTN_OPEN, []()
        {
            wxFileDialog dialog(self, "Открыть файл модели измерения", wxEmptyString, wxEmptyString, "*.mod", wxFD_OPEN);

            if (dialog.ShowModal() == wxID_OK)
            {

            }
            else
            {

            }
        },
        BTN_CLOSE, []()
        {
            if (Model::IsModified())
            {
                wxMessageDialog dialog(self, "Файл модели был изменён. Сохранить изменения?", "Подтверждение", wxYES_NO | wxICON_QUESTION);

                if (dialog.ShowModal() == wxID_YES)
                {
                    wxFileDialog dialog_save(self, "Сохранить файл модели измерения", GF::DirForModFiles(), Model::GetName() + ".mod", "*.mod", wxFD_SAVE);

                    if (dialog_save.ShowModal() == wxID_OK)
                    {

                    }
                }
                else
                {
                    Model::Delete();
                    PanelModel::self->Update();
                }
            }

            DialogFile::self->Close(true);
        },
        BTN_SAVE, []()
        {
            DialogFile::self->Close(true);
        },
        BTN_SAVE_AS, []()
        {
            wxFileDialog dialog(self, "Сохранить файл модели измерения", GF::DirForModFiles(), Model::GetName() + ".mod", "*.mod", wxFD_SAVE);

            if (dialog.ShowModal() == wxID_OK)
            {
                Model::Delete();
                PanelModel::self->Update();
            }
            else
            {

            }
        },
        BTN_DELETE, []()
        {

        }
    )
{
    self = this;

    MenuDialog::Update();
}


void DialogFile::Update()
{
    MenuDialog::Update();

    bool empty = Model::IsEmpty();

    FindButton(BTN_NEW)->Enable(empty);
    FindButton(BTN_OPEN)->Enable(empty);
    FindButton(BTN_CLOSE)->Enable(!empty);
    FindButton(BTN_SAVE)->Enable(!empty);
    FindButton(BTN_SAVE_AS)->Enable(!empty);
}
