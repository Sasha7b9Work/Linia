// 2025/7/12 10:59:56 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panel11_Menu/DialogFile.h"



DialogFile *DialogFile::self = nullptr;


DialogFile::DialogFile() :
    MenuDialog(_L("Файл"), 125,
        _L("Новый"), ID_BTN_MENU_FILE_NEW, OnButtonNew,
        _L("Открыть"), ID_BTN_MENU_FILE_OPEN, OnButtonOpen,
        _L("Закрыть"), ID_BTN_MENU_FILE_CLOSE, OnButtonClose,
        _L("Cохранить"), ID_BTN_MENU_FILE_SAVE, OnButtonSave,
        _L("Сохранить как..."), ID_BTN_MENU_FILE_SAVE_AS, OnButtonSaveAs,
        _L("Удалить"), ID_BTN_MENU_FILE_DELETE, OnButtonDelete
    )
{
    self = this;
}


void DialogFile::OnButtonNew()
{
    wxFileDialog dialog(self, _L("Новый файл модели измерения"), wxEmptyString, wxEmptyString, "*.mod", wxFD_SAVE);

    if (dialog.ShowModal() == wxID_OK)
    {

    }
    else
    {

    }
}


void DialogFile::OnButtonOpen()
{
    wxFileDialog dialog(self, _L("Открыть файл модели измерения"), wxEmptyString, wxEmptyString, "*.mod", wxFD_OPEN);

    if (dialog.ShowModal() == wxID_OK)
    {

    }
    else
    {

    }
}


void DialogFile::OnButtonClose()
{
    DialogFile::self->Close(true);
}


void DialogFile::OnButtonSave()
{
    DialogFile::self->Close(true);
}


void DialogFile::OnButtonSaveAs()
{
    wxFileDialog dialog(self, _L("Сохранить файл модели измерения"), wxEmptyString, wxEmptyString, "*.mod", wxFD_SAVE);

    if (dialog.ShowModal() == wxID_OK)
    {

    }
    else
    {

    }
}


void DialogFile::OnButtonDelete()
{

}
