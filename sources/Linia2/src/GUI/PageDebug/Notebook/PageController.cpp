// 2026/08/19 11:02:33 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "GUI/PageDebug/Notebook/PageController.h"
#include "GUI/Controls/StaticBox.h"
#include "GUI/Controls/Button.h"
#include "GUI/Controls/StaticText.h"
#include "GUI/Controls/Sizers.h"


PageController *PageController::self = nullptr;


PageController::PageController(wxNotebook *notebook) :
    PageChip(notebook, "stm32")
{
    self = this;

    StaticBox *box = new StaticBox(this, L("Обновление прошивки"));

    // Вертикальный сайзер для всего содержимого бокса
    wxStaticBoxSizer *boxSizer = new wxStaticBoxSizer(box, wxVERTICAL);

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
    wxBoxSizer *updateRowSizer = new wxBoxSizer(wxHORIZONTAL);
    updateRowSizer->AddStretchSpacer();
    updateRowSizer->Add(updateButton, 0, wxALL, 5);
    updateRowSizer->AddStretchSpacer();

    boxSizer->Add(updateRowSizer, 0, wxEXPAND | wxBOTTOM, 5);

    // Устанавливаем сайзер на панель PageController
    // Предполагается, что PageChip уже имеет сайзер или его нужно создать
    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->Add(boxSizer, 0, wxALL, 10);
    SetSizer(mainSizer);
}
