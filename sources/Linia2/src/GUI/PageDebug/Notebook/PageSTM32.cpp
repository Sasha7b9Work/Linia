// 2026/08/19 11:02:33 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "GUI/PageDebug/Notebook/PageSTM32.h"
#include "GUI/Controls/StaticBox.h"
#include "GUI/Controls/Button.h"
#include "GUI/Controls/StaticText.h"
#include "GUI/Controls/Sizers.h"
#include "IPPP/IDevice.h"
#include "Utils/GlobalFunctions.h"
#include "Utils/Timer.h"
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

    btnUpgrade = new Button(box, L("Обновить"));
    btnUpgrade->Enable(false);

    btnUpgrade->Bind(wxEVT_BUTTON, [this, fileNameText](wxCommandEvent &)
        {
            if (wxFile::Exists(fileNameText->GetLabel()))
            {
                StartUpgrade(fileNameText->GetLabel());
            }
            else
            {
                btnUpgrade->Enable(false);
                fileNameText->SetLabel("");
            }
        });

    BoxSizerHor *updateRowSizer = new BoxSizerHor();
    updateRowSizer->AddStretchSpacer();
    updateRowSizer->Add(btnUpgrade, 0, wxALL, 5);
    updateRowSizer->AddStretchSpacer();

    boxSizer->Add(updateRowSizer, 0, wxEXPAND | wxBOTTOM, 5);

    BoxSizerVert *mainSizer = new BoxSizerVert();
    mainSizer->Add(boxSizer, 0, wxALL, 10);
    SetSizer(mainSizer);

    {
        selectButton->Bind(wxEVT_BUTTON, [this, fileNameText](wxCommandEvent &)
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
                    this->btnUpgrade->Enable(true);                      // Активируем кнопку "Обновить"
                }
            });
    }

    box->Layout();
}


void PageSTM32::StartUpgrade(pchar _file_name)
{
    btnUpgrade->Enable(false);
    duration_upgrade.Reset();

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
}


void PageSTM32::StopUpgrade()
{
    btnUpgrade->Enable(true);
    btnUpgrade->SetLabel(L("Обновить"));
    btnUpgrade->Refresh();
    btnUpgrade->Update();
}


void PageSTM32::OnConfirmUpgradeStart()
{
    current_block = -1;

    SendNextHeadBlock();
}


void PageSTM32::OnConfirmHeadBlock(int _num_block, int _size, uint _crc32)
{
    btnUpgrade->SetLabel(wxString::Format("%.1f", duration_upgrade.ElapsedMS() / 1e3f));

    if (_num_block != current_block)
    {
        ResetUpgrade();
    }
    else
    {
        int offset = 0;
        int size = 0;
        uint crc32 = 0;

        CalculateParametersBlock(current_block, offset, size, crc32);

        if ((size != _size) ||
            (crc32 != _crc32))
        {
            ResetUpgrade();
        }
        else
        {
            SendContentBlock();
        }
    }
}


void PageSTM32::OnConfirmContentBlock(int _num_block, int _size, uint _crc32)
{
    int offset = 0;
    int size = 0;
    uint crc32 = 0;

    CalculateParametersBlock(current_block, offset, size, crc32);

    if (current_block == _num_block &&
        size == _size &&
        crc32 == _crc32)
    {
        SendNextHeadBlock();
    }
    else
    {
        ResetUpgrade();
    }
}


void PageSTM32::OnConfirmUpgradeEnd(int size, uint crc32)
{
    if (size == (int)data.size() &&
        crc32 == GF::CalculateCRC32(data.data(), (int)data.size()))
    {
        StopUpgrade();
    }
    else
    {
        ResetUpgrade();
    }
}


void PageSTM32::OnError()
{
    ResetUpgrade();
}


void PageSTM32::ResetUpgrade()
{
    LOG_WRITE("PageSTM32::ResetUpgrade()");

    StopUpgrade();

    StartUpgrade(file_name);
}


void PageSTM32::SendNextHeadBlock()
{
    ++current_block;

    int offset = 0;
    int size = 0;
    uint crc32 = 0;

    if (CalculateParametersBlock(current_block, offset, size, crc32))
    {
        IDevice::impl->SendCommand(":UPGRADE:HEAD %d %d %X", current_block.load(), size, crc32);
    }
    else
    {
        crc32 = GF::CalculateCRC32(data.data(), (int)data.size());
        IDevice::impl->SendCommand(":UPGRADE:END %u %X", data.size(), crc32);
    }
}


void PageSTM32::SendContentBlock()
{
    int offset = 0;
    int size = 0;
    uint crc32 = 0;

    if (CalculateParametersBlock(current_block, offset, size, crc32))
    {
        IDevice::impl->SendBinaryData(data.data() + offset, size);
    }
    else
    {
        ResetUpgrade();
    }
}


bool PageSTM32::CalculateParametersBlock(int num_block, int &offset, int &size, uint &crc32)
{
    static const int SIZE_BLOCK = 2048;

    offset = -1;
    size = -1;
    crc32 = (uint)-1;

    offset = num_block * SIZE_BLOCK;

    if (offset >= (int)data.size())
    {
        return false;
    }

    size = std::min(static_cast<int>(data.size()) - offset, SIZE_BLOCK);

    crc32 = GF::CalculateCRC32(data.data() + offset, size);

    return true;
}
