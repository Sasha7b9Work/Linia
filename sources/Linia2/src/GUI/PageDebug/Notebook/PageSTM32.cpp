// 2026/08/19 11:02:33 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "GUI/PageDebug/Notebook/PageSTM32.h"
#include "GUI/Controls/StaticBox.h"
#include "GUI/Controls/Button.h"
#include "GUI/Controls/StaticText.h"
#include "GUI/Controls/Sizers.h"
#include "IPPP/IDevice.h"
#include "Utils/GlobalFunctions.h"
#pragma warning(push, 0)
    #include <wx/filedlg.h>
    #include <wx/file.h>
#pragma warning(pop)


PageSTM32 *PageSTM32::self = nullptr;


PageSTM32::PageSTM32(wxNotebook *notebook) :
    PageChip(notebook, "stm32")
{
    self = this;

    StaticBox *box = new StaticBox(this, L("Обновление прошивки"));

    StaticBoxSizer *boxSizer = new StaticBoxSizer(box, wxVERTICAL);

    BoxSizerHor *fileRowSizer = new BoxSizerHor();

    Button *selectButton = new Button(box, L("Выбрать файл"));
    fileRowSizer->Add(selectButton, 0, wxALL, 5);

    StaticText *fileNameText = new StaticText(box, L("Файл не выбран"));
    fileNameText->SetMinSize({ 300, TEXTCNTRL_HEIGHT + 5 });
    fileNameText->SetSize({ 300, TEXTCNTRL_HEIGHT + 5 });
    fileRowSizer->Add(fileNameText, 1, wxALIGN_CENTER_VERTICAL | wxALL, 5);

    boxSizer->Add(fileRowSizer, 0, wxEXPAND | wxALL, 5);

    Button *updateButton = new Button(box, L("Обновить"));
    updateButton->Enable(false);

    updateButton->Bind(wxEVT_BUTTON, [this, fileNameText](wxCommandEvent &)
        {
            StartUpgrade(fileNameText->GetLabel());
        });

    BoxSizerHor *updateRowSizer = new BoxSizerHor();
    updateRowSizer->AddStretchSpacer();
    updateRowSizer->Add(updateButton, 0, wxALL, 5);
    updateRowSizer->AddStretchSpacer();

    boxSizer->Add(updateRowSizer, 0, wxEXPAND | wxBOTTOM, 5);

    BoxSizerVert *mainSizer = new BoxSizerVert();
    mainSizer->Add(boxSizer, 0, wxALL, 10);
    SetSizer(mainSizer);

    {
        selectButton->Bind(wxEVT_BUTTON, [this, fileNameText, updateButton](wxCommandEvent &)
            {
                wxFileDialog dialog(this,
                    L("Выберите файл прошивки"),            // Заголовок окна
                    L("/media/mnipi"),                      // Начальная папка (пусто = текущая)
                    wxEmptyString,                          // Начальное имя файла
                    L("Файлы прошивок (*.bin)|*.bin"),      // Фильтры
                    wxFD_OPEN | wxFD_FILE_MUST_EXIST);      // Стили

                // Показываем диалог и проверяем результат
                if (dialog.ShowModal() == wxID_OK)
                {
                    wxString path = dialog.GetPath();                    // Полный путь к файлу
                    fileNameText->SetLabel(path);                        // Отображаем путь
                    updateButton->Enable(true);                          // Активируем кнопку "Обновить"
                }
            });
    }

    box->Layout();
}


void PageSTM32::StartUpgrade(pchar _file_name)
{
    if (is_running.exchange(true))
    {
        return;
    }

    file_name = _file_name;

    wxFile file(file_name, wxFile::read);
    if (!file.IsOpened())
    {
        LOG_ERROR("Can not open file %s", file_name.c_str().AsChar());

        return;
    }

    int size = (int)file.Length();

    if (size == (int)wxInvalidOffset)
    {
        LOG_ERROR("Can not get size file %s", file_name.c_str().AsChar());

        return;
    }

    data.resize((uint64)size);

    ssize_t bytes_read = file.Read(data.data(), data.size());

    if (bytes_read != size)
    {
        LOG_ERROR("Readed %d bytes from %d", bytes_read, size);

        return;
    }

    IDevice::impl->SendCommand(":UPGRADE:START %d", size);

    current_block = -1;
    state = IDLE;

    thread = std::thread([this]()
        {
            while (is_running.load())
            {
                switch (state)
                {
                case IDLE:
                    break;

                case START_UPGRADE:
                    break;

                case PROCESS_UPGRADE:
                    break;

                case END_UPGRADE:
                    break;

                case Count:
                    break;
                }
            }

            std::this_thread::sleep_for(std::chrono::microseconds(10));
        });
}


void PageSTM32::StopUpgrade()
{
    if (!is_running.exchange(false))
    {
        return;
    }

    if (thread.joinable())
    {
        thread.join();
    }
}


void PageSTM32::OnUpgradeStart(int size, uint crc32, bool is_ok)
{
    if (!is_ok)
    {
        StopUpgrade();

        StartUpgrade(file_name);
    }

    if (size != (int)data.size())
    {
        StopUpgrade();

        StartUpgrade(file_name);
    }

    if (crc32 != GF::CalculateCRC32(data.data(), (int)data.size()))
    {
        StopUpgrade();

        StartUpgrade(file_name);
    }

    state = START_UPGRADE;
}


void PageSTM32::OnUpgradeBlock(int num_block, int size, uint crc32)
{

}


void PageSTM32::OnUpgradeEnd(int size, uint crc32)
{

}
