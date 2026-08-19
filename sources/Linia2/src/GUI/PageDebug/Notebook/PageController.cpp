// 2026/08/19 11:02:33 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "GUI/PageDebug/Notebook/PageController.h"
#include "GUI/Controls/StaticBox.h"
#include "GUI/Controls/Button.h"
#include "GUI/Controls/StaticText.h"
#include "GUI/Controls/Sizers.h"
#pragma warning(push, 0)
    #include <wx/filedlg.h>
#pragma warning(pop)


PageController *PageController::self = nullptr;


PageController::PageController(wxNotebook *notebook) :
    PageChip(notebook, "stm32")
{
    self = this;

    StaticBox *box = new StaticBox(this, L("Обновление прошивки"));

    // Вертикальный сайзер для всего содержимого бокса
    StaticBoxSizer *boxSizer = new StaticBoxSizer(box, wxVERTICAL);

    // --- Первая строка: кнопка "Выбрать файл" + название файла ---
    BoxSizerHor *fileRowSizer = new BoxSizerHor();

    Button *selectButton = new Button(box, L("Выбрать файл"));
    fileRowSizer->Add(selectButton, 0, wxALL, 5);

    StaticText *fileNameText = new StaticText(box, L("Файл не выбран"));
    fileRowSizer->Add(fileNameText, 1, wxALIGN_CENTER_VERTICAL | wxALL, 5);

    boxSizer->Add(fileRowSizer, 0, wxEXPAND | wxALL, 5);

    // --- Вторая строка: кнопка "Обновить" ---
    Button *updateButton = new Button(box, L("Обновить"));
    updateButton->Enable(false);  // По умолчанию отключена, пока не выбран файл

    // Центрируем кнопку по горизонтали
    BoxSizerHor *updateRowSizer = new BoxSizerHor();
    updateRowSizer->AddStretchSpacer();
    updateRowSizer->Add(updateButton, 0, wxALL, 5);
    updateRowSizer->AddStretchSpacer();

    boxSizer->Add(updateRowSizer, 0, wxEXPAND | wxBOTTOM, 5);

    // Устанавливаем сайзер на панель PageController
    // Предполагается, что PageChip уже имеет сайзер или его нужно создать
    BoxSizerVert *mainSizer = new BoxSizerVert();
    mainSizer->Add(boxSizer, 0, wxALL, 10);
    SetSizer(mainSizer);

    {
        selectButton->Bind(wxEVT_BUTTON, [this, fileNameText, updateButton](wxCommandEvent &)
            {
                wxFileDialog dialog(this,
                    L("Выберите файл прошивки"),          // Заголовок окна
                    wxEmptyString,                        // Начальная папка (пусто = текущая)
                    wxEmptyString,                        // Начальное имя файла
                    L("Файлы прошивок (*.bin;*.hex)|*.bin;*.hex|Все файлы (*.*)|*.*"),  // Фильтры
                    wxFD_OPEN | wxFD_FILE_MUST_EXIST);    // Стили

                // Показываем диалог и проверяем результат
                if (dialog.ShowModal() == wxID_OK)
                {
                    wxString path = dialog.GetPath();                    // Полный путь к файлу
                    fileNameText->SetLabel(path);                        // Отображаем путь
                    updateButton->Enable(true);                          // Активируем кнопку "Обновить"
                }
            });
    }
}
