// 2025/7/12 10:59:56 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PanelMenu/DialogModels.h"
#include "IPPP/Tests/Model.h"
#include "Panels/PanelConfig/PanelModel.h"


DialogModels *DialogModels::self = nullptr;


const wxString DialogModels::BTN_NEW = wxT("Новый");
const wxString DialogModels::BTN_OPEN = wxT("Открыть");
const wxString DialogModels::BTN_CLOSE = wxT("Закрыть");
const wxString DialogModels::BTN_SAVE = wxT("Cохранить");
const wxString DialogModels::BTN_SAVE_AS = wxT("Сохранить как...");
const wxString DialogModels::BTN_DELETE = wxT("Удалить");


DialogModels::DialogModels() :
    MenuDialog(wxT("Модели"), 125, { 2, 4 },
        BTN_NEW, []()
        {
            Model::CreateNew("Untitled");
            PanelModel::self->Update();
        },
        BTN_OPEN, []()
        {
            wxFileDialog dialog(self, wxT("Открыть файл модели измерения"), wxEmptyString, wxEmptyString, "*.mod", wxFD_OPEN);

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
                wxMessageDialog dialog(self, wxT("Файл модели был изменён. Сохранить изменения?"), wxT("Подтверждение"), wxYES_NO | wxICON_QUESTION);

                if (dialog.ShowModal() == wxID_YES)
                {
                    wxFileDialog dialog_save(self, wxT("Сохранить файл модели измерения"), GF::DirForModFiles(), Model::GetName() + ".mod", "*.mod", wxFD_SAVE);

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

            DialogModels::self->Close(true);
        },
        BTN_SAVE, []()
        {
            DialogModels::self->Close(true);
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


void DialogModels::Update()
{
    MenuDialog::Update();

    bool empty = Model::IsEmpty();

    FindButton(BTN_NEW)->Enable(empty);
    FindButton(BTN_OPEN)->Enable(empty);
    FindButton(BTN_CLOSE)->Enable(!empty);
    FindButton(BTN_SAVE)->Enable(!empty);
    FindButton(BTN_SAVE_AS)->Enable(!empty);
}
