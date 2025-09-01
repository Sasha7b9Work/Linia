// 2025/09/01 12:26:51 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class RestrictedFileDialog : public wxDialog
{
public:
    RestrictedFileDialog(wxWindow *parent, const wxString &restrictedPath)
        : wxDialog(parent, wxID_ANY, "Выберите файл", wxDefaultPosition, wxSize(600, 400)),
        m_restrictedPath(restrictedPath)
    {
        wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

        // Дерево каталогов, начинающееся с ограниченного пути
        m_dirCtrl = new wxGenericDirCtrl(this, wxID_ANY, restrictedPath,
            wxDefaultPosition, wxDefaultSize,
            wxDIRCTRL_DIR_ONLY);

        // Список файлов
        m_fileList = new wxListBox(this, wxID_ANY);

        wxBoxSizer *contentSizer = new wxBoxSizer(wxHORIZONTAL);
        contentSizer->Add(m_dirCtrl, 1, wxEXPAND | wxALL, 5);
        contentSizer->Add(m_fileList, 1, wxEXPAND | wxALL, 5);

        mainSizer->Add(contentSizer, 1, wxEXPAND);

        // Кнопки
        wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
        buttonSizer->Add(new wxButton(this, wxID_OK, "OK"), 0, wxRIGHT, 10);
        buttonSizer->Add(new wxButton(this, wxID_CANCEL, "Отмена"), 0, wxLEFT, 10);

        mainSizer->Add(buttonSizer, 0, wxALIGN_CENTER | wxALL, 10);

        SetSizer(mainSizer);

        // Привязка событий
        m_dirCtrl->Bind(wxEVT_DIRCTRL_SELECTIONCHANGED, &RestrictedFileDialog::OnDirChanged, this);
        UpdateFileList(restrictedPath);
    }

    wxString GetSelectedFile() const
    {
        return m_selectedFile;
    }

private:
    wxGenericDirCtrl *m_dirCtrl;
    wxListBox *m_fileList;
    wxString m_restrictedPath;
    wxString m_selectedFile;

    void OnDirChanged(wxCommandEvent & /*event*/)
    {
        wxString currentDir = m_dirCtrl->GetPath();
        UpdateFileList(currentDir);
    }

    void UpdateFileList(const wxString &directory)
    {
        m_fileList->Clear();
        wxArrayString files;

        wxDir::GetAllFiles(directory, &files);
        for (const auto &file : files)
        {
            m_fileList->Append(file);
        }
    }

    void OnOK(wxCommandEvent & /*event*/)
    {
        int selection = m_fileList->GetSelection();
        if (selection != wxNOT_FOUND)
        {
            m_selectedFile = m_fileList->GetString((uint)selection);
            EndModal(wxID_OK);
        }
        else
        {
            wxMessageBox("Выберите файл", "Внимание", wxOK | wxICON_WARNING, this);
        }
    }
};

